#ifndef PIN_H
#define PIN_H

#include <Arduino.h>

#include <cstdint>

template <typename T>
class Input {
 public:
  virtual ~Input() {
  }

  virtual T Read() = 0;
};

typedef Input<uint16_t> AnalogInput;
typedef Input<bool> DigitalInput;

class AnalogInputPin : public AnalogInput {
  uint8_t pin_id_;

 public:
  AnalogInputPin(uint8_t pin_id) : pin_id_(pin_id) {
    pinMode(pin_id_, INPUT);
  }

  virtual ~AnalogInputPin() override {
  }

  virtual uint16_t Read() override {
    // convert 12bits analog input into uint16_t
    return ((uint16_t)analogRead(pin_id_)) << 4;
  };
};

class DigitalInputPin : public DigitalInput {
  uint8_t pin_id_;
  bool pull_up_;

 public:
  DigitalInputPin(uint8_t pin_id, bool pull_up)
      : pin_id_(pin_id), pull_up_(pull_up) {
    pinMode(pin_id_, pull_up ? INPUT_PULLUP : INPUT_PULLDOWN);
  }

  virtual ~DigitalInputPin() override {
  }

  virtual bool Read() override {
    if (pull_up_) {
      return digitalRead(pin_id_) == LOW;
    }
    return digitalRead(pin_id_) == HIGH;
  };
};

template <typename T>
class Output {
 public:
  Output() {
  }

  virtual ~Output() {
  }

  /// virtual void DigitalWrite(PinStatus status) = 0;
  virtual void Write(T value) = 0;
};

typedef Output<uint16_t> AnalogOutput;
typedef Output<PinStatus> DigitalOutput;

class AnalogOutputPin : public AnalogOutput {
  uint8_t pin_id_;

 public:
  AnalogOutputPin(uint8_t pin_id) : pin_id_(pin_id) {
    pinMode(pin_id_, OUTPUT);
  }

  virtual ~AnalogOutputPin() override {
  }

  virtual void Write(uint16_t value) override {
    // convert uint16_t into 8bits
    // NOTE: analogWrite always takes 0-255
    analogWrite(pin_id_, value >> 8);
  };
};

class DigitalOutputPin : public DigitalOutput {
  uint8_t pin_id_;

 public:
  DigitalOutputPin(uint8_t pin_id) : pin_id_(pin_id) {
    pinMode(pin_id_, OUTPUT);
  }

  virtual ~DigitalOutputPin() override {
  }

  virtual void Write(PinStatus value) override {
    digitalWrite(pin_id_, value);
  };
};

template <uint16_t kValue>
class AnalogConstantInput : public AnalogInput {
 public:
  AnalogConstantInput() {
  }

  virtual ~AnalogConstantInput() override {
  }

  virtual uint16_t Read() override {
    return kValue;
  };
};

typedef AnalogConstantInput<0> AnalogGroundInput;

#endif
