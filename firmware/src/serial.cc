#include "serial.h"

// #include <ArduinoJson.h>
//  #include <pb_decode.h>
//  #include <pb_encode.h>

#include <sstream>

#include "decaproto/decoder.h"
#include "decaproto/encoder.h"
#include "decaproto/stream/stl.h"
#include "proto/config.pb.h"

namespace {

void SendJsonResponse(const String& cmd, const JsonDocument& response) {
  String res_buf;
  serializeJson(response, res_buf);

  Serial.print(cmd.c_str());
  Serial.print(':');
  Serial.print(res_buf.c_str());
  Serial.print('\n');
}

void HandleGet(Udong& context, const String& cmd) {
  JsonDocument json_response;

  JsonArray analog_switches = json_response["analog_switches"].to<JsonArray>();
  for (auto& analog_switch : context.circuit->analog_switches) {
    JsonVariant var = analog_switches.add<JsonVariant>();
    var["id"] = analog_switch->GetId();
    var["press_mm"] = analog_switch->GetLastPressMm();
  }

  SendJsonResponse(cmd, json_response);
}

void HandleGetConfig(Udong& context, const String& cmd) {
  /*
  std::vector<uint8_t> payload;
  pb_ostream_t outs = pb_ovstream(payload);
  pb_encode(&outs, &UdongConfig_msg, &context.circuit->config);
  */

  std::ostringstream oss;
  decaproto::StlOutputStream sos(&oss);
  size_t written_size;

  UdongConfig config = context.circuit->config;
  config.clear_analog_switch_assignments();  // OK

  /////////
  // config.clear_analog_switch_groups();  //  trying to fix

  auto fst = config.get_analog_switch_groups(0);
  config.clear_analog_switch_groups();
  AnalogSwitchGroup* ana_switch_group = config.add_analog_switch_groups();
  // ^ OK
  // ana_switch_group->set_analog_switch_group_id(8);
  ana_switch_group->set_analog_switch_group_id(0);
  // ^ OK

  Serial.printf("trigger type: %d\n", fst.trigger_type());
  ana_switch_group->set_trigger_type(TriggerType::RAPID_TRIGGER);
  //  ^ NG

  // ana_switch_group->set_trigger_type(fst.trigger_type());
  //  ^ NG

  //  AnalogSwitchAssignment* aasa = config.add_analog_switch_assignments();

  /////

  config.clear_button_assignments();  // < ng

  if (!EncodeMessage(sos, config, written_size)) {
    Serial.println("Failed to encode the message");
    return;
  }

  std::string payload = oss.str();
  Serial.printf("Encoded message size: %d %d\n", written_size, payload.size());
  Serial.flush();

  Serial.printf("%s@%u#", cmd.c_str(), written_size);
  size_t sent = Serial.write(payload.data(), payload.size());
  Serial.flush();

  Serial.printf("Encoded message sent: %d\n", sent);
}

void HandleSaveConfig(Udong& context, const UdongConfig& udong_config) {
  // Save the received UdongConfig
  saveUdonConfig(udong_config);
  // Then reload the config and reconstruct Circuit
  context.ReloadConfig();
}

}  // namespace

void SerialHandler::ReadCommand(Udong& context) {
  while (Serial.available()) {
    uint8_t ch = Serial.read();
    if (ch == ':') {
      // Command with Json payload.
      state_ = State::kReadingJsonPayload;
      return;
    } else if (ch == '@') {
      // Command with payload size.
      payload_size_ = 0;
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
      deserializeJson(arg, payload_buffer_.data());
      HandleJsonCmd(context, command_, arg);
      return;
    } else {
      payload_buffer_.push_back(ch);
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
      return;
    } else {
      payload_size_ = payload_size_ * 10 + (ch - '0');
    }
  }
}

void SerialHandler::ReadBinaryPayload(Udong& context) {
  while (Serial.available()) {
    uint8_t ch = Serial.read();
    payload_buffer_.push_back(ch);
    if (payload_buffer_.size() == payload_size_) {
      // End of the payload.
      HandleBinaryCommand(context, payload_buffer_.data(), payload_size_);
      return;
    }
  }
}

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
  //  pb_istream_t ins = pb_istream_from_buffer(binary, payload_size_);

  if (command_ == "save-config") {
    CstrInputStream cis((const char*)binary, size);
    // std::string buf(reinterpret_cast<const char*>(binary), size);
    // pb_decode(&ins, &UdongConfig_msg, &arg);
    UdongConfig config;
    DecodeMessage(cis, &config);

    HandleSaveConfig(context, config);
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
  Circuit& circuit = *context.circuit;
  if (cmd == "") {
    // Do nothing
  } else if (cmd == "dump") {
    for (auto& analog_switch : circuit.analog_switches) {
      Serial.printf("* Dump switch-%d\n", analog_switch->GetId());
      analog_switch->DumpLastState();
      analog_switch->DumpLookupTable();
    }
  } else if (cmd == "reset") {
    Serial.println("Reset all calibration data");

    LittleFS.remove(kUdongConfigPath);
    LittleFS.remove(kCalibrationFilePath);

    context.ReloadConfig();
  } else if (cmd == "get") {
    HandleGet(context, cmd);
  } else if (cmd == "get-config") {
    HandleGetConfig(context, cmd);
  } else if (cmd == "save-config") {
    // TODO: Support this command!
    // HandleSaveConfig(context, cmd, arg);
    Serial.println("save-config is not supported yet");
  } else {
    Serial.print("Received unknown-cmd:");
    Serial.printf("%s\n", cmd.c_str());
  }

  Reset();
}
