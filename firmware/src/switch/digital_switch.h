#ifndef DIGITAL_SWITCH_H
#define DIGITAL_SWITCH_H

#include <memory>

#include "io_utils/io.h"
#include "switch.h"

// DigitalSwitch represents a switch which has boolean state (on/off).
class DigitalSwitch : public Switch {
  std::shared_ptr<DigitalInput> input_;

 public:
  DigitalSwitch(std::shared_ptr<DigitalInput> input) : input_(input) {
  }

  bool IsOn() override {
    return input_->Read();
  }
};

#endif
