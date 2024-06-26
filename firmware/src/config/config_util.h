#ifndef CONFIG_UTIL_H
#define CONFIG_UTIL_H

#include <Arduino.h>
#include <ArduinoJson.h>

// TODO: Get rid of Nanopb which doesn't support getter/setter
// As a result, the runtime doesn't know whether each field is set or not.
// So we need to maintain has_* fields manually. It is terrible for maintenance.
#include <pb_decode.h>
#include <pb_encode.h>

#include "../proto/config.pb.h"
#include "json_file.h"

// const String kUdongConfigPath = "/user/config.json";
const String kUdongConfigPath = "/user/config.binpb";

inline const AnalogSwitchGroup& getConfigFromGroupId(
    uint8_t analog_switch_group_id, const UdongConfig& config) {
  for (int i = 0; i < config.analog_switch_groups_count; i++) {
    const AnalogSwitchGroup& group = config.analog_switch_groups[i];
    if (group.analog_switch_group_id == analog_switch_group_id) {
      return group;
    }
  }

  // Not found.
  // TODO: Choose a solid/better solution
  return config.analog_switch_groups[0];
}

inline const AnalogSwitchGroup& getConfigFromSwitchId(
    uint8_t analog_switch_id, const UdongConfig& config) {
  for (int i = 0; i < config.analog_switch_assignments_count; i++) {
    const AnalogSwitchAssignment& assignment =
        config.analog_switch_assignments[i];
    if (assignment.analog_switch_id == analog_switch_id) {
      return getConfigFromGroupId(assignment.analog_switch_group_id, config);
    }
  }
  return config.analog_switch_groups[0];
}

const std::vector<ButtonId>& getAllButtonIds();

inline UdongConfig defaultUdongConfig() {
  UdongConfig config = UdongConfig_init_zero;
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

    AnalogSwitchAssignment& assignment = config.analog_switch_assignments[i];
    assignment.analog_switch_id = i;
    assignment.analog_switch_group_id = group_id;
  }
  config.analog_switch_assignments_count = 16;

  for (int i = 0; i < 8; i++) {
    AnalogSwitchGroup& group = config.analog_switch_groups[i];
    group.analog_switch_group_id = i;
    group.trigger_type =
        i < 4 ? TriggerType_RAPID_TRIGGER : TriggerType_STATIC_TRIGGER;
    group.has_static_trigger = true;
    group.has_rapid_trigger = true;
    group.static_trigger = {
        .act = 1.2,
        .rel = 0.8,
    };
    group.rapid_trigger = {
        .act = 0.8,
        .rel = 0.4,
        .f_act = 3.8,
        .f_rel = 0.2,
    };
  }
  config.analog_switch_groups_count = 8;

  // TODO: Assign human friendly button like assigning Down button to Switch-12
  const std::vector<ButtonId>& button_ids = getAllButtonIds();
  for (int i = 0; i < 16; i++) {
    ButtonAssignment& button_assignment = config.button_assignments[i];
    button_assignment.switch_id = i;
    button_assignment.has_button_id = true;
    button_assignment.button_id = button_ids[i];
  }
  config.button_assignments_count = 16;

  return config;
}

inline bool write_file_callback(
    pb_ostream_t* outs, const pb_byte_t* buf, size_t count) {
  Stream* stream = (Stream*)(outs->state);
  size_t written_cnt = stream->write(buf, count);
  return written_cnt == count;
}

inline pb_ostream_s pb_ofstream(File& file) {
  return {write_file_callback, &file, SIZE_MAX, 0};
}

inline bool write_vector_callback(
    pb_ostream_t* outs, const pb_byte_t* buf, size_t count) {
  std::vector<uint8_t>* vec = (std::vector<uint8_t>*)(outs->state);
  vec->reserve(vec->size() + count);
  vec->insert(vec->end(), buf, buf + count);
  return true;
}

inline pb_ostream_s pb_ovstream(std::vector<uint8_t>& buf) {
  return {write_vector_callback, &buf, SIZE_MAX, 0};
}

inline bool SaveProtoBin(
    const String& path, const pb_msgdesc_t& fields, const void* msg) {
  File file = LittleFS.open(path, "w");
  if (!file) {
    Serial.printf("file doesn't exist: %s\n", path.c_str());
    return false;
  }

  pb_ostream_s outs = pb_ofstream(file);
  bool enc_result = pb_encode(&outs, &fields, msg);
  Serial.printf("enc_result: %d\n", enc_result);

  return true;
}

inline static bool read_callback(
    pb_istream_t* ins, pb_byte_t* buf, size_t count) {
  Stream* stream = (Stream*)ins->state;
  size_t read_cnt = stream->readBytes(buf, count);
  return read_cnt == count;
};

inline pb_istream_s pb_ifstream(File& file) {
  return {read_callback, &file, file.size(), 0};
};

inline pb_istream_s pb_isstream(Stream& stream, size_t size) {
  return {read_callback, &stream, size, 0};
};

inline bool LoadProtoBin(
    const String& path, const pb_msgdesc_t* fields, void* dst) {
  File file = LittleFS.open(path, "r");
  if (!file) {
    Serial.printf("file doesn't exist: %s\n", path.c_str());
    return false;
  }

  pb_istream_s ins = pb_ifstream(file);
  bool dec_result = pb_decode(&ins, fields, dst);
  Serial.printf("dec_result: %d\n", dec_result);

  return true;
}

template <typename T>
inline void complementArray(
    const T src[], pb_size_t src_count, T dst[], pb_size_t& dst_count) {
  while (dst_count < src_count) {
    dst[dst_count] = src[dst_count];
    dst_count++;
  }
}

inline void complementWithDefaultValues(UdongConfig& config) {
  UdongConfig def = defaultUdongConfig();
  complementArray(
      def.analog_switch_assignments,
      def.analog_switch_assignments_count,
      config.analog_switch_assignments,
      config.analog_switch_assignments_count);
  complementArray(
      def.analog_switch_groups,
      def.analog_switch_groups_count,
      config.analog_switch_groups,
      config.analog_switch_groups_count);
  complementArray(
      def.button_assignments,
      def.button_assignments_count,
      config.button_assignments,
      config.button_assignments_count);
}

inline UdongConfig loadUdonConfig() {
  Serial.println("Load UdongConfig");

  UdongConfig config = UdongConfig_init_zero;
  if (!LoadProtoBin(kUdongConfigPath, &UdongConfig_msg, &config)) {
    Serial.println("Failed to load saved UdongConfig. Use default one.");
    return defaultUdongConfig();
  }
  complementWithDefaultValues(config);
  return config;
}

inline void saveUdonConfig(const UdongConfig& config) {
  SaveProtoBin(kUdongConfigPath, UdongConfig_msg, &config);
}

#endif
