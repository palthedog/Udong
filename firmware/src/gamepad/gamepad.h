#ifndef GAMEPAD_REPORT_H
#define GAMEPAD_REPORT_H

#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include <stdint.h>

const uint8_t kHidDescriptor[] = {
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),
    HID_USAGE(HID_USAGE_DESKTOP_GAMEPAD),
    HID_COLLECTION(HID_COLLECTION_APPLICATION),

    // 16 bit analog stick (X, Y)
    /*
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),
    HID_USAGE(HID_USAGE_DESKTOP_X),
    HID_USAGE(HID_USAGE_DESKTOP_Y),
    HID_LOGICAL_MIN_N(INT16_MIN, 2),
    HID_LOGICAL_MAX_N(INT16_MAX, 2),
    HID_REPORT_COUNT(2),
    HID_REPORT_SIZE(16),
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

    // Unsigned 16 bit analog button Z (for debugging)
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),
    HID_USAGE(HID_USAGE_DESKTOP_Z),
    HID_USAGE(HID_USAGE_DESKTOP_RX),
    HID_USAGE(HID_USAGE_DESKTOP_RY),
    HID_USAGE(HID_USAGE_DESKTOP_RZ),
    HID_LOGICAL_MIN_N(0, 2),
    // We can't use UINT16_MAX here because it's always treated as signed value.
    HID_LOGICAL_MAX_N(INT16_MAX, 2),
    HID_REPORT_COUNT(4),
    HID_REPORT_SIZE(16),
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),
*/

    // 8 bit DPad/Hat Button Map
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),
    HID_USAGE(HID_USAGE_DESKTOP_HAT_SWITCH),
    HID_LOGICAL_MIN(1),
    HID_LOGICAL_MAX(8),
    HID_PHYSICAL_MIN(0),
    HID_PHYSICAL_MAX_N(315, 2),
    HID_REPORT_COUNT(1),
    HID_REPORT_SIZE(8),
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

    // 32 press buttons
    HID_USAGE_PAGE(HID_USAGE_PAGE_BUTTON),
    HID_USAGE_MIN(1),
    HID_USAGE_MAX(32),
    HID_LOGICAL_MIN(0),
    HID_LOGICAL_MAX(1),
    HID_REPORT_COUNT(32),
    HID_REPORT_SIZE(1),
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

    // End
    HID_COLLECTION_END,
};

struct TU_ATTR_PACKED GamepadReport {
  /*
  // analog stick
  int16_t x;
  int16_t y;

  // debug
  uint16_t z;
  uint16_t rx;
  uint16_t ry;
  uint16_t rz;
  */

  // Buttons mask for currently pressed buttons in the DPad/hat
  uint8_t d_pad;
  // Buttons mask for currently pressed buttons
  uint32_t press_buttons;

  inline void Clear() {
    d_pad = 0;
    press_buttons = 0;
  }

  inline void PressButton(int press_button_id) {
    if (press_button_id < 0 || press_button_id >= 32) {
      Serial.printf("ERROR: invalid press button ID: %d\n", press_button_id);
      return;
    }

    uint32_t button_bit = 1u << press_button_id;
    press_buttons |= button_bit;
  }
};

#endif
