#ifndef HALL_H
#define HALL_H

#include <Arduino.h>

#include "../io.h"

/// @brief
/// @tparam kDistMinMicro closest magnet position in micro meters
/// @tparam kDistMaxMicro farest magnet position in micro meters
/// @tparam kBr magnet strength in Gauss
template <
    uint32_t kAnalogMin,
    uint32_t kAnalogMax,
    uint32_t kDistMinMicro,
    uint32_t kDistMaxMicro,
    uint32_t kBr,
    uint32_t kThicknessMicro,
    uint32_t kDiameterMicro,
    uint32_t kSensitivity_mV_mT,
    uint32_t kVcc_mV,
    uint32_t kQuiescentVoltage_mV>
class HallInput : public AnalogInput {
 public:
  HallInput() {
  }

  virtual uint16_t Read() override {
    uint32_t dist_micro = GetDistanceMicros();
    uint32_t micro_tesla = DistanceMicrosToMicroTesla(dist_micro);
    uint32_t milli_volt =
        kQuiescentVoltage_mV + micro_tesla * kSensitivity_mV_mT / 1000;
    // Analog-Digital convertion
    if (milli_volt > kVcc_mV) {
      milli_volt = kVcc_mV;
    }
    double per_mv = kAnalogMax / (double)kVcc_mV;
    return per_mv * milli_volt;
  }

  uint32_t DistanceMicrosToMicroTesla(uint32_t distance_micros) {
    // tmp0 = (D + T) / sqrt((0.5C)^2 + (D + T)^2)
    // tmp1 = D / sqrt((0.5C)^2 + D^2)
    // B = (Br / 2) * (tmp0 - tmp1)
    const uint32_t D = distance_micros;
    const uint32_t T = kThicknessMicro;
    const uint32_t C = kDiameterMicro;
    const uint32_t D_T = D + T;
    const uint32_t D_T_2 = D_T * D_T;
    const uint32_t C_2_2 = (C / 2) * (C / 2);
    const uint32_t D_2 = D * D;
    const uint32_t Br_2 = 100 * kBr / 2;
    /*
    Serial.printf("D_T_2: %d\n", D_T_2);
    Serial.printf("C_2_2: %d\n", C_2_2);
    Serial.printf("D_T_2 + C_2_2: %d\n", D_T_2 + C_2_2);
    Serial.printf("sqrtl(D_T_2 + C_2_2): %lf\n", sqrtl(C_2_2 + D_T_2));
    Serial.printf("sqrtl(C_2_2 + D_2): %lf\n", sqrtl(C_2_2 + D_2));
    */
    double tmp0 = D_T / sqrtl(C_2_2 + D_T_2);
    double tmp1 = D / sqrtl(C_2_2 + D_2);
    /*
    Serial.printf("tmp0: %lf\n", tmp0);
    Serial.printf("tmp1: %lf\n", tmp1);
    */
    return Br_2 * (tmp0 - tmp1);
  }

  // Returns [kDistMinMicro, uint32_t kDistMaxMicro]
  uint32_t GetDistanceMicros() {
    uint32_t t = millis() % 2000;
    if (t < 1000) {
      // rising
      return kDistMinMicro + t * (kDistMaxMicro - kDistMinMicro) / 1000;
    } else {
      // setting
      return kDistMinMicro +
             (2000 - t) * (kDistMaxMicro - kDistMinMicro) / 1000;
    }
  };
};

#endif
