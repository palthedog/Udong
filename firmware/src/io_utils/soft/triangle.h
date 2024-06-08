#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <Arduino.h>

#include "../io.h"

// For testing
class TriangleInput : public AnalogInput {
 public:
  virtual ~TriangleInput() override {
  }

  // Returns [0, 65536)
  // Increases the number from 0 to 1023 over 1 second then
  // Decreasess the number from 1023 to 0 over 1 second then
  virtual uint16_t Read() override {
    const uint32_t kSecondMask = (1 << 20) - 1;
    const uint32_t kRisingMask = (1 << 20);
    uint32_t t = time_us_32();
    if (kRisingMask & t) {
      // rising
      // 20 bits to 16 bits
      return (t & kSecondMask) >> 4;
    } else {
      // setting
      // 20 bits to 16 bits
      return ((1 << 16) - 1) - ((t & kSecondMask) >> 4);
    }
    return t & kSecondMask;
  }
};

#endif