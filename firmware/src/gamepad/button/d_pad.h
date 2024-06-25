#ifndef D_PAD_H
#define D_PAD_H

#include <stdint.h>

#include <memory>

#include "../../switch/switch.h"
#include "button.h"

enum DPadUDState {
  UDNeutral,   // Nothing is presssed
  OnlyUp,      // Only Up is pressed
  OnlyDown,    // Only Down is pressed
  UpThenDown,  // Up pressed first then Down pressed later
  DownThenUp,  // Down pressed first then Down pressed later
};

enum DPadLRState {
  LRNeutral,      // Nothing is presssed
  OnlyLeft,       // Only Left is pressed
  OnlyRight,      // Only Right is pressed
  LeftThenRight,  // Left pressed first then Right pressed later
  RightThenLeft,  // Right pressed first then Left pressed later
};

class DPad {
  std::vector<std::shared_ptr<Switch>> up_switches_;
  std::vector<std::shared_ptr<Switch>> down_switches_;
  std::vector<std::shared_ptr<Switch>> left_switches_;
  std::vector<std::shared_ptr<Switch>> right_switches_;

  DPadUDState ud_state_ = UDNeutral;
  DPadLRState lr_state_ = LRNeutral;

  void UpdateUDStates();
  void UpdateLRStates();

 public:
  DPad() {
  }

  void UpdateGamepadReport(GamepadReport& gamepad_report) {
    UpdateUDStates();
    UpdateLRStates();

    int x = 1;
    switch (lr_state_) {
      case DPadLRState::LRNeutral:
        x = 1;
        break;
      case DPadLRState::OnlyLeft:
        x = 0;
        break;
      case DPadLRState::OnlyRight:
        x = 2;
        break;
      case DPadLRState::LeftThenRight:
      case DPadLRState::RightThenLeft:
        // TODO: Make it configurabale.
        x = 1;
        break;
    }

    int y = 1;
    switch (ud_state_) {
      case DPadUDState::UDNeutral:
        y = 1;
        break;
      case DPadUDState::OnlyUp:
        y = 0;
        break;
      case DPadUDState::OnlyDown:
        y = 2;
        break;
      case DPadUDState::UpThenDown:
      case DPadUDState::DownThenUp:
        // TODO: Make it configurabale.
        y = 1;
        break;
    }

    const u_int8_t kDpadTable[3][3] = {
        {GAMEPAD_HAT_UP_LEFT, GAMEPAD_HAT_UP, GAMEPAD_HAT_UP_RIGHT},
        {GAMEPAD_HAT_LEFT, GAMEPAD_HAT_CENTERED, GAMEPAD_HAT_RIGHT},
        {GAMEPAD_HAT_DOWN_LEFT, GAMEPAD_HAT_DOWN, GAMEPAD_HAT_DOWN_RIGHT},
    };
    gamepad_report.d_pad = kDpadTable[y][x];
  }

  void AddUpSwitch(std::shared_ptr<Switch> up_switch) {
    up_switches_.push_back(up_switch);
  }

  void AddDownSwitch(std::shared_ptr<Switch> down_switch) {
    down_switches_.push_back(down_switch);
  }

  void AddLeftSwitch(std::shared_ptr<Switch> left_switch) {
    left_switches_.push_back(left_switch);
  }

  void AddRightSwitch(std::shared_ptr<Switch> right_switch) {
    right_switches_.push_back(right_switch);
  }

  void Clear() {
    up_switches_.clear();
    down_switches_.clear();
    left_switches_.clear();
    right_switches_.clear();
  }

 private:
};

#endif
