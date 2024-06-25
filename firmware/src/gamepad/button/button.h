#ifndef BUTTON_H
#define BUTTON_H

#include "../gamepad.h"

class Button {
 public:
  virtual ~Button() {
  }

  virtual void UpdateGamepadReport(GamepadReport& gamepad_report) = 0;
};

#endif
