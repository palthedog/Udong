#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <inttypes.h>

#include <vector>

#include "button/button.h"
#include "json_file.h"

const String kUdongConfigPath = "/user/config.json";

struct RapidTriggerConfig {
  double act;
  double rel;
  double f_act;
  double f_rel;

  // Default config
  RapidTriggerConfig() : act(0.8), rel(0.4), f_act(3.8), f_rel(0.2) {
  }
};

inline void convertFromJson(JsonVariantConst var, RapidTriggerConfig& dst) {
  dst.act = var["act"];
  dst.rel = var["rel"];
  dst.f_act = var["f_act"];
  dst.f_rel = var["f_rel"];
}

inline bool convertToJson(const RapidTriggerConfig& config, JsonVariant dst) {
  dst["act"] = config.act;
  dst["rel"] = config.rel;
  dst["f_act"] = config.f_act;
  dst["f_rel"] = config.f_rel;
  return true;
}

struct StaticTriggerConfig {
  double act;
  double rel;
  StaticTriggerConfig() : act(1.2), rel(0.8) {
  }
};

inline void convertFromJson(JsonVariantConst var, StaticTriggerConfig& dst) {
  dst.act = var["act"];
  dst.rel = var["rel"];
}

inline bool convertToJson(const StaticTriggerConfig& config, JsonVariant dst) {
  dst["act"] = config.act;
  dst["rel"] = config.rel;
  return true;
}

struct AnalogSwitchAssignment {
  uint8_t analog_switch_id;
  uint8_t analog_switch_group_id;

  AnalogSwitchAssignment() {
  }
};

inline void convertFromJson(JsonVariantConst var, AnalogSwitchAssignment& dst) {
  dst.analog_switch_id = var["analog_switch_id"];
  dst.analog_switch_group_id = var["analog_switch_group_id"];
}

inline bool convertToJson(
    const AnalogSwitchAssignment& config, JsonVariant dst) {
  dst["analog_switch_id"] = config.analog_switch_id;
  dst["analog_switch_group_id"] = config.analog_switch_group_id;
  return true;
}

struct AnalogSwitchGroup {
  uint8_t analog_switch_group_id;

  String trigger_type;

  // Keep configurations for all trigger type so that the user can keep
  // their setting after switch trigger mode.
  RapidTriggerConfig rapid_trigger;
  StaticTriggerConfig static_trigger;

  AnalogSwitchGroup() {
  }
};

inline void convertFromJson(JsonVariantConst var, AnalogSwitchGroup& dst) {
  dst.analog_switch_group_id = var["analog_switch_group_id"];

  dst.trigger_type = var["trigger_type"].as<const char*>();

  dst.rapid_trigger = var["rapid_trigger"];
  dst.static_trigger = var["static_trigger"];
}

inline bool convertToJson(const AnalogSwitchGroup& config, JsonVariant dst) {
  dst["analog_switch_group_id"] = config.analog_switch_group_id;
  dst["trigger_type"] = config.trigger_type;

  convertToJson(config.rapid_trigger, dst["rapid_trigger"].to<JsonObject>());
  convertToJson(config.static_trigger, dst["static_trigger"].to<JsonObject>());
  return true;
}

struct UdongConfig {
 private:
  const AnalogSwitchGroup& getConfigFromGroupId(
      uint8_t analog_switch_group_id) const {
    for (const AnalogSwitchGroup& conf : analog_switch_groups) {
      if (conf.analog_switch_group_id == analog_switch_group_id) {
        return conf;
      }
    }

    // Not found.
    // TODO: Choose a solid/better solution
    return analog_switch_groups[0];
  }

 public:
  std::vector<AnalogSwitchAssignment> analog_switch_assignments;
  std::vector<AnalogSwitchGroup> analog_switch_groups;
  std::vector<ButtonAssignment> button_assignments;

  UdongConfig() {
  }

  const AnalogSwitchGroup& getConfigFromSwitchId(uint8_t analog_switch_id) {
    for (const AnalogSwitchAssignment& conf : analog_switch_assignments) {
      if (conf.analog_switch_id == analog_switch_id) {
        return getConfigFromGroupId(conf.analog_switch_group_id);
      }
    }
    return analog_switch_groups[0];
  }
};

inline void convertFromJson(JsonVariantConst var, UdongConfig& dst) {
  size_t size = var["analog_switch_assignments"].size();
  for (size_t i = 0; i < size; i++) {
    dst.analog_switch_assignments.push_back(
        AnalogSwitchAssignment(var["analog_switch_assignments"][i]));
  }
  size = var["analog_switch_groups"].size();
  for (size_t i = 0; i < size; i++) {
    dst.analog_switch_groups.push_back(
        AnalogSwitchGroup(var["analog_switch_groups"][i]));
  }
  size = var["button_assignments"].size();
  for (size_t i = 0; i < size; i++) {
    dst.button_assignments.push_back(
        ButtonAssignment(var["button_assignments"][i]));
  }
}

inline bool convertToJson(const UdongConfig& config, JsonVariant dst) {
  for (size_t i = 0; i < config.analog_switch_assignments.size(); i++) {
    convertToJson(
        config.analog_switch_assignments[i],
        dst["analog_switch_assignments"][i].to<JsonObject>());
  }
  for (size_t i = 0; i < config.analog_switch_groups.size(); i++) {
    convertToJson(
        config.analog_switch_groups[i],
        dst["analog_switch_groups"][i].to<JsonObject>());
  }
  for (size_t i = 0; i < config.button_assignments.size(); i++) {
    convertToJson(
        config.button_assignments[i],
        dst["button_assignments"][i].to<JsonObject>());
  }
  return true;
}

inline UdongConfig defaultUdongConfig() {
  UdongConfig config;

  for (int i = 0; i < 16; i++) {
    uint8_t group_id;
    if (i >= 12 && i <= 15) {
      // D-pad
      group_id = 0;
    } else if (i == 2 || i == 4) {
      // for little fingers
      group_id = 1;
    } else {
      group_id = 4;
    }

    AnalogSwitchAssignment assignment;
    assignment.analog_switch_id = i;
    assignment.analog_switch_group_id = group_id;
    config.analog_switch_assignments.push_back(assignment);
  }

  for (int i = 0; i < 8; i++) {
    AnalogSwitchGroup group;
    group.analog_switch_group_id = i;
    group.trigger_type = i < 4 ? "rapid-trigger" : "static-trigger";
    config.analog_switch_groups.push_back(group);
  }

  // TODO: Assign human friendly button
  const std::vector<ButtonId>& button_ids = getAllButtonIds();
  Serial.println("all button IDs");
  for (int i = 0; i < button_ids.size(); i++) {
    const ButtonId& bid = button_ids[i];
    if (bid.type == ButtonType::PushButton) {
      Serial.printf(
          "i: %d, type: push, sel: %d\n",
          i,
          bid.selector.push_button.push_button_id);
    } else if (bid.type == ButtonType::DPadButton) {
      Serial.printf(
          "i: %d, type: d-pad, sel: %d\n", i, bid.selector.d_pad.direction);
    }
  }

  for (int i = 0; i < 16; i++) {
    ButtonAssignment button_assignment;
    button_assignment.switch_id = i;
    button_assignment.button_id = button_ids[i];
    Serial.printf(
        "sel: %d\n",
        button_assignment.button_id.selector.push_button.push_button_id);
    config.button_assignments.push_back(button_assignment);
  }
  return config;
}

template <typename T>
inline void complementVector(std::vector<T>& dst, const std::vector<T>& def) {
  while (dst.size() < def.size()) {
    dst.push_back(def[dst.size()]);
  }
}

inline void complementWithDefaultValues(UdongConfig& config) {
  UdongConfig def = defaultUdongConfig();
  complementVector(
      config.analog_switch_assignments, def.analog_switch_assignments);
  complementVector(config.analog_switch_groups, def.analog_switch_groups);
  complementVector(config.button_assignments, def.button_assignments);
}

inline UdongConfig loadUdonConfig() {
  Serial.println("Load UdongConfig");
  JsonDocument doc;
  if (!LoadJson(kUdongConfigPath, doc)) {
    Serial.println("Failed to load saved UdongConfig. Use default one.");
    return defaultUdongConfig();
  }
  UdongConfig config;
  convertFromJson(doc, config);
  complementWithDefaultValues(config);
  return config;
}

inline void saveUdonConfig(const UdongConfig& config) {
  JsonDocument doc;
  convertToJson(config, doc.to<JsonObject>());
  SaveJson(kUdongConfigPath, doc);
}

#endif
