#include "serial.h"

#include <Arduino.h>

#include <sstream>

#include "decaproto/decoder.h"
#include "decaproto/encoder.h"
#include "decaproto/stream/stl.h"
#include "decaproto/stream/string_stream.h"
#include "proto/config.pb.h"
#include "proto/rpc.pb.h"

namespace {

void SendJsonResponse(const String& cmd, const JsonDocument& response) {
  String res_buf;
  serializeJson(response, res_buf);

  Serial.print(cmd.c_str());
  Serial.print(':');
  Serial.print(res_buf.c_str());
  Serial.print('\n');
}

std::string send_proto_buf;
bool SendProto(const char* cmd, const decaproto::Message& msg) {
  send_proto_buf.clear();
  send_proto_buf.reserve(512);

  // For now, it's much faster (about 4x) to use StringOutputStream than
  // ArduinoSerialOutputStream maybe because our poor Stream write data one byte
  // at a time.
  decaproto::StringOutputStream sos(&send_proto_buf);
  size_t written_size;

  if (!msg.Encode(sos, written_size)) {
    Serial.println("Failed to encode a message");
    return false;
  }
  Serial.printf("%s@%u#", cmd, written_size);
  Serial.write(send_proto_buf.data(), written_size);
  return true;
}

void HandleGetConfig(Udong& context, const String& cmd) {
  UdongConfig config = context.GetConfig();
  if (!SendProto(cmd.c_str(), config)) {
    Serial.println("Faild to send UdongConfig");
    return;
  }
}

void HandleSaveConfig(Udong& context, const UdongConfig& udong_config) {
  Serial.println("HandleSaveConfig");

  // Save the received UdongConfig
  saveUdonConfig(udong_config);
  // Then reload the config and reconstruct Circuit
  context.ReloadConfig();
}

}  // namespace

void SerialHandler::PushAnalogSwitchState(Udong& context) {
  switch_state_history_.PushAnalogSwitchState(context);
}

void SerialHandler::HandleGetAnalogSwitchStateRequest(
    Udong& context,
    const String& cmd,
    const GetAnalogSwitchStateRequest& request) {
  GetAnalogSwitchStateResponse response;
  switch_state_history_.GetAnalogSwitchStateHistory(request, response);

  if (!SendProto(cmd.c_str(), response)) {
    Serial.println("Faild to send get-analog-switch-state response");
  }
}

void SerialHandler::ReadCommand(Udong& context) {
  while (Serial.available()) {
    uint8_t ch = Serial.read();
    if (ch == ':') {
      // Command with Json payload.
      state_ = State::kReadingJsonPayload;
      return;
    } else if (ch == '@') {
      // Command with payload size.
      recv_payload_size_ = 0;
      state_ = State::kReadingPayloadSize;
      return;
    } else if (ch == '\n') {
      // No payload. Command only.
      HandleCmd(context, command_);
      return;
    } else if (ch == '\r') {
      // Ignore CR
      continue;
    } else {
      command_ += (char)ch;
    }
  }
}

void SerialHandler::HandleJsonCmd(
    Udong& context, const String& cmd, const JsonDocument& arg) {
  Serial.printf("JSON command: %s is not supported\n", cmd.c_str());
  Reset();
}

void SerialHandler::ReadJsonPayload(Udong& context) {
  while (Serial.available()) {
    uint8_t ch = Serial.read();
    if (ch == '\n') {
      // End of the payload.
      JsonDocument arg;
      deserializeJson(arg, recv_buffer_.data());
      HandleJsonCmd(context, command_, arg);
      return;
    } else {
      recv_buffer_.push_back(ch);
    }
  }
}

void SerialHandler::ReadPayloadSize(Udong& context) {
  while (Serial.available()) {
    uint8_t ch = Serial.read();
    if (ch == '#') {
      // End of the payload size.
      // Start reading the binary payload.
      state_ = State::kReadingBinaryPayload;
      if (recv_payload_size_ == 0) {
        // Payload size is zero
        HandleBinaryCommand(context, recv_buffer_.data(), 0);
        return;
      }
      return;
    } else {
      recv_payload_size_ = recv_payload_size_ * 10 + (ch - '0');
    }
  }
}

void SerialHandler::ReadBinaryPayload(Udong& context) {
  while (Serial.available()) {
    uint8_t ch = Serial.read();
    recv_buffer_.push_back(ch);
    if (recv_buffer_.size() == recv_payload_size_) {
      // End of the payload.
      HandleBinaryCommand(context, recv_buffer_.data(), recv_payload_size_);
      break;
    }
  }
}

// TODO: Move to the decaproto library
class CstrInputStream : public decaproto::InputStream {
  const char* data_;
  size_t size_;

 public:
  CstrInputStream(const char* data, size_t size) : data_(data), size_(size) {
  }

  virtual ~CstrInputStream() {
  }

  virtual bool Read(uint8_t& out) {
    if (size_ == 0) {
      return false;
    }

    out = *data_;
    data_++;
    size_--;
    return true;
  }
};

void SerialHandler::HandleBinaryCommand(
    Udong& context, const uint8_t* binary, size_t size) {
  if (command_ == "save-config") {
    CstrInputStream cis((const char*)binary, size);
    UdongConfig config;
    DecodeMessage(cis, &config);

    HandleSaveConfig(context, config);
  } else if (command_ == "get-analog-switch-state") {
    CstrInputStream cis((const char*)binary, size);

    GetAnalogSwitchStateRequest req;
    DecodeMessage(cis, &req);

    HandleGetAnalogSwitchStateRequest(context, command_, req);
  } else {
    Serial.printf("unknown-cmd: %s\n", command_.c_str());
  }

  Reset();
}

void SerialHandler::HandleSerial(Udong& context) {
  while (Serial.available()) {
    switch (state_) {
      case State::kReadingCommand:
        this->ReadCommand(context);
        break;
      case State::kReadingJsonPayload:
        this->ReadJsonPayload(context);
        break;
      case State::kReadingPayloadSize:
        this->ReadPayloadSize(context);
        break;
      case State::kReadingBinaryPayload:
        this->ReadBinaryPayload(context);
        break;
    }
  }
}

// String serial_buffer;
void SerialHandler::HandleCmd(Udong& context, const String& cmd) {
  Serial.printf("Received command: %s\n", cmd.c_str());
  if (cmd == "") {
    // Do nothing
  } else if (cmd == "dump") {
    for (auto& analog_switch : context.GetAnalogSwitches()) {
      Serial.printf("* Dump switch-%d\n", analog_switch->GetId());
      analog_switch->DumpLastState();
      analog_switch->DumpLookupTable();
    }
  } else if (cmd == "reset") {
    Serial.println("Reset all calibration data");

    LittleFS.remove(kUdongConfigPath);
    LittleFS.remove(kCalibrationFilePath);

    context.ReloadConfig();
  } else if (cmd == "get-config") {
    HandleGetConfig(context, cmd);
  } else {
    Serial.print("Received unknown-cmd:");
    Serial.printf("%s\n", cmd.c_str());
  }

  Reset();
}
