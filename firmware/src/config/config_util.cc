#include "config_util.h"

#include "proto/config.pb.h"

std::vector<ButtonId> allButtonIds;

const std::vector<ButtonId>& getAllButtonIds() {
  if (!allButtonIds.empty()) {
    return allButtonIds;
  }

  for (uint32_t i = 0; i < 16; i++) {
    allButtonIds.push_back(
        {.type = ButtonType_PUSH,
         .which_selector = ButtonId_push_button_tag,
         .selector{.push_button{
             .push_button_id = i,
         }}});
  }

  allButtonIds.push_back(
      {.type = ButtonType_D_PAD,
       .which_selector = ButtonId_d_pad_tag,
       .selector{
           .d_pad = DPadButtonSelector{
               .direction = DPadButtonSelector_Direction_UP,
           }}});

  allButtonIds.push_back(
      {.type = ButtonType_D_PAD,
       .which_selector = ButtonId_d_pad_tag,

       .selector{
           .d_pad = DPadButtonSelector{
               .direction = DPadButtonSelector_Direction_RIGHT,
           }}});

  allButtonIds.push_back(
      {.type = ButtonType_D_PAD,
       .which_selector = ButtonId_d_pad_tag,

       .selector{
           .d_pad = DPadButtonSelector{
               .direction = DPadButtonSelector_Direction_DOWN,
           }}});

  allButtonIds.push_back(
      {.type = ButtonType_D_PAD,
       .which_selector = ButtonId_d_pad_tag,
       .selector{
           .d_pad = DPadButtonSelector{
               .direction = DPadButtonSelector_Direction_LEFT,
           }}});

  return allButtonIds;
}
