#ifndef LED_H
#define LED_H

#include <Arduino.h>

#include <cstdint>

#include "io.h"

class Led {
  enum LedMode {
    kToggle,
    kBlink,
    //    kPwm,
  };

  enum PinState {
    kLow,
    kHigh,
  };

  AnalogOutput* output_;
  LedMode mode_;
  PinState current_pin_state_;

  // Timestamps based on time_us_32()
  uint32_t blink_period_in_us_;
  uint32_t last_blink_time_in_us_;

 public:
  Led(AnalogOutput* output)
      : output_(output),
        mode_(kToggle),
        current_pin_state_(kLow),
        blink_period_in_us_(0),
        last_blink_time_in_us_(0) {
  }

  void TurnOn() {
    mode_ = kToggle;
    output_->Write(65535);
  }

  void TurnOff() {
    mode_ = kToggle;
    output_->Write(0);
  }

  void StartBlink(uint32_t blink_perio_ms) {
    mode_ = kBlink;
    output_->Write(0);

    last_blink_time_in_us_ = time_us_32();
    blink_period_in_us_ = blink_perio_ms * 1000;
  }

  void Step() {
    switch (mode_) {
      case kToggle:
        break;
      case kBlink:
        StepBlink();
        break;
    }
  }

 private:
  void StepBlink() {
    uint32_t now_us_32 = time_us_32();
    uint32_t delta_us = now_us_32 - last_blink_time_in_us_;
    if (delta_us < blink_period_in_us_) {
      return;
    }
    last_blink_time_in_us_ = now_us_32;

    if (current_pin_state_ == kLow) {
      current_pin_state_ = kHigh;
      output_->Write(HIGH);
    } else {
      current_pin_state_ = kLow;
      output_->Write(LOW);
    }
  }
};

#endif
