#ifndef SWITCH_H
#define SWITCH_H

#include <cmath>

#include "io_utils/io.h"
#include "solver.h"

class AnalogSwitch {
  // For calibration
  uint32_t mag_flux_min_;
  uint32_t mag_flux_max_;

  Input* input_;

  uint32_t ReadMagnetFlux() {
    const double kSensitivity_mV_per_mT = 30.0;
    const double kQuiescentVoltage_mV = 0.6 * 1000;

    // [0, 4096)
    int adc_value = input_->AnalogRead();
    double milli_volt = adc_value * 3300.0 / 4096.0;
    double delta_mV = milli_volt - kQuiescentVoltage_mV;
    delta_mV = max(0.0, delta_mV);
    return delta_mV * kSensitivity_mV_per_mT;
  }

 public:
  AnalogSwitch(Input* input) : input_(input) {
    mag_flux_min_ = UINT32_MAX;
    mag_flux_max_ = 0;
  }

  ~AnalogSwitch() {
  }

  bool IsOn() {
    uint32_t value = ReadMagnetFlux();
    mag_flux_min_ = std::min(mag_flux_min_, value);
    mag_flux_max_ = std::max(mag_flux_max_, value);

    return value > (mag_flux_min_ + mag_flux_max_) / 2;
  }

  void Calibrate() {
    const double kKeyStrokeMicro = 4.0 * 1000;
    const double kMagnetRadiusMicro = 2.9 * 1000 / 2.0;
    const double kMagnetThicknessMicro = 3.4 * 1000;

    const double distance_range_min_millis = 0.5 * 1000.0;
    const double distance_range_max_millis = 5.0 * 1000.0;
    const double eps_micros = 0.01 * 1000;
    double solved_d_near = solve(
        kKeyStrokeMicro,
        kMagnetRadiusMicro * kMagnetRadiusMicro,
        kMagnetThicknessMicro,
        mag_flux_min_,
        mag_flux_max_,
        distance_range_min_millis,
        distance_range_max_millis,
        eps_micros);
    Serial.printf("b_near: %d, b_far: %d\n", mag_flux_max_, mag_flux_min_);
    Serial.printf("Solved d_near: %.2lf mm\n", solved_d_near / 1000.0);
  }
};

#endif
