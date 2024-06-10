#ifndef THROTTLING_H
#define THROTTLING_H

#include <Arduino.h>
#include <inttypes.h>

#include <functional>

class Throttling {
  uint32_t last_t_;
  uint32_t period_;
  std::function<void()> func_;

 public:
  Throttling(uint32_t period_ms, std::function<void()> func)
      : period_(period_ms * 1000), func_(func) {
    last_t_ = time_us_32();
  }

  void MaybeRun(uint32_t now_us) {
    if (now_us - last_t_ > period_) {
      func_();
      last_t_ = now_us;
    }
  }
};

#endif
