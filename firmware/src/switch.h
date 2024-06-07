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
  int last_analog_;

  // array[0] : B @ far_mm + 0.0mm (bottom)
  // array[1] : B @ far_mm + 0.1mm
  // array[2] : B @ far_mm + 0.2mm
  //  ...
  // array[39]: B @ far_mm + 3.9mm
  // array[40]: B @ far_mm + 4.0mm
  double mag_flux_table_[41] = {0.0};

  Input* input_;

  void UpdateMagFluxTable(double d_far_mm, double Br, double R_2, double T) {
    for (int i = 0; i <= 40; i++) {
      double press_mm = i * 0.1;
      double d = d_far_mm - press_mm;
      double mag_flux = CalcMagFlux(d, Br, R_2, T);
      mag_flux_table_[i] = mag_flux;

      /*
      if (i % 10 == 0) {
        Serial.printf("B@%.2lf: %.2lf\n", press_mm, mag_flux);
      }
      */
    }
  }

  double LookupPressMmFromMagFlux(double mag_flux) {
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

  // analog: [0, 4096)
  double AnalogToMagnetFlux(int analog) {
    double mv = (double)analog * (3300.0 / 4096.0);
    double delta_mV = mv - kQuiescentVoltage_mV;
    delta_mV = max(0.0, delta_mV);

    return delta_mV / kSensitivity_mV_per_mT;
  }

  double MagnetFluxToMilliVolt(double mag_flux) {
    return kSensitivity_mV_per_mT * mag_flux + kQuiescentVoltage_mV;
  }

  double ReadMagnetFlux() {
    // [0, 4096)
    last_analog_ = input_->AnalogRead();
    return AnalogToMagnetFlux(last_analog_);
  }

 public:
  AnalogSwitch(Input* input) : input_(input) {
    mag_flux_min_ = 100000;
    mag_flux_max_ = 0;
    last_press_mm_ = 0.0;
  }

  ~AnalogSwitch() {
  }

  void DumpLookupTable() {
    Serial.println("Dump lookup table");
    for (int mm = 0; mm < 4; mm++) {
      for (int micro = 0; micro < 5; micro++) {
        int i = mm * 10 + micro;
        double press_mm = i * 0.1;
        double mag_flux = mag_flux_table_[i];
        double mv = MagnetFluxToMilliVolt(mag_flux);
        Serial.printf(
            "  %.2lf mm: %.2lf mT, %.2lf mV\n", press_mm, mag_flux, mv);
      }
    }
    double mag_flux = mag_flux_table_[40];
    double mv = MagnetFluxToMilliVolt(mag_flux);
    Serial.printf("  %.2lf mm: %.2lf mT, %.2lf mV\n", 4.0, mag_flux, mv);
  }

  void DumpLastState() {
    Serial.printf(
        "press: %.2lf mm, "
        "raw: %d (%.lf mV), "
        "mag: %.2lf mT, min: %.2lf mt, "
        "max: %.2lf mT\n",
        last_press_mm_,
        last_analog_,
        last_analog_ * (3300.0 / 4096.0),
        last_mag_flux_,
        mag_flux_min_,
        mag_flux_max_);
  }

  bool IsOn() {
    last_mag_flux_ = ReadMagnetFlux();
    if (last_mag_flux_ < mag_flux_min_) {
      Serial.printf(
          "min_flux: %.2lf -> %.2lf\n", mag_flux_min_, last_mag_flux_);
    }

    mag_flux_min_ = std::min(mag_flux_min_, last_mag_flux_);
    mag_flux_max_ = std::max(mag_flux_max_, last_mag_flux_);

    /*
      TODO:
        Now we can calibrate all parameters let calculate distance next
    */
    // return current_mag_flux > (mag_flux_min_ + mag_flux_max_) / 2;

    last_press_mm_ = LookupPressMmFromMagFlux(last_mag_flux_);
    return last_press_mm_ > 2.0;
  }

  double GetLastPressMm() {
    return last_press_mm_;
  }

  double GetLastMagFlux() {
    return last_mag_flux_;
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
        AnalogToMagnetFlux((1 << 12) - 1));
    Serial.printf("Solved d_near: %.3lf mm\n", solved_d_near_mm);
    double mag_remanence = CalcRemanence(
        solved_d_near_mm, mag_flux_max_, kR_2, kMagnetThickness_mm);

    UpdateMagFluxTable(
        // d_far_mm
        solved_d_near_mm + kKeyStroke_mm,
        mag_remanence,
        kR_2,
        kMagnetThickness_mm);
  }
};

#endif
