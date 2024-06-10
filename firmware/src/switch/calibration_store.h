#ifndef CALIBRATION_STORE
#define CALIBRATIN_STORE

#include <ArduinoJson.h>
#include <LittleFS.h>

#include <map>

class AnalogSwitchCalibrationStore {
  int id_;
  double mag_flux_at_farest_;
  double mag_flux_at_nearest_;

  bool updated_;

 public:
  AnalogSwitchCalibrationStore()
      : id_(0),
        mag_flux_at_farest_(UINT16_MAX),
        mag_flux_at_nearest_(0),
        updated_(false) {
  }

  AnalogSwitchCalibrationStore(
      int id, double mag_flux_at_farest, double mag_flux_at_nearest)
      : id_(id),
        mag_flux_at_farest_(mag_flux_at_farest),
        mag_flux_at_nearest_(mag_flux_at_nearest) {
  }

  inline int GetId() const {
    return id_;
  }

  inline double GetMagFluxAtNearest() const {
    return mag_flux_at_nearest_;
  }

  inline void SetMagFluxAtNearest(double v) {
    mag_flux_at_nearest_ = v;
  }

  inline double GetMagFluxAtFarest() const {
    return mag_flux_at_farest_;
  }

  inline void SetMagFluxAtFarest(double v) {
    mag_flux_at_farest_ = v;
  }
};

inline bool convertToJson(
    const AnalogSwitchCalibrationStore& analog_switch, JsonVariant dst) {
  dst["id"] = analog_switch.GetId();
  dst["mag_flux_at_nearest"] = analog_switch.GetMagFluxAtNearest();
  dst["mag_flux_at_farest"] = analog_switch.GetMagFluxAtFarest();
  return true;
}

AnalogSwitchCalibrationStore convertFromJson(const JsonVariant& json) {
  int id = json["id"];
  double mag_flux_at_nearest = json["mag_flux_at_nearest"];
  double mag_flux_at_farest = json["mag_flux_at_farest"];
  return AnalogSwitchCalibrationStore(
      id, mag_flux_at_farest, mag_flux_at_nearest);
}

class CalibrationStore {
  const char* kFilePath = "/data/calibration.json";
  std::map<int, AnalogSwitchCalibrationStore> analog_switches_;

 public:
  AnalogSwitchCalibrationStore* GetSwitchRef(int id) {
    Serial.printf("Getting switch-calibration store. Id: %d\n", id);
    auto f = analog_switches_.find(id);
    if (f != analog_switches_.end()) {
      return &f->second;
    }

    Serial.printf("Not found in the saved data. Creating a new one: %id\n", id);
    // TODO: Read from factory data?
    return &analog_switches_
                .insert(std::make_pair(
                    id, AnalogSwitchCalibrationStore(id, UINT16_MAX, 0)))
                .first->second;
  }

  bool LoadFromFile() {
    File file = LittleFS.open(F(kFilePath), "r");
    if (!file) {
      Serial.println("calibration data doesn't exist");
      return false;
    }

    String jsonString = file.readString();
    Serial.println("Loaded JSON");
    Serial.println(jsonString);

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonString);
    if (error) {
      Serial.println("Failed to deserialize jsonString:");
      Serial.println(jsonString);
      Serial.println("Error:");
      Serial.print(error.c_str());
      return false;
    }

    // doc["analog_switches"];
    size_t as_size = doc["analog_switches"].size();
    for (size_t i = 0; i < as_size; i++) {
      JsonVariant var = doc["analog_switches"][i];
      AnalogSwitchCalibrationStore v = convertFromJson(var);
      Serial.printf("loaded analog switch: %d\n", v.GetId());
      Serial.printf("analog switches.size(): %d\n", analog_switches_.size());
      analog_switches_[v.GetId()] = v;
    }
    file.close();
    Serial.println("Calibration data loaded successfully");

    return true;
  }

  void SaveIntoFile() {
    File file = LittleFS.open(F(kFilePath), "w");
    if (!file) {
      Serial.println("Failed to open calibration data file in writable mode.");
      return;
    }

    JsonDocument doc;

    // doc["analog_switches"];
    Serial.printf("# of analog_switches: %d\n", analog_switches_.size());
    int index = 0;
    for (auto& it : analog_switches_) {
      doc["analog_switches"][index++] = it.second;
    }

    size_t size = serializeJsonPretty(doc, file);
    Serial.printf("Calibration data saved successfully: %d bytes\n", size);
    file.close();
  }
};

#endif
