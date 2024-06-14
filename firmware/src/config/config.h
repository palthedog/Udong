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
  for (int i = 0; i < 8; i++) {
    AnalogSwitchGroup group;
    group.analog_switch_group_id = i;
    group.trigger_type = i < 2 ? "rapid-trigger" : "static-trigger";

    group.rappid_trigger.act = 0.6;
    group.rappid_trigger.rel = 0.3;
    group.rappid_trigger.f_act = 3.8;
    group.rappid_trigger.f_rel = 0.2;

    group.static_trigger.act = 1.2;
    group.static_trigger.rel = 0.8;

    config.analog_switch_groups.push_back(group);
  }
  return config;
}

#endif
