#ifndef SOLVER_H
#define SOLVER_H

#include <Arduino.h>

#include <cmath>
#include <functional>

// let
//   term0 = (D + T) / sqrt(R^2 + (D + T)^2)
//   term1 = D / sqrt(R^2 + D^2)
// B = (Br / 2) * (term0 - term1)
inline double mag_flux_2(double d, double b, double R_2, double T) {
  double term0 = (d + T) / std::sqrt(R_2 + (d + T) * (d + T));
  double term1 = d / std::sqrt(R_2 + d * d);
  return b / (term0 - term1);
}

// solve d_near for a range of [d_min, d_max]
inline double solve(
    // known (e.g. 4mm)
    double stroke,
    // known
    double R_2,
    // known
    double T,
    // known(measured)
    double b_far,
    // known(measured)
    double b_near,
    // range_min
    double d_min,
    // range_max
    double d_max,
    double eps) {
  double d_near = (d_max + d_min) / 2.0;
  if (d_max - d_min < eps) {
    return d_near;
  }

  // let
  //   term0 = (D + T) / sqrt(R^2 + (D + T)^2)
  //   term1 = D / sqrt(R^2 + D^2)
  // B = (Br / 2) * (term0 - term1)
  // where
  //   B: magnetic flux at the point
  //   D: distance from the magnet to the point
  //   R: magnet's radius (C / 2)
  //   T: thickness
  //   Br: magnetic flux of the magnet (unknown but known as a constant value)
  // So
  //   Br / 2 = B / (term0 - term1)
  // We know:
  //    b_near (magnetic flux at the nearest point)
  //    b_far (magnetic flux at the farest point)
  //
  // Since Br is constant,
  // mag_flux(D, B, R, T)
  //
  // mag_flux(d_near, T, R) = mag_flux(d_far, T, R)
  // mag_flux(d_near, T, R) - mag_flux(d_far, T, R) = 0
  // where:
  //   d_far = d_near + stroke

  double mag_flux_near = mag_flux_2(d_near, b_near, R_2, T);
  double d_far = d_near + stroke;
  double mag_flux_far = mag_flux_2(d_far, b_far, R_2, T);

  // Serial.printf("d_near: %lf, range: [%lf, %lf]\n", d_near, d_min, d_max);
  // Serial.printf("B_near: %lf, B_far: %lf\n", mag_flux_near, mag_flux_far);
  if (mag_flux_near > mag_flux_far) {
    return solve(stroke, R_2, T, b_far, b_near, d_min, d_near, eps);
  } else {
    return solve(stroke, R_2, T, b_far, b_near, d_near, d_max, eps);
  }
  //*/
}

#endif
