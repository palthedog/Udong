#ifndef SWITCH_H
#define SWITCH_H

#include <cmath>

#include "io_utils/io.h"
#include "solver.h"

// let
//   term0 = (D + T) / sqrt(R^2 + (D + T)^2)
//   term1 = D / sqrt(R^2 + D^2)
// B = (Br / 2) * (term0 - term1)
// where
//   B: magnetic flux at the point
//   D: distance from the magnet to the point
//   R: magnet's radius (C / 2)
//   T: thickness
//   Br: Remanence of the magnet (unknown but known as a constant value)

// B = (Br / 2) * (term0 - term1)
//
// Can be converted into
//   Br / 2 = B / (term0 - term1)
//
// Let
//   remanence_2(D, B, C, T) = B / (term0 - term1)
// Since Br is constant value for same magnet
//   remanence_2(d_near, B_near, C, T) = mag_flux(d_far, B_near, C, T)
//
// Because
//   d_far = d_near + K
// where
//   K: the depth of key stroke
// remanence_2(d_near, B_near, C, T) = remanence_2(d_near + K, B_near, C, T)
//
// Now we can solve d_near with a function f(d):
//   f(d) = remanence_2(d, B_near, C, T) - remanence_2(d + K, B_near, C, T)

inline double CalcTerm0(double d, double R_2, double T) {
  return (d + T) / std::sqrt(R_2 + (d + T) * (d + T));
}

inline double CalcTerm1(double d, double R_2) {
  return d / std::sqrt(R_2 + d * d);
}

// Br / 2
inline double CalcRemanence_2(double d, double b, double R_2, double T) {
  double term0 = CalcTerm0(d, R_2, T);
  double term1 = CalcTerm1(d, R_2);
  return b / (term0 - term1);
}

// Br / 2
inline double CalcRemanence(double d, double b, double R_2, double T) {
  return 2.0 * CalcRemanence_2(d, b, R_2, T);
}

// let
//   term0 = (D + T) / sqrt(R^2 + (D + T)^2)
//   term1 = D / sqrt(R^2 + D^2)
// B = (Br / 2) * (term0 - term1)
inline double CalcMagFlux(double d, double Br, double R_2, double T) {
  double term0 = CalcTerm0(d, R_2, T);
  double term1 = CalcTerm1(d, R_2);
  return (Br / 2.0) * (term0 - term1);
}

class AnalogSwitch {
  // For calibration
  double mag_flux_min_;
  double mag_flux_max_;

  double last_mag_flux_;
  double last_press_mm_;
  uint16_t last_analog_;

  // array[0] : B @ far_mm + 0.0mm (bottom)
  // array[1] : B @ far_mm + 0.1mm
  // array[2] : B @ far_mm + 0.2mm
  //  ...
  // array[39]: B @ far_mm + 3.9mm
  // array[40]: B @ far_mm + 4.0mm
  double mag_flux_table_[41] = {0.0};

  int id_;
  AnalogInput* input_;

  void UpdateMagFluxTable(double d_far_mm, double Br, double R_2, double T) {
    for (int i = 0; i <= 40; i++) {
      double press_mm = i * 0.1;
      double d = d_far_mm - press_mm;
      double mag_flux = CalcMagFlux(d, Br, R_2, T);
      mag_flux_table_[i] = mag_flux;
    }
  }

  double LookupPressMmFromMagFlux(double mag_flux) {
    // TODO:
    //  - bsearch
    //  - Linear Interpolation
    //  - Instead of creating a mag_flux -> mm table
    //    we can create analog-value -> mm table?
    for (int i = 0; i <= 40; i++) {
      double mag_flux_at = mag_flux_table_[i];
      if (mag_flux_at >= mag_flux) {
        return i * 0.1;
      }
    }
    return 4.0;
  }

  const double kSensitivity_mV_per_mT = 30.0;
  const double kQuiescentVoltage_mV = 0.6 * 1000;

  // analog: [0, 65536)
  double AnalogToMagnetFlux(uint16_t analog) {
    double mv = analog * (3300.0 / 65536.0);
    double delta_mV = mv - kQuiescentVoltage_mV;
    if (delta_mV <= 0.0) {
      return 0.0;
    }
    return delta_mV / kSensitivity_mV_per_mT;
  }

  double MagnetFluxToMilliVolt(double mag_flux) {
    return kSensitivity_mV_per_mT * mag_flux + kQuiescentVoltage_mV;
  }

  double ReadMagnetFlux() {
    // [0, 65536)
    last_analog_ = input_->Read();
    return AnalogToMagnetFlux(last_analog_);
  }

 public:
  AnalogSwitch(int id, AnalogInput* input) : id_(id), input_(input) {
    mag_flux_min_ = INT16_MAX;
    mag_flux_max_ = 0;
    last_press_mm_ = 0.0;
  }

  ~AnalogSwitch() {
  }

  bool IsOn() {
    last_mag_flux_ = ReadMagnetFlux();

    // Record the maximum value for later calibration.
    // It may take noisy inputs a lot but it might be OK since mag-flux is
    // poropotional to the squre of the magnet distance.
    // (e.g. 1200mV at 3.9mm, 1240mV at 4.0mm)
    // Thus we don't care about a few mV errors for the nearest side.
    // On the other hand, we must calibrate the zero-point(mag_flux_min_)
    // because the mag-flux at the farest point is much much lower (e.g. 700mV
    // at 0.0mm, 705mV at 0.1mm).
    mag_flux_max_ = std::max(mag_flux_max_, last_mag_flux_);

    last_press_mm_ = LookupPressMmFromMagFlux(last_mag_flux_);

#if TELEPLOT
    Serial.printf(">asw%d-mV:%lf\n", id_, last_analog_ / 65536.0 * 3300.0);
    Serial.printf(">asw%d-mm:%lf\n", id_, last_press_mm_);
#endif
    return last_press_mm_ > 2.0;
  }

  double GetLastPressMm() {
    return last_press_mm_;
  }

  double GetLastMagFlux() {
    return last_mag_flux_;
  }

  // Calibrate the mag-flux value at the farest point.
  // It should be called when the switch is NOT pressed.
  void CalibrateZeroPoint() {
    uint32_t sum = 0;
    int count = 0;
    uint32_t start_t = time_us_32();
    // TODO: Run all switchs' calibrations parallery so that we can collect
    // data that is spread out over longer time.
    // 100 ms
    while (time_us_32() - start_t < 100 * 1000) {
      sum += input_->Read();
      count++;
    }
    mag_flux_min_ = AnalogToMagnetFlux(sum / count);
  }

  void Calibrate() {
    // known constants
    const double kKeyStroke_mm = 4.0;
    const double kMagnetRadius_mm = 2.9 / 2.0;
    const double kR_2 = kMagnetRadius_mm * kMagnetRadius_mm;
    const double kMagnetThickness_mm = 3.4;

    // Solving for D (distance)
    const double kDistanceRangeMin_mm = 0.5;
    const double kDistanceRangeMax_mm = 5.0;
    const double kEps_mm = 0.01;

    BSearch<double> bsearch([=](double x) {
      double d_near = x;
      double d_far = d_near + kKeyStroke_mm;

      double mag_flux_near =
          CalcRemanence_2(d_near, mag_flux_max_, kR_2, kMagnetThickness_mm);
      double mag_flux_far =
          CalcRemanence_2(d_far, mag_flux_min_, kR_2, kMagnetThickness_mm);
      return mag_flux_near - mag_flux_far;
    });
    double solved_d_near_mm =
        bsearch.Solve(0.0, kDistanceRangeMin_mm, kDistanceRangeMax_mm, kEps_mm);

    Serial.printf(
        "b_near: %.3lf mT, b_far: %.3lf mT\n", mag_flux_max_, mag_flux_min_);
    Serial.printf(
        "    note: b_MAX: %.3lf mT for this sensor\n",
        AnalogToMagnetFlux(UINT16_MAX));
    Serial.printf("Solved d_near: %.3lf mm\n", solved_d_near_mm);

    double mag_remanence = CalcRemanence(
        solved_d_near_mm, mag_flux_max_, kR_2, kMagnetThickness_mm);
    UpdateMagFluxTable(
        solved_d_near_mm + kKeyStroke_mm,
        mag_remanence,
        kR_2,
        kMagnetThickness_mm);
  }

  void DumpLookupTable() {
    Serial.println("Dump lookup table");
    const int kDumpList[] = {
        0,
        1,
        2,
        3,
        4,
        10,
        11,
        20,
        21,
        39,
        40,
    };

    for (int i : kDumpList) {
      double press_mm = i * 0.1;
      double mag_flux = mag_flux_table_[i];
      double mv = MagnetFluxToMilliVolt(mag_flux);
      Serial.printf("  %.2lf mm: %.2lf mT, %.2lf mV\n", press_mm, mag_flux, mv);
    }
  }

  void DumpLastState() {
    Serial.printf(
        "press: %.2lf mm, "
        "raw: %d (%.lf mV), "
        "mag: %.2lf mT, "
        "min: %.2lf mT, "
        "max: %.2lf mT\n",
        last_press_mm_,
        last_analog_,
        last_analog_ * (3300.0 / 65536.0),
        last_mag_flux_,
        mag_flux_min_,
        mag_flux_max_);
  }
};

#endif
