#include "serial.h"

#include <ArduinoJson.h>

namespace {

void SendResponse(const String& cmd, const JsonDocument& response) {
  String res_buf;
  serializeJson(response, res_buf);

  Serial.print(cmd.c_str());
  Serial.print(':');
  Serial.print(res_buf.c_str());
  Serial.print('\n');
}

void HandleGet(Udong& context, const String& cmd, const JsonDocument& arg) {
  JsonDocument json_response;

  JsonArray analog_switches = json_response["analog_switches"].to<JsonArray>();
  for (auto& analog_switch : context.circuit->analog_switches) {
    JsonVariant var = analog_switches.add<JsonVariant>();
    var["id"] = analog_switch->GetId();
    var["press_mm"] = analog_switch->GetLastPressMm();
  }

  SendResponse(cmd, json_response);
}

void HandleGetConfig(
    Udong& context, const String& cmd, const JsonDocument& arg) {
  JsonDocument json_response;
  convertToJson(context.circuit->config, json_response.to<JsonObject>());
  SendResponse(cmd, json_response);
}

void HandleSaveConfig(
    Udong& context, const String& cmd, const JsonDocument& arg) {
  // Save the received UdongConfig
  UdongConfig config;
  convertFromJson(arg, config);
  saveUdonConfig(config);

  // Then reload the config and reconstruct Circuit
  context.ReloadConfig();
}

}  // namespace

void SerialHandler::HandleSerial(Udong& context) {
  while (Serial.available()) {
    char ch = Serial.read();
    if (ch == ':') {
      String cmd = serial_buffer;
      JsonDocument arg;
      deserializeJson(arg, Serial);
      HandleCmd(context, cmd, arg);
      serial_buffer = "";
    } else if (ch == '\n') {
      JsonDocument arg;
      HandleCmd(context, serial_buffer, arg);
      serial_buffer = "";
    } else {
      serial_buffer += ch;
    }
  }
}

// String serial_buffer;
void SerialHandler::HandleCmd(
    Udong& context, const String& cmd, const JsonDocument& arg) {
  Circuit& circuit = *context.circuit;
  if (cmd == "") {
    // Do nothing
  } else if (cmd == "dump") {
    for (auto& analog_switch : circuit.analog_switches) {
      Serial.printf("* Dump switch-%d\n", analog_switch->GetId());
      analog_switch->DumpLastState();
      analog_switch->DumpLookupTable();
    }
    return;
  } else if (cmd == "reset") {
    Serial.println("Reset all calibration data");

    LittleFS.remove(kUdongConfigPath);
    LittleFS.remove(kCalibrationFilePath);

    context.ReloadConfig();
  } else if (cmd == "get") {
    HandleGet(context, cmd, arg);
  } else if (cmd == "get-config") {
    HandleGetConfig(context, cmd, arg);
  } else if (cmd == "save-config") {
    HandleSaveConfig(context, cmd, arg);
  } else {
    Serial.print("unknown-cmd:");
    Serial.printf("%s\n", cmd.c_str());
  }
}
