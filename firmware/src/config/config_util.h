#ifndef CONFIG_UTIL_H
#define CONFIG_UTIL_H

#include <Arduino.h>
#include <LittleFS.h>
#include <decaproto/descriptor.h>

#include "decaproto/decoder.h"
#include "decaproto/encoder.h"
#include "decaproto/message.h"
#include "decaproto/stream/stream.h"
#include "proto/config.pb.h"

const String kUdongConfigPath = "/user/config.binpb";

// TODO: Move to decaproto library
class ArduinoSerialInputStream : public decaproto::InputStream {
  Stream* stream_;

 public:
  ArduinoSerialInputStream(Stream* stream) : stream_(stream) {
  }

  virtual ~ArduinoSerialInputStream() {
  }

  virtual bool Read(uint8_t& out) {
    if (stream_->available() == 0) {
      return false;
    }
    out = stream_->read();
    return true;
  }
};

class ArduinoSerialOutputStream : public decaproto::OutputStream {
  Stream* stream_;

 public:
  ArduinoSerialOutputStream(Stream* stream) : stream_(stream) {
  }

  virtual ~ArduinoSerialOutputStream() {
  }

  virtual bool Write(uint8_t ch) {
    return stream_->write(ch) == 1;
  }
};

inline ButtonId DPadButton(DPadButtonSelector::Direction direction) {
  ButtonId dpad_button;
  dpad_button.set_type(ButtonType::D_PAD);
  dpad_button.mutable_d_pad()->set_direction(direction);
  return dpad_button;
}

inline ButtonId PushButton(uint32_t id) {
  ButtonId push_button;
  push_button.set_type(ButtonType::PUSH);
  push_button.mutable_push_button()->set_push_button_id(id);
  return push_button;
}

inline const AnalogSwitchGroup& getConfigFromGroupId(
    uint8_t analog_switch_group_id, const UdongConfig& config) {
  for (const AnalogSwitchGroup& group : config.analog_switch_groups()) {
    if (group.analog_switch_group_id() == analog_switch_group_id) {
      return group;
    }
  }

  // Not found.
  // TODO: Choose a solid/better solution
  Serial.printf(
      "ERROR: Given analog switch group ID(%d) doesn't exist in the "
      "UdonConfig.\n",
      analog_switch_group_id);
  return config.analog_switch_groups()[0];
}

inline const AnalogSwitchGroup& getConfigFromSwitchId(
    uint8_t analog_switch_id, const UdongConfig& config) {
  Serial.printf("config count: %d\n", config.analog_switch_configs_size());
  for (const AnalogSwitchConfig& aconf : config.analog_switch_configs()) {
    if (aconf.analog_switch_id() == analog_switch_id) {
      return getConfigFromGroupId(aconf.analog_switch_group_id(), config);
    }
  }

  Serial.printf(
      "ERROR: Given analog switch ID(%d) doesn't exist in the UdonConfig.\n",
      analog_switch_id);
  return config.analog_switch_groups()[0];
}

// Default config for rev.1 board.
// TODO: Move it to the board config?
inline UdongConfig defaultUdongConfig() {
  UdongConfig config;
  const uint32_t kDPadGroup = 0;
  const uint32_t kElseGroup = 1;
  for (int i = 0; i < 16; i++) {
    uint8_t group_id;
    if (i >= 11 && i <= 15) {
      // D-pad
      group_id = kDPadGroup;
    } else {
      group_id = kElseGroup;
    }

    AnalogSwitchConfig* aconf = config.add_analog_switch_configs();
    aconf->set_analog_switch_id(i);
    aconf->set_analog_switch_group_id(group_id);
  }

  for (int i = 0; i < 8; i++) {
    AnalogSwitchGroup& group = *config.add_analog_switch_groups();
    group.set_analog_switch_group_id(i);

    // KS-20
    // https://www.gateron.com/products/gateron-ks-20-magnetic-white-switch-set
    group.set_total_travel_distance(4.1);

    group.set_trigger_type(
        i < 4 ? TriggerType::RAPID_TRIGGER : TriggerType::STATIC_TRIGGER);
    group.mutable_static_trigger()->set_act(1.2);
    group.mutable_static_trigger()->set_rel(0.8);

    group.mutable_rapid_trigger()->set_act(0.8);
    group.mutable_rapid_trigger()->set_rel(0.4);
    group.mutable_rapid_trigger()->set_f_act(3.6);
    group.mutable_rapid_trigger()->set_f_rel(0.2);
  }

  // TODO: Assign human friendly button like assigning Down button to Switch-12
  const int kDigitalMask = 0x100;
  std::map<int, uint32_t> hwid_to_button_id = {
      {5, 0},
      {7, 1},
      {3, 2},
      {0, 3},

      {1, 4},  // L1
      {2, 5},  // R1

      {6, 6},  // L2
      {4, 7},  // R2

      {10, 8},                // Select, Share
      {kDigitalMask | 0, 9},  // Start, Option

      {8, 10},  // L3
      {9, 11},  // R3

      {kDigitalMask | 1, 12},  // Mode
  };

  for (auto& it : hwid_to_button_id) {
    ButtonAssignment& assign = *config.add_button_assignments();
    bool is_digital = (it.first & kDigitalMask) != 0;
    if (!is_digital) {
      assign.mutable_switch_id()->set_type(SwitchType::ANALOG_SWITCH);
      assign.mutable_switch_id()->set_id(it.first);
    } else {
      assign.mutable_switch_id()->set_type(SwitchType::DIGITAL_SWITCH);
      assign.mutable_switch_id()->set_id(it.first ^ kDigitalMask);
    }
    *assign.mutable_button_id() = PushButton(it.second);
  }

  ButtonAssignment* assign;

  assign = config.add_button_assignments();
  assign->mutable_switch_id()->set_type(SwitchType::ANALOG_SWITCH);
  assign->mutable_switch_id()->set_id(11);
  *assign->mutable_button_id() = DPadButton(DPadButtonSelector::Direction::UP);

  assign = config.add_button_assignments();
  assign->mutable_switch_id()->set_type(SwitchType::ANALOG_SWITCH);
  assign->mutable_switch_id()->set_id(12);
  *assign->mutable_button_id() = DPadButton(DPadButtonSelector::Direction::UP);

  assign = config.add_button_assignments();
  assign->mutable_switch_id()->set_type(SwitchType::ANALOG_SWITCH);
  assign->mutable_switch_id()->set_id(14);
  *assign->mutable_button_id() =
      DPadButton(DPadButtonSelector::Direction::RIGHT);

  assign = config.add_button_assignments();
  assign->mutable_switch_id()->set_type(SwitchType::ANALOG_SWITCH);
  assign->mutable_switch_id()->set_id(15);
  *assign->mutable_button_id() =
      DPadButton(DPadButtonSelector::Direction::DOWN);

  assign = config.add_button_assignments();
  assign->mutable_switch_id()->set_type(SwitchType::ANALOG_SWITCH);
  assign->mutable_switch_id()->set_id(13);
  *assign->mutable_button_id() =
      DPadButton(DPadButtonSelector::Direction::LEFT);
  return config;
}

inline bool SaveProtoBin(
    const String& path, const decaproto::Message& message) {
  File file = LittleFS.open(path, "w");
  if (!file) {
    Serial.printf("file doesn't exist: %s\n", path.c_str());
    return false;
  }

  ArduinoSerialOutputStream asos(&file);
  size_t written_size;
  return message.Encode(asos, written_size);
}

inline bool LoadProtoBin(const String& path, decaproto::Message* dst) {
  File file = LittleFS.open(path, "r");
  if (!file) {
    Serial.printf("file doesn't exist: %s\n", path.c_str());
    return false;
  }

  Serial.printf("loading proto size: %d\n", file.size());
  ArduinoSerialInputStream asis(&file);
  return decaproto::DecodeMessage(asis, dst);
}

void ComplementMessage(const decaproto::Message& src, decaproto::Message& dst);

inline void complementWithDefaultValues(UdongConfig& config) {
  UdongConfig def = defaultUdongConfig();
  // TODO: Add unit test
  ComplementMessage(def, config);
}

// TODO: Implement DebugString in decaproto::Message and use it here
inline void printUdonConfig(const UdongConfig& config) {
  Serial.printf(
      "analog switch assignments size: %u\n",
      config.analog_switch_configs_size());
  for (const AnalogSwitchConfig& aconf : config.analog_switch_configs()) {
    Serial.printf(
        "   analog_switch_id: %lu, analog_switch_group_id: %lu\n",
        aconf.analog_switch_id(),
        aconf.analog_switch_group_id());
  }

  Serial.printf("group size: %u\n", config.analog_switch_groups_size());
  for (const AnalogSwitchGroup& group : config.analog_switch_groups()) {
    Serial.printf(
        "   group id: %lu, trigger_type: %d\n",
        group.analog_switch_group_id(),
        (int)group.trigger_type());
    if (group.trigger_type() == TriggerType::STATIC_TRIGGER) {
      Serial.printf(
          "     act: %.2lf, rel: %.2lf\n",
          group.static_trigger().act(),
          group.static_trigger().rel());
    } else {
      Serial.printf(
          "     act: %.2lf, rel: %.2lf, f_act: %.2lf, f_rel: %.2lf\n",
          group.rapid_trigger().act(),
          group.rapid_trigger().rel(),
          group.rapid_trigger().f_act(),
          group.rapid_trigger().f_rel());
    }
  }

  Serial.printf(
      "button assignments size: %u\n", config.button_assignments_size());
  for (const ButtonAssignment& button : config.button_assignments()) {
    Serial.printf("   switch_id: %lu\n", button.switch_id().id());
    const ButtonId& button_id = button.button_id();
    if (button_id.has_d_pad()) {
      Serial.printf("     d-pad: %d\n", button_id.d_pad().direction());
    } else {
      Serial.printf(
          "     push: %lu\n", button_id.push_button().push_button_id());
    }
  }
}

inline UdongConfig loadUdonConfig() {
  Serial.println("Load UdongConfig");

  UdongConfig config;
  if (!LoadProtoBin(kUdongConfigPath, &config)) {
    Serial.println("Failed to load saved UdongConfig. Use default one.");
    return defaultUdongConfig();
  }

  Serial.println("config from file");
  printUdonConfig(config);

  complementWithDefaultValues(config);

  Serial.println("merged one");
  printUdonConfig(config);

  return config;
}

inline void saveUdonConfig(const UdongConfig& config) {
  SaveProtoBin(kUdongConfigPath, config);
}

#endif
