#include "d_pad.h"

using namespace std;

void DPad::UpdateUDStates() {
  bool up_pressed = false;
  for (auto& up_switch : up_switches_) {
    if (up_switch->IsOn()) {
      up_pressed = true;
      break;
    }
  }

  bool down_pressed = false;
  for (auto& down_switch : down_switches_) {
    if (down_switch->IsOn()) {
      down_pressed = true;
      break;
    }
  }

  // One or zero button is pressed.
  if (up_pressed && !down_pressed) {
    ud_state_ = OnlyUp;
    return;
  } else if (!up_pressed && down_pressed) {
    ud_state_ = OnlyDown;
    return;
  } else if (!up_pressed && !down_pressed) {
    ud_state_ = UDNeutral;
    return;
  }

  // Both Up and Down buttons are pressed.

  switch (ud_state_) {
    case OnlyUp:
      ud_state_ = UpThenDown;
      break;
    case OnlyDown:
      ud_state_ = DownThenUp;
      break;
    case UDNeutral:
      // Well, they are pressed at the same time but there is no great reason
      // to handle it separately.
      ud_state_ = DownThenUp;
      break;
    case UpThenDown:
    case DownThenUp:
      // Keep the current state.
      break;
  }
}

void DPad::UpdateLRStates() {
  bool left_pressed = false;
  for (auto& left_switch : left_switches_) {
    if (left_switch->IsOn()) {
      left_pressed = true;
      break;
    }
  }

  bool right_pressed = false;
  for (auto& right_switch : right_switches_) {
    if (right_switch->IsOn()) {
      right_pressed = true;
      break;
    }
  }

  // One or zero button is pressed.
  if (left_pressed && !right_pressed) {
    lr_state_ = OnlyLeft;
    return;
  } else if (!left_pressed && right_pressed) {
    lr_state_ = OnlyRight;
    return;
  } else if (!left_pressed && !right_pressed) {
    lr_state_ = LRNeutral;
    return;
  }

  // Both Left and Right buttons are pressed.

  switch (lr_state_) {
    case OnlyLeft:
      lr_state_ = LeftThenRight;
      break;
    case OnlyRight:
      lr_state_ = RightThenLeft;
      break;
    case LRNeutral:
      // Well, they are pressed at the same time but there is no great reason
      // to handle it separately.
      lr_state_ = LeftThenRight;
      break;
    case LeftThenRight:
    case RightThenLeft:
      // Keep the current state.
      break;
  }
}