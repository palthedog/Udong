#include "config_util.h"

#include "proto/config.pb.h"

std::vector<ButtonId> allButtonIds;

const std::vector<ButtonId>& getAllButtonIds() {
  if (!allButtonIds.empty()) {
    return allButtonIds;
  }

  for (uint32_t i = 0; i < 16; i++) {
    ButtonId button_id;
    button_id.set_type(ButtonType::PUSH);
    button_id.mutable_push_button()->set_push_button_id(i);
    allButtonIds.push_back(button_id);
  }

  const DPadButtonSelector::Direction kDirections[] = {
      DPadButtonSelector::Direction::UP,
      DPadButtonSelector::Direction::RIGHT,
      DPadButtonSelector::Direction::DOWN,
      DPadButtonSelector::Direction::LEFT,
  };

  for (DPadButtonSelector::Direction direction : kDirections) {
    ButtonId dpad_button;
    dpad_button.set_type(ButtonType::D_PAD);
    dpad_button.mutable_d_pad()->set_direction(direction);
    allButtonIds.push_back(dpad_button);
  }

  return allButtonIds;
}