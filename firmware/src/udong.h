#ifndef UDONG_H
#define UDONG_H

#include <Adafruit_TinyUSB.h>
#include <Adafruit_USBD_CDC.h>
#include <Arduino.h>

#include <memory>

#include "config/config_util.h"
#include "gamepad/button/button.h"
#include "gamepad/button/d_pad.h"
#include "gamepad/button/press_button.h"
#include "gamepad/gamepad.h"
#include "io_utils/io.h"
#include "io_utils/multi_sampling.h"
#include "io_utils/multiplexer.h"
#include "io_utils/soft/triangle.h"
#include "proto/config.pb.h"
#include "switch/analog_switch.h"
#include "switch/digital_switch.h"
#include "switch/triggers/rapid_trigger.h"
#include "switch/triggers/static_trigger.h"

const uint8_t LED_PIN = D25;
struct Circuit {
  CalibrationStore calibration_store;

  Multiplexer8 mux0;
  Multiplexer8 mux1;

  AnalogOutputPin led_pin;
  TriangleInput triangle_in;

  // TODO: Move it to a different class somethig like "Board"
  std::vector<std::shared_ptr<AnalogInput>> analog_switch_raw_ins;
  std::vector<std::shared_ptr<AnalogInput>> analog_switch_multi_sampled_ins;
  std::vector<std::shared_ptr<AnalogSwitch>> analog_switches;

  std::vector<std::shared_ptr<DigitalInput>> digital_switch_ins;
  std::vector<std::shared_ptr<DigitalSwitch>> digital_switches;

  // DO NOT USE IT until we fix a hardware bug!!!
  // TODO: refrence 0.6V must be connected to an ANALOG input not Digital
  // Input AnalogInputPin adc_600mv_input;
  // AnalogInputPin adc_600mv_input;

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
    enabled_switchs.resize(16, true);

    bool dev_board = false;

    // For my dev board, it has very limited number of analog switches.
    if (dev_board) {
      enabled_switchs[0] = true;
      enabled_switchs[1] = true;
      enabled_switchs[2] = true;
      enabled_switchs[3] = true;
      enabled_switchs[4] = true;

      enabled_switchs[8] = true;
      enabled_switchs[9] = true;
    }

    // Analog switches
    const int kAnalogSwitchCount = 16;
    std::shared_ptr<AnalogInput> virtual_ground =
        std::make_shared<AnalogGroundInput>();
    for (uint8_t switch_id = 0; switch_id < kAnalogSwitchCount; switch_id++) {
      if (enabled_switchs[switch_id]) {
        if (switch_id < 8) {
          analog_switch_raw_ins.push_back(mux0.GetInput(switch_id));
        } else {
          analog_switch_raw_ins.push_back(mux1.GetInput(switch_id - 8));
        }
      } else {
        //
        analog_switch_raw_ins.push_back(virtual_ground);
      }
    }

    for (int switch_id = 0; switch_id < kAnalogSwitchCount; switch_id++) {
      analog_switch_multi_sampled_ins.push_back(
          std::make_shared<MultiSampling<1, 0, 0>>(
              analog_switch_raw_ins[switch_id]));

      const AnalogSwitchGroup& switch_config =
          getConfigFromSwitchId(switch_id, config);
      std::unique_ptr<Trigger> trigger;
      if (switch_config.trigger_type() == TriggerType::RAPID_TRIGGER) {
        Serial.printf("analog-switch-%d: rapid-trigger\n", switch_id);

        const RapidTriggerConfig& rt_conf = switch_config.rapid_trigger();
        trigger.reset(new RapidTrigger(
            rt_conf.act(), rt_conf.rel(), rt_conf.f_act(), rt_conf.f_rel()));
      } else {
        const StaticTriggerConfig& st_conf = switch_config.static_trigger();

        Serial.printf(
            "analog-switch-%d: static-trigger: act: %.2lf, rel: %.2lf\n",
            switch_id,
            st_conf.act(),
            st_conf.rel());

        trigger.reset(new StaticTrigger(st_conf.act(), st_conf.rel()));
      }
      analog_switches.push_back(std::make_shared<AnalogSwitch>(
          switch_id,
          analog_switch_multi_sampled_ins[switch_id],
          calibration_store.GetSwitchRef(switch_id),
          std::move(trigger)));
    }

    if (!dev_board) {
      // Don't use D23 on the dev board since it's connected to the power
      // supply...
      const bool pull_up = true;
      digital_switch_ins.push_back(
          std::make_shared<DigitalInputPin>(D22, pull_up));
      digital_switch_ins.push_back(
          std::make_shared<DigitalInputPin>(D23, pull_up));

      for (size_t i = 0; i < digital_switch_ins.size(); i++) {
        digital_switches.push_back(
            std::make_shared<DigitalSwitch>(digital_switch_ins[i]));
      }
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

  std::vector<std::shared_ptr<Button>> buttons_;
  DPad d_pad_;

  GamepadReport gamepad_report;
  Adafruit_USBD_HID usb_hid;

  void FillGamepadReport() {
    // Buttons
    gamepad_report.Clear();
    for (auto& button : buttons_) {
      button->UpdateGamepadReport(gamepad_report);
    }
    // D-Pad
    d_pad_.UpdateGamepadReport(gamepad_report);
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

    // Consider using sleep_until? (with performance measurement of loop())
    delayMicroseconds(500);
    return true;
  }

  void LoadConfig() {
    UdongConfig config = loadUdonConfig();
    Serial.println("Loaded button assignments");
    for (const ButtonAssignment& b : config.button_assignments()) {
      Serial.println(b.button_id().push_button().push_button_id());
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
    buttons_.clear();
    d_pad_.Clear();
    for (const ButtonAssignment& b : config.button_assignments()) {
      std::shared_ptr<Switch> switch_ptr;
      if (b.switch_id().type() == SwitchType::ANALOG_SWITCH) {
        switch_ptr = circuit->analog_switches[b.switch_id().id()];
      } else if (b.switch_id().type() == SwitchType::DIGITAL_SWITCH) {
        switch_ptr = circuit->digital_switches[b.switch_id().id()];
      } else {
        Serial.println("Unknown switch type");
        continue;
      }

      if (b.button_id().type() == ButtonType::PUSH) {
        Serial.printf(
            "button-id: %ld > switch-id: %s-%ld\n",
            b.button_id().push_button().push_button_id(),
            b.switch_id().type() == SwitchType::ANALOG_SWITCH ? "analog"
                                                              : "digital",
            b.switch_id().id());

        buttons_.push_back(std::make_shared<PressButton>(
            switch_ptr, b.button_id().push_button().push_button_id()));
      } else if (b.button_id().type() == ButtonType::D_PAD) {
        switch (b.button_id().d_pad().direction()) {
          case DPadButtonSelector::Direction::UNSPECIFIED_DIRECTION:
            break;
          case DPadButtonSelector::Direction::UP:
            d_pad_.AddUpSwitch(switch_ptr);
            break;
          case DPadButtonSelector::Direction::DOWN:
            d_pad_.AddDownSwitch(switch_ptr);
            break;
          case DPadButtonSelector::Direction::LEFT:
            d_pad_.AddLeftSwitch(switch_ptr);
            break;
          case DPadButtonSelector::Direction::RIGHT:
            d_pad_.AddRightSwitch(switch_ptr);
            break;
        }
      }
    }
  }

 public:
  // TODO: Move it to private.
  std::unique_ptr<Circuit> circuit;

  Udong() {
    usb_hids_setup_completed = false;

    gamepad_report.d_pad = GAMEPAD_HAT_CENTERED;
    gamepad_report.press_buttons = 0;
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
