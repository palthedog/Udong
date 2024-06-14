#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <inttypes.h>

#include <vector>

struct RapidTriggerConfig {
  double act;
  double rel;
  double f_act;
  double f_rel;

  // Default config
  RapidTriggerConfig() : act(0.8), rel(0.4), f_act(3.8), f_rel(0.2) {
  }
};

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

inline bool convertToJson(const StaticTriggerConfig& config, JsonVariant dst) {
  dst["act"] = config.act;
  dst["rel"] = config.rel;
  return true;
}

struct AnalogSwitchAssignment {
  uint8_t analog_switch_id;
  uint8_t analog_switch_group_id;
};

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
  RapidTriggerConfig rappid_trigger;
  StaticTriggerConfig static_trigger;
};

inline bool convertToJson(const AnalogSwitchGroup& config, JsonVariant dst) {
  dst["analog_switch_group_id"] = config.analog_switch_group_id;
  dst["trigger_type"] = config.trigger_type;

  convertToJson(config.rappid_trigger, dst["rapid_trigger"].to<JsonObject>());
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

  const AnalogSwitchGroup& getConfigFromSwitchId(uint8_t analog_switch_id) {
    for (const AnalogSwitchAssignment& conf : analog_switch_assignments) {
      if (conf.analog_switch_id == analog_switch_id) {
        return getConfigFromGroupId(conf.analog_switch_group_id);
      }
    }
    return analog_switch_groups[0];
  }
};

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
  return true;
}

inline UdongConfig defaultUdongConfig() {
  UdongConfig config;

  for (int i = 0; i < 16; i++) {
    AnalogSwitchAssignment assignment;
    assignment.analog_switch_id = i;

    assignment.analog_switch_group_id = i < 8 ? 0 : 4;
    config.analog_switch_assignments.push_back(assignment);
  }

  for (int i = 0; i < 8; i++) {
    AnalogSwitchGroup group;
    group.analog_switch_group_id = i;
    group.trigger_type = i < 4 ? "rapid-trigger" : "static-trigger";
    config.analog_switch_groups.push_back(group);
  }
  return config;
}

inline UdongConfig loadUdonConfig() {
  return defaultUdongConfig();
}

#endif
