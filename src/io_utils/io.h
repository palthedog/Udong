#ifndef PIN_H
#define PIN_H

#include <Arduino.h>

#include <cstdint>

class Input {
 public:
  virtual ~Input() {
  }

  virtual PinStatus DigitalRead() = 0;
  virtual int AnalogRead() = 0;
};

class InputPin : Input {
  uint8_t pin_id_;

 public:
  InputPin(uint8_t pin_id) : pin_id_(pin_id) {
    pinMode(pin_id_, INPUT);
  }

  virtual ~InputPin() override {
  }

  virtual PinStatus DigitalRead() override {
    return digitalRead(pin_id_);
  };

  virtual int AnalogRead() override {
    return analogRead(pin_id_);
  };
};

// For testing
class TictocInput : Input {
 public:
  virtual ~TictocInput() override {
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
    const uint kTwoSecond = 2 * 1024 * 1024;
    const uint kHalf = kTwoSecond / 2;
    const uint kMask = kTwoSecond - 1;
    uint t = time_us_32() & kMask;
    if (t <= kHalf) {
      // rising
      return map(t, 0, kHalf, 0, 1023);
    } else {
      // setting
      return map(t, kHalf + 1, kMask, 1023, 0);
    }
  };
};

class Output {
 public:
  virtual ~Output() {
  }

  void DigitalWrite(uint8_t value) {
    if (value) {
      DigitalWrite(HIGH);
    } else {
      DigitalWrite(LOW);
    }
  };

  virtual void DigitalWrite(PinStatus status) = 0;
  virtual void AnalogWrite(int value) = 0;
};

class OutputPin : Output {
  uint8_t pin_id_;

 public:
  OutputPin(uint8_t pin_id) : pin_id_(pin_id) {
    pinMode(pin_id_, OUTPUT);
  }

  virtual ~OutputPin() override {
  }

  virtual void DigitalWrite(PinStatus status) override {
    digitalWrite(pin_id_, status);
  };

  virtual void AnalogWrite(int value) override {
    analogWrite(pin_id_, value);
  };
};

#endif
