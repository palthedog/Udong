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

  union {
    RapidTriggerConfig rappid_trigger;
    StaticTriggerConfig static_trigger;
  };
};

inline bool convertToJson(const AnalogSwitchGroup& config, JsonVariant dst) {
  dst["analog_switch_group_id"] = config.analog_switch_group_id;
  dst["trigger_type"] = config.trigger_type;

  if (config.trigger_type == "rapid-trigger") {
    convertToJson(config.rappid_trigger, dst["rapid_trigger"].to<JsonObject>());
  } else if (config.trigger_type == "static-trigger") {
    convertToJson(
        config.static_trigger, dst["static_trigger"].to<JsonObject>());
  } else {
    return false;
  }
  return true;
}

struct UdongConfig {
  std::vector<AnalogSwitchAssignment> analog_switch_assignments;

  std::vector<AnalogSwitchGroup> analog_switch_groups;
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

inline UdongConfig loadUdonConfig() {
  UdongConfig config;

  // dummy
  for (int i = 0; i < 16; i++) {
    AnalogSwitchAssignment assignment;
    assignment.analog_switch_id = i;

    assignment.analog_switch_group_id = i % 4;
    config.analog_switch_assignments.push_back(assignment);
  }

  // Rapid triggers
  for (int i = 0; i < 2; i++) {
    AnalogSwitchGroup group;
    group.analog_switch_group_id = i;
    group.trigger_type = "rapid-trigger";
    group.rappid_trigger.act = 0.6;
    group.rappid_trigger.rel = 0.3;
    group.rappid_trigger.f_act = 3.8;
    group.rappid_trigger.f_rel = 0.2;
    config.analog_switch_groups.push_back(group);
  }
  // Static triggers
  for (int i = 2; i < 4; i++) {
    AnalogSwitchGroup group;
    group.analog_switch_group_id = i;
    group.trigger_type = "static-trigger";
    group.static_trigger.act = 1.2;
    group.static_trigger.rel = 0.8;

    config.analog_switch_groups.push_back(group);
  }

  return config;
}

#endif
