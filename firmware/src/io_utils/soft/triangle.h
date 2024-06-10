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
    // 20 bits to 16 bits
    uint16_t value = ((t & kSecondMask) >> 4);
    if (kRisingMask & t) {
      // rising
      return value;
    }
    // falling
    // 20 bits to 16 bits
    return UINT16_MAX - value;
  }
};

#endif