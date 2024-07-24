#ifndef D_PAD_H
#define D_PAD_H

#include <stdint.h>

#include <memory>

#include "../../switch/switch.h"
#include "button.h"
#include "proto/config.pb.h"

enum DPadLRState {
  LRNeutral,      // Nothing is presssed
  OnlyLeft,       // Only Left is pressed
  OnlyRight,      // Only Right is pressed
  LeftThenRight,  // Left pressed first then Right pressed later
  RightThenLeft,  // Right pressed first then Left pressed later
};

enum DPadUDState {
  UDNeutral,   // Nothing is presssed
  OnlyUp,      // Only Up is pressed
  OnlyDown,    // Only Down is pressed
  UpThenDown,  // Up pressed first then Down pressed later
  DownThenUp,  // Down pressed first then Down pressed later
};

class DPad {
  DPadConfig d_pad_config_;

  std::vector<std::shared_ptr<Switch>> left_switches_;
  std::vector<std::shared_ptr<Switch>> right_switches_;
  std::vector<std::shared_ptr<Switch>> up_switches_;
  std::vector<std::shared_ptr<Switch>> down_switches_;

  DPadLRState lr_state_ = LRNeutral;
  DPadUDState ud_state_ = UDNeutral;

  void UpdateLRStates();
  void UpdateUDStates();

 public:
  DPad() {
  }

  void SetConfig(const DPadConfig& config) {
    d_pad_config_ = config;
  }

  int LRIndex() {
    const int kLeft = 0;
    const int kCenter = 1;
    const int kRight = 2;
    int x = kCenter;
    switch (lr_state_) {
      case DPadLRState::LRNeutral:
        x = kCenter;
        break;
      case DPadLRState::OnlyLeft:
        x = kLeft;
        break;
      case DPadLRState::OnlyRight:
        x = kRight;
        break;
      case DPadLRState::LeftThenRight:
        switch (d_pad_config_.lr_socd_mode()) {
          case DPadConfig::LRSocdCleanerMode::LR_UNSPECIFIED:
          case DPadConfig::LRSocdCleanerMode::LR_NEUTRAL:
            x = kCenter;
            break;
          case DPadConfig::LRSocdCleanerMode::LR_LAST_PRIORITY:
            x = kRight;
            break;
        }
        break;
      case DPadLRState::RightThenLeft:
        switch (d_pad_config_.lr_socd_mode()) {
          case DPadConfig::LRSocdCleanerMode::LR_UNSPECIFIED:
          case DPadConfig::LRSocdCleanerMode::LR_NEUTRAL:
            x = kCenter;
            break;
          case DPadConfig::LRSocdCleanerMode::LR_LAST_PRIORITY:
            x = kLeft;
            break;
        }
        break;
    }
    return x;
  }

  int UDIndex() {
    const int kUp = 0;
    const int kCenter = 1;
    const int kDown = 2;
    int y = kCenter;
    switch (ud_state_) {
      case DPadUDState::UDNeutral:
        y = kCenter;
        break;
      case DPadUDState::OnlyUp:
        y = kUp;
        break;
      case DPadUDState::OnlyDown:
        y = kDown;
        break;
      case DPadUDState::UpThenDown:
        switch (d_pad_config_.ud_socd_mode()) {
          case DPadConfig::UDSocdCleanerMode::UD_UNSPECIFIED:
          case DPadConfig::UDSocdCleanerMode::UD_NEUTRAL:
            y = kCenter;
            break;
          case DPadConfig::UDSocdCleanerMode::UD_LAST_PRIORITY:
            y = kDown;
            break;
          case DPadConfig::UDSocdCleanerMode::UP_PRIORITY:
            y = kUp;
            break;
          case DPadConfig::UDSocdCleanerMode::DOWN_PRIORITY:
            y = kDown;
            break;
        }
        break;
      case DPadUDState::DownThenUp:
        switch (d_pad_config_.ud_socd_mode()) {
          case DPadConfig::UDSocdCleanerMode::UD_UNSPECIFIED:
          case DPadConfig::UDSocdCleanerMode::UD_NEUTRAL:
            y = kCenter;
            break;
          case DPadConfig::UDSocdCleanerMode::UD_LAST_PRIORITY:
            y = kUp;
            break;
          case DPadConfig::UDSocdCleanerMode::UP_PRIORITY:
            y = kUp;
            break;
          case DPadConfig::UDSocdCleanerMode::DOWN_PRIORITY:
            y = kDown;
            break;
        }
        break;
    }
    return y;
  }

  void UpdateGamepadReport(GamepadReport& gamepad_report) {
    UpdateLRStates();
    UpdateUDStates();

    int x = LRIndex();
    int y = UDIndex();

    const uint8_t kCenter = 0;
    const uint8_t kUp = 1;
    const uint8_t kUpRight = 2;
    const uint8_t kRight = 3;
    const uint8_t kDownRight = 4;
    const uint8_t kDown = 5;
    const uint8_t kDownLeft = 6;
    const uint8_t kLeft = 7;
    const uint8_t kUpLeft = 8;

    const u_int8_t kDpadTable[3][3] = {
        {kUpLeft, kUp, kUpRight},
        {kLeft, kCenter, kRight},
        {kDownLeft, kDown, kDownRight},
    };

    gamepad_report.d_pad = kDpadTable[y][x];
  }

  void AddLeftSwitch(std::shared_ptr<Switch> left_switch) {
    left_switches_.push_back(left_switch);
  }

  void AddRightSwitch(std::shared_ptr<Switch> right_switch) {
    right_switches_.push_back(right_switch);
  }

  void AddUpSwitch(std::shared_ptr<Switch> up_switch) {
    up_switches_.push_back(up_switch);
  }

  void AddDownSwitch(std::shared_ptr<Switch> down_switch) {
    down_switches_.push_back(down_switch);
  }

  void Clear() {
    left_switches_.clear();
    right_switches_.clear();
    up_switches_.clear();
    down_switches_.clear();
    d_pad_config_ = DPadConfig();
  }

 private:
};

#endif
