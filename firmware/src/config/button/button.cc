#include "button.h"

std::vector<ButtonId> allButtonIds;

const std::vector<ButtonId>& getAllButtonIds() {
  if (!allButtonIds.empty()) {
    return allButtonIds;
  }

  for (int i = 0; i < 16; i++) {
    allButtonIds.push_back(ButtonId(PushButtonSelector{i}));
  }

  allButtonIds.push_back(ButtonId(DPadButtonSelector(Up)));
  allButtonIds.push_back(ButtonId(DPadButtonSelector(Right)));
  allButtonIds.push_back(ButtonId(DPadButtonSelector(Down)));
  allButtonIds.push_back(ButtonId(DPadButtonSelector(Left)));

  return allButtonIds;
}
