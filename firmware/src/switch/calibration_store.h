#ifndef CALIBRATION_STORE
#define CALIBRATIN_STORE

#include <ArduinoJson.h>
#include <LittleFS.h>

#include <map>

// TODO: Replace it with decaproto
#include "json_file.h"

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

  AnalogSwitchCalibrationStore(const JsonVariant json)
      : AnalogSwitchCalibrationStore(
            json["id"],
            json["mag_flux_at_farest"],
            json["mag_flux_at_nearest"]) {
  }

  inline void Reset() {
    mag_flux_at_farest_ = UINT16_MAX;
    mag_flux_at_nearest_ = 0;
    //    updated_ = true;
  }

  inline bool IsUpdated() const {
    return updated_;
  }

  inline void ClearUpdatedFlag() {
    updated_ = false;
  }

  inline int GetId() const {
    return id_;
  }

  inline double GetMagFluxAtNearest() const {
    return mag_flux_at_nearest_;
  }

  inline void SetMagFluxAtNearest(double v) {
    updated_ = true;
    mag_flux_at_nearest_ = v;
  }

  inline double GetMagFluxAtFarest() const {
    return mag_flux_at_farest_;
  }

  inline void SetMagFluxAtFarest(double v) {
    updated_ = true;
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

const String kCalibrationFilePath = "/user/hidden/calibration.json";

class CalibrationStore {
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
    // The value moves between 6mT ~ 60mT on the prototype board.
    // Initialize calibration store with the range but with margin so that we
    // can calibrate the data at runtime.
    const double kFarest_mT = 10.0;
    const double kNearest_mT = 40.0;
    return &analog_switches_
                .insert(std::make_pair(
                    id,
                    AnalogSwitchCalibrationStore(id, kFarest_mT, kNearest_mT)))
                .first->second;
  }

  void Reset() {
    for (auto& it : analog_switches_) {
      it.second.Reset();
    }
  }

  inline bool IsUpdated() const {
    for (const auto& it : analog_switches_) {
      if (it.second.IsUpdated()) {
        return true;
      }
    }
    return false;
  }

  inline void ClearUpdatedFlag() {
    for (auto& it : analog_switches_) {
      it.second.ClearUpdatedFlag();
    }
  }
};

#endif
