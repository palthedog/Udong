#ifndef CONFIG_UTIL_H
#define CONFIG_UTIL_H

#include <Arduino.h>
// #include <ArduinoJson.h>
#include <LittleFS.h>

#include "../proto/config.pb.h"
// #include "json_file.h"

const String kUdongConfigPath = "/user/config.binpb";

inline const AnalogSwitchGroup& getConfigFromGroupId(
    uint8_t analog_switch_group_id, const UdongConfig& config) {
  for (const AnalogSwitchGroup& group : config.analog_switch_groups()) {
    if (group.analog_switch_group_id() == analog_switch_group_id) {
      return group;
    }
  }

  // Not found.
  // TODO: Choose a solid/better solution
  return config.analog_switch_groups()[0];
}

inline const AnalogSwitchGroup& getConfigFromSwitchId(
    uint8_t analog_switch_id, const UdongConfig& config) {
  for (const AnalogSwitchAssignment& assignment :
       config.analog_switch_assignments()) {
    if (assignment.analog_switch_id() == analog_switch_id) {
      return getConfigFromGroupId(assignment.analog_switch_group_id(), config);
    }
  }
  return config.analog_switch_groups()[0];
}

const std::vector<ButtonId>& getAllButtonIds();

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

    AnalogSwitchAssignment* assignment = config.add_analog_switch_assignments();
    assignment->set_analog_switch_id(i);
    assignment->set_analog_switch_group_id(group_id);
  }

  for (int i = 0; i < 8; i++) {
    AnalogSwitchGroup& group = *config.add_analog_switch_groups();
    group.set_analog_switch_group_id(i);
    group.set_trigger_type(
        i < 4 ? TriggerType::RAPID_TRIGGER : TriggerType::STATIC_TRIGGER);
    // group->has_static_trigger = true;
    // group.has_rapid_trigger = true;
    group.mutable_static_trigger()->set_act(1.2);
    group.mutable_static_trigger()->set_rel(0.8);
    group.mutable_rapid_trigger()->set_act(0.8);
    group.mutable_rapid_trigger()->set_rel(0.4);
    group.mutable_rapid_trigger()->set_f_act(3.8);
    group.mutable_rapid_trigger()->set_f_act(0.2);
  }

  // TODO: Assign human friendly button like assigning Down button to Switch-12
  const std::vector<ButtonId>& button_ids = getAllButtonIds();
  for (int i = 0; i < 16; i++) {
    ButtonAssignment& button_assignment = *config.add_button_assignments();
    button_assignment.set_switch_id(i);
    // button_assignment.has_button_id = true;
    *button_assignment.mutable_button_id() = button_ids[i];
  }

  return config;
}

/*
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
*/

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

/*
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
*/

inline bool LoadProtoBin(
    const String& path, const decaproto::Message* fields, void* dst) {
  File file = LittleFS.open(path, "r");
  if (!file) {
    Serial.printf("file doesn't exist: %s\n", path.c_str());
    return false;
  }

  // file
  //  pb_istream_s ins = pb_ifstream(file);
  //  bool dec_result = pb_decode(&ins, fields, dst);

  // Serial.printf("dec_result: %d\n", dec_result);

  return true;
}

template <typename T>
inline void complementArray(const std::vector<T>& src, std::vector<T>* dst) {
  dst.resize(src.size());
  for (size_t i = 0; i < src.size(); i++) {
    *dst[i] = src[i];
  }
}

// TODO: Implement decaproto::Message::MergeFrom and use it here
inline void complementWithDefaultValues(UdongConfig& config) {
  UdongConfig def = defaultUdongConfig();
  complementArray(
      def.analog_switch_assignments(),
      config.mutable_analog_switch_assignments());
  complementArray(
      def.analog_switch_groups(), config.mutable_analog_switch_groups());
  complementArray(
      def.button_assignments(), config.mutable_button_assignments());
}

inline UdongConfig loadUdonConfig() {
  Serial.println("Load UdongConfig");

  UdongConfig config;
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
