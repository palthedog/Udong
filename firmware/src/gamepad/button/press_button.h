#ifndef PRESS_BUTTON_H
#define PRESS_BUTTON_H

#include <stdint.h>

#include <memory>

#include "../../config/button_config.h"
#include "../../switch/analog_switch.h"
#include "button.h"

class PressButton : public Button {
  std::shared_ptr<AnalogSwitch> analog_switch_;
  uint8_t press_button_id_;

 public:
  PressButton(
      std::shared_ptr<AnalogSwitch> analog_switch, uint8_t press_button_id)
      : analog_switch_(analog_switch), press_button_id_(press_button_id) {
  }

  void UpdateGamepadReport(GamepadReport& gamepad_report) override {
    if (analog_switch_->IsOn()) {
      gamepad_report.PressButton(press_button_id_);
    }
  }
};

#endif
