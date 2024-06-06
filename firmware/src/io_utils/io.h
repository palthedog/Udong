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

class InputPin : public Input {
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

class Output {
 public:
  Output() {
  }

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

class OutputPin : public Output {
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
