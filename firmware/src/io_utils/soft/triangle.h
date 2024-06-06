#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <Arduino.h>

#include "../io.h"

// For testing
template <int MIN, int MAX>
class TriangleInput : public Input {
 public:
  virtual ~TriangleInput() override {
  }

  // Repeat LOW for 1 second, HIGH for 1 second
  virtual PinStatus DigitalRead() override {
    const int kOneSecond = 1024 * 1024;
    if (time_us_32() & kOneSecond == 0) {
      return LOW;
    } else {
      return HIGH;
    }
  };

  // Returns [0, 1023]
  // Increases the number from 0 to 1023 over 1 second then
  // Decreasess the number from 1023 to 0 over 1 second then
  virtual int AnalogRead() override {
    const uint32_t kTwoSecond = 2 * 1024 * 1024;
    const uint32_t kHalf = kTwoSecond / 2;
    const uint32_t kMask = kTwoSecond - 1;
    uint32_t t = time_us_32() & kMask;
    if (t <= kHalf) {
      // rising
      return map(t, 0, kHalf, MIN, MAX);
    } else {
      // setting
      return map(t, kHalf + 1, kMask, MAX, MIN);
    }
  };
};

#endif