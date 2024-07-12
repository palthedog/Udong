#ifndef UDONG_PROTOTYPE1_H
#define UDONG_PROTOTYPE1_H

#include <Arduino.h>

#include "board.h"
#include "io_utils/io.h"
#include "io_utils/multiplexer.h"
#include "switch/analog_switch.h"
#include "switch/digital_switch.h"

class UdongPrototype1 : public Board {
  Multiplexer8 mux0_;
  Multiplexer8 mux1_;

  AnalogOutputPin led_pin_;

  std::vector<std::shared_ptr<AnalogInput>> analog_switch_raw_ins_;
  std::vector<std::shared_ptr<DigitalInput>> digital_switch_ins_;

 public:
  UdongPrototype1()
      : mux0_(
            std::make_shared<DigitalOutputPin>(D16),
            std::make_shared<DigitalOutputPin>(D17),
            std::make_shared<DigitalOutputPin>(D18),
            std::make_shared<AnalogInputPin>(A0)),
        mux1_(
            std::make_shared<DigitalOutputPin>(D19),
            std::make_shared<DigitalOutputPin>(D20),
            std::make_shared<DigitalOutputPin>(D21),
            std::make_shared<AnalogInputPin>(A1)),
        led_pin_(D25) {
    // First 8 switches are connected to mux0.
    for (int i = 0; i < 8; i++) {
      analog_switch_raw_ins_.push_back(mux0_.GetInput(i));
    }
    // Next 8 switches are connected to mux1.
    for (int i = 0; i < 8; i++) {
      analog_switch_raw_ins_.push_back(mux1_.GetInput(i));
    }

    digital_switch_ins_.push_back(std::make_shared<DigitalInputPin>(D2, true));
    digital_switch_ins_.push_back(std::make_shared<DigitalInputPin>(D3, true));

    // TODO: Configure unused pins as input pulldown.
    //       All unused GPIOs are connected to the ground on Udong Board.
  }

  virtual ~UdongPrototype1() = default;

  virtual std::vector<std::shared_ptr<AnalogInput>>& GetAnalogInputs() {
    return analog_switch_raw_ins_;
  }

  virtual std::vector<std::shared_ptr<DigitalInput>>& GetDigitalInputs() {
    return digital_switch_ins_;
  }

  virtual AnalogOutput& GetPowerLed() {
    return led_pin_;
  }
};

#endif
