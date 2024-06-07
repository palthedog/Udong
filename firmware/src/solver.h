#ifndef SOLVER_H
#define SOLVER_H

#include <Arduino.h>

#include <cmath>
#include <functional>

template <typename T>
class BSearch {
  const std::function<T(T)> func_;

 public:
  template <class F>
  BSearch(F func) : func_(func) {
  }

  T Solve(T y_target, T x_min, T x_max, const T kEpsX) {
    bool increasing = func_(x_max) > func_(x_min);

    while (x_max - x_min > kEpsX) {
      T x = (x_max + x_min) / (T)2;
      T y = func_(x);
      if (y < y_target) {
        if (increasing) {
          x_min = x;
        } else {
          x_max = x;
        }
      } else {
        // y >= target_y
        if (increasing) {
          x_max = x;
        } else {
          x_min = x;
        }
      }
    }
    return (x_max + x_min) / (T)2;
  }
};

#endif
