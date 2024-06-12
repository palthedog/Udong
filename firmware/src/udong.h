#ifndef UDONG_H
#define UDONG_H

#include <Adafruit_TinyUSB.h>
#include <Adafruit_USBD_CDC.h>
#include <Arduino.h>

#include "io_utils/io.h"
#include "io_utils/multi_sampling.h"
#include "io_utils/multiplexer.h"
#include "io_utils/soft/triangle.h"
#include "switch/analog_switch.h"
#include "switch/triggers/rapid_trigger.h"
#include "switch/triggers/static_trigger.h"

const uint8_t kHidDescriptor[] = {
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),
    HID_USAGE(HID_USAGE_DESKTOP_GAMEPAD),
    HID_COLLECTION(HID_COLLECTION_APPLICATION),

    // 16 bit analog stick (X, Y)
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

    // 8 bit DPad/Hat Button Map
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),
    HID_USAGE(HID_USAGE_DESKTOP_HAT_SWITCH),
    HID_LOGICAL_MIN(0),
    HID_LOGICAL_MAX(8),
    HID_REPORT_COUNT(1),
    HID_REPORT_SIZE(8),
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

    // 16 buttons
    HID_USAGE_PAGE(HID_USAGE_PAGE_BUTTON),
    HID_USAGE_MIN(1),
    HID_USAGE_MAX(16),
    HID_LOGICAL_MIN(0),
    HID_LOGICAL_MAX(1),
    HID_REPORT_COUNT(16),
    HID_REPORT_SIZE(1),
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

    // End
    HID_COLLECTION_END,
};

struct TU_ATTR_PACKED GamepadReport {
  // analog stick
  int16_t x;
  int16_t y;

  // debug
  uint16_t z;
  uint16_t rx;
  uint16_t ry;
  uint16_t rz;

  uint8_t hat;  ///< Buttons mask for currently pressed buttons in the DPad/hat
  uint16_t buttons;  ///< Buttons mask for currently pressed buttons

  inline void UpdateButton(int index, bool is_on) {
    if (index < 0 || index >= 16) {
      Serial.printf("ERROR: invalid button index: %d\n", index);
      return;
    }

    uint16_t button_bit = 1u << index;
    if (is_on) {
      buttons |= button_bit;
    } else {
      buttons &= ~button_bit;
    }
  }
};

const uint8_t LED_PIN = D25;

/*
  HallInput<
      0,
      (1 << 16) - 1,
      // min dist
      2000,
      // max dist
      2000 + 4000,
      // Br
      10000,
      // thickness
      3400,
      // diameter
      2900,
      // sensitivity
      //   DRV5056A3 @ 3.3V
      30,
      // 3.3V
      3300,
      // quiescent voltage (mV)
      600>
      hall_in;
  AnalogSwitch analog_switch_soft;
*/

struct Circuit {
  CalibrationStore calibration_store;

  Multiplexer8 mux;

  AnalogOutputPin led_pin;
  TriangleInput triangle_in;

  std::vector<AnalogInput*> analog_switch_raw_ins;
  std::vector<AnalogInput*> analog_switch_multi_sampled_ins;
  std::vector<AnalogSwitch*> analog_switches;

  AnalogInputPin adc_600mv_input;

  Circuit()
      : mux(new DigitalOutputPin(D10),
            new DigitalOutputPin(D11),
            new DigitalOutputPin(D12),
            new AnalogInputPin(A0)),
        led_pin(D25),
        adc_600mv_input(A2) {
    analog_switch_raw_ins.push_back(mux.GetInput(0));
    analog_switch_raw_ins.push_back(mux.GetInput(1));
    analog_switch_raw_ins.push_back(new AnalogInputPin(A1));

    const int kButtonCount = 3;

    for (int id = 0; id < kButtonCount; id++) {
      analog_switch_multi_sampled_ins.push_back(
          new MultiSampling<1, 0, 0>(analog_switch_raw_ins[id]));

      analog_switches.push_back(new AnalogSwitch(
          id,
          analog_switch_multi_sampled_ins[id],
          calibration_store.GetSwitchRef(id),
          std::move(
              std::unique_ptr<Trigger>(new RapidTrigger(0.2, 3.8, 0.6, 0.3)))));
    }

    // Dummy switch for logging
    for (int id = 0; id < kButtonCount; id++) {
      analog_switches.push_back(new AnalogSwitch(
          id + 10,
          analog_switch_raw_ins[id],
          calibration_store.GetSwitchRef(id + 10),
          std::move(std::unique_ptr<Trigger>(new StaticTrigger(1.4, 1.2)))));
    }
  }

  void CalibrateAllZeroPoint() {
    for (auto& analog_switch : analog_switches) {
      analog_switch->CalibrateZeroPoint();
    }
  }
};

// context
struct Udong {
  Circuit circuit;
  GamepadReport gamepad_report;
  Adafruit_USBD_HID usb_hid;

  Udong() {
    gamepad_report.x = 0;
    gamepad_report.y = 0;
    gamepad_report.z = 0;
    gamepad_report.rx = 0;
    gamepad_report.ry = 0;
    gamepad_report.rz = 0;
    gamepad_report.hat = GAMEPAD_HAT_CENTERED;
    gamepad_report.buttons = 0;
  }
};

#endif
