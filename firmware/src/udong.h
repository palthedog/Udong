#ifndef UDONG_H
#define UDONG_H

#include <Adafruit_TinyUSB.h>
#include <Adafruit_USBD_CDC.h>
#include <Arduino.h>

#include <memory>

#include "config/config.h"
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
  uint16_t buttons;

  inline void Clear() {
    d_pad = 0;
    buttons = 0;
  }

  inline void PressButton(int index) {
    if (index < 0 || index >= 16) {
      Serial.printf("ERROR: invalid button index: %d\n", index);
      return;
    }

    uint16_t button_bit = 1u << index;
    buttons |= button_bit;
  }
};

const uint8_t LED_PIN = D25;
struct Circuit {
  CalibrationStore calibration_store;

  Multiplexer8 mux0;
  Multiplexer8 mux1;

  AnalogOutputPin led_pin;
  TriangleInput triangle_in;

  std::vector<std::shared_ptr<AnalogInput>> analog_switch_raw_ins;
  std::vector<std::shared_ptr<AnalogInput>> analog_switch_multi_sampled_ins;
  std::vector<std::shared_ptr<AnalogSwitch>> analog_switches;

  // DO NOT USE IT until we fix a hardware bug!!!
  // TODO: refrence 0.6V must be connected to an ANALOG input not Digital
  // Input AnalogInputPin adc_600mv_input;
  std::vector<bool> enabled_switchs;

  UdongConfig config;

  Circuit(UdongConfig _config)
      : mux0(
            std::make_shared<DigitalOutputPin>(D16),
            std::make_shared<DigitalOutputPin>(D17),
            std::make_shared<DigitalOutputPin>(D18),
            std::make_shared<AnalogInputPin>(A0)),
        mux1(
            std::make_shared<DigitalOutputPin>(D19),
            std::make_shared<DigitalOutputPin>(D20),
            std::make_shared<DigitalOutputPin>(D21),
            std::make_shared<AnalogInputPin>(A1)),
        led_pin(D25),
        // adc_600mv_input(A2),
        config(_config) {
    // The analog sensors are located to optimize wiring on the PCB.
    // In contrast to that, switch_id is based on HID button ID
    // See
    //   https://learn.microsoft.com/windows/win32/xinput/directinput-and-xusb-devices
    // for more details about HID button IDs.
    // {hardware_id, switch_id}
    enabled_switchs.resize(16, false);

    // For my dev board
    enabled_switchs[0] = true;
    enabled_switchs[1] = true;
    enabled_switchs[2] = true;
    enabled_switchs[3] = true;
    enabled_switchs[4] = true;

    enabled_switchs[8] = true;
    enabled_switchs[9] = true;

    for (uint8_t switch_id = 0; switch_id < 16; switch_id++) {
      if (enabled_switchs[switch_id]) {
        if (switch_id < 8) {
          analog_switch_raw_ins.push_back(mux0.GetInput(switch_id));
        } else {
          analog_switch_raw_ins.push_back(mux1.GetInput(switch_id - 8));
        }
      } else {
        analog_switch_raw_ins.push_back(std::make_shared<AnalogGroundInput>());
      }
    }
    const int kButtonCount = 16;

    for (int switch_id = 0; switch_id < kButtonCount; switch_id++) {
      analog_switch_multi_sampled_ins.push_back(
          std::make_shared<MultiSampling<1, 0, 0>>(
              analog_switch_raw_ins[switch_id]));

      const AnalogSwitchGroup& switch_config =
          config.getConfigFromSwitchId(switch_id);
      std::unique_ptr<Trigger> trigger;
      if (switch_config.trigger_type == "rapid-trigger") {
        const RapidTriggerConfig& rt_conf = switch_config.rapid_trigger;
        trigger.reset(new RapidTrigger(
            rt_conf.act, rt_conf.rel, rt_conf.f_act, rt_conf.f_rel));
      } else {
        const StaticTriggerConfig& st_conf = switch_config.static_trigger;
        trigger.reset(new StaticTrigger(st_conf.act, st_conf.rel));
      }
      analog_switches.push_back(std::make_shared<AnalogSwitch>(
          switch_id,
          analog_switch_multi_sampled_ins[switch_id],
          calibration_store.GetSwitchRef(switch_id),
          std::move(trigger)));
    }
  }

  void CalibrateAllZeroPoint() {
    for (auto& analog_switch : analog_switches) {
      analog_switch->CalibrateZeroPoint();
    }
  }
};

// context
class Udong {
  bool usb_hids_setup_completed;

  // Key: Switch-ID
  std::map<uint8_t, ButtonAssignment> button_assignments;

  GamepadReport gamepad_report;
  Adafruit_USBD_HID usb_hid;

  void FillGamepadReport() {
    // Debug info
    /*
    gamepad_report.z = circuit.analog_switch_raw_ins[0]->Read() / 2;
    gamepad_report.rx =
        circuit.analog_switches[0]->GetLastPressMm() / 4.0 * UINT16_MAX / 2;
    gamepad_report.ry = circuit.analog_switch_raw_ins[2]->Read() / 2;
    gamepad_report.rz =
        circuit.analog_switches[2]->GetLastPressMm() / 4.0 * UINT16_MAX / 2;
  */
    // analog switches
    gamepad_report.Clear();
    for (auto& analog_switch : circuit->analog_switches) {
      if (!analog_switch->IsOn()) {
        continue;
      }

      auto f = this->button_assignments.find(analog_switch->GetId());
      if (f == this->button_assignments.end()) {
        // Nothing is assigned to this switch
        Serial.printf(
            "Nothing is assigned to switch %d\n", analog_switch->GetId());
        continue;
      }

      const ButtonId& button_id = f->second.button_id;
      if (button_id.type == PushButton) {
        gamepad_report.PressButton(
            button_id.selector.push_button.push_button_id);
      } else {
        Serial.printf("unknown type: %d\n", button_id.type);
      }
    }

    // temporal D-pad impl.
    gamepad_report.d_pad = (time_us_32() / 1000000) % 9;
  }

  bool MaybeSendReport() {
    if (!usb_hid.ready()) {
      delayMicroseconds(50);
      return false;
    }

    FillGamepadReport();
    if (!usb_hid.sendReport(0, &gamepad_report, sizeof(gamepad_report))) {
      Serial.println("Failed to send report");
    }

#if TELEPLOT
    uint32_t now = time_us_32();
    Serial.printf(">report-dt(ms): %lf\n", (now - last_report_t) / 1000.0);
    last_report_t = now;
#endif
    // Consider using sleep_until? (with performance measurement of loop())
    delayMicroseconds(500);
    return true;
  }

  void LoadConfig() {
    UdongConfig config = loadUdonConfig();
    Serial.println("Loaded button assignments");
    for (auto b : config.button_assignments) {
      Serial.println(b.button_id.selector.push_button.push_button_id);
    }

    // Configure circuit
    circuit = std::make_unique<Circuit>(config);
    circuit->calibration_store.LoadFromFile();
    circuit->CalibrateAllZeroPoint();
    // We need to call Calibrate after loading calibration data to update all
    // related data (e.g. lookup-table)
    for (auto& analog_switch : circuit->analog_switches) {
      analog_switch->Calibrate();
      analog_switch->DumpLastState();
    }

    // Button assignment
    button_assignments.clear();
    for (auto it : config.button_assignments) {
      button_assignments.insert(std::make_pair(it.switch_id, it));
    }
  }

 public:
  // TODO: Move it to private.
  std::unique_ptr<Circuit> circuit;

  Udong() {
    usb_hids_setup_completed = false;

    gamepad_report.d_pad = GAMEPAD_HAT_CENTERED;
    gamepad_report.buttons = 0;
  }

  void Loop() {
    circuit->led_pin.Write(circuit->triangle_in.Read());

    MaybeSendReport();
  }

  bool SetupHidDevices() {
    // Make sure that this method is called only once.
    if (usb_hids_setup_completed) {
      return false;
    }
    usb_hids_setup_completed = true;

    usb_hid.setPollInterval(1);  // 1ms
    usb_hid.setReportDescriptor(kHidDescriptor, sizeof(kHidDescriptor));

    if (!usb_hid.begin()) {
      Serial.println("Failed to begin USB_HID");
      return false;
    }

    // Wait for the USB device to be mounted
    while (!TinyUSBDevice.mounted()) {
      delay(1);
    }

    if (!usb_hid.ready()) {
      Serial.println("ERRROR: Failed to begin Gamepad device");
      return false;
    }

    // Send very initial report
    usb_hid.sendReport(0, &gamepad_report, sizeof(gamepad_report));

    return true;
  }

  void Setup() {
    // Note that it's safe to call LittleFS.begin() multiple times.
    if (!LittleFS.begin()) {
      Serial.println("Failed to initialize LittleFS");
    }
    LoadConfig();
  }

  void ReloadConfig() {
    LoadConfig();
  }
};

#endif
