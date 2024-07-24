#ifndef UDONG_H
#define UDONG_H

#include <Adafruit_TinyUSB.h>
#include <Adafruit_USBD_CDC.h>
#include <Arduino.h>

#include <memory>

#include "boards/boards.h"
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
#include "proto/rpc.pb.h"
#include "proto_util.h"
#include "switch/analog_switch.h"
#include "switch/digital_switch.h"
#include "switch/triggers/rapid_trigger.h"
#include "switch/triggers/static_trigger.h"
#include "throttling.h"

// context
class Udong {
  UdongConfig config;

  std::unique_ptr<Board> circuit;

  TriangleInput triangle_in_;

  bool usb_hids_setup_completed;

  CalibrationStore calibration_store_;

  std::vector<std::shared_ptr<AnalogInput>> analog_switch_multi_sampled_ins;
  std::vector<std::shared_ptr<AnalogSwitch>> analog_switches_;
  std::vector<std::shared_ptr<DigitalSwitch>> digital_switches_;

  std::vector<std::shared_ptr<Button>> buttons_;

  DPad d_pad_;

  GamepadReport gamepad_report;
  Adafruit_USBD_HID usb_hid;

  std::function<void()> on_report_sent_;

  Throttling update_led_;
  Throttling calibration_runner_;

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
    // TODO: Consider; if we can estimate the time to fill the report, we can
    // fill the report by just before the USB_HID is ready. Like:
    //    wait_until(usb_hid_ready_time - time_to_fill_report);
    //    FillGameReport();  // may consume time_to_fill_report us
    //    while (!usb_hid.ready()) {
    //       // we can expect that usb_hid will be ready immediately.
    //       sleep_us(1);
    //    }

    // Fill the gamepad report even if the USB HID is not ready yet.
    // It improves the actual polling-rate a little because it takes time to
    // fill the report and it makes a time gap between "ready" and "sendReport".
    FillGamepadReport();

    if (!usb_hid.ready()) {
      return false;
    }

    irq_set_enabled(USBCTRL_IRQ, false);
    bool result =
        usb_hid.sendReport(0, &gamepad_report, sizeof(gamepad_report));
    irq_set_enabled(USBCTRL_IRQ, true);
    if (tud_task_event_ready()) {
      TinyUSB_Device_Task();
    }

    if (!result) {
      Serial.println("Failed to send report");
      return false;
    }
    this->on_report_sent_();

    return true;
  }

  void ConfigureSwitches() {
    // Configure analog switches
    auto& analog_inputs = circuit->GetAnalogInputs();

    size_t size = analog_inputs.size();
    for (size_t switch_id = 0; switch_id < size; switch_id++) {
      auto analog_input = analog_inputs[switch_id];
      analog_switch_multi_sampled_ins.push_back(
          std::make_shared<MultiSampling<1, 0, 0>>(analog_input));

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
      Serial.printf(
          "%d total travel distance: %lf\n",
          switch_id,
          switch_config.total_travel_distance());
      analog_switches_.push_back(std::make_shared<AnalogSwitch>(
          switch_id,
          analog_switch_multi_sampled_ins[switch_id],
          calibration_store_.GetSwitchRef(switch_id),
          std::move(trigger),
          switch_config.total_travel_distance()));
    }

    for (auto digital_switch_in : circuit->GetDigitalInputs()) {
      Serial.printf("digital-switch-\n");
      digital_switches_.push_back(
          std::make_shared<DigitalSwitch>(digital_switch_in));
    }
  }

  const String LoadBoardName() {
    // Load board name
    const char* kBoardNamePath = "/baked/board_name.txt";
    if (LittleFS.exists(kBoardNamePath)) {
      File file = LittleFS.open(kBoardNamePath, "r");
      if (file) {
        String board_name = file.readStringUntil('\n');
        file.close();
        return board_name.c_str();
      }
    }
    return "Unknown";
  }

  void LoadConfig(bool reloading) {
    buttons_.clear();
    d_pad_.Clear();
    analog_switches_.clear();
    analog_switch_multi_sampled_ins.clear();
    digital_switches_.clear();

    String board_name = LoadBoardName();
    config = loadUdonConfig();
    config.mutable_baked()->set_board_name(board_name.c_str());

    // Configure circuit
    if (config.baked().board_name() == "Udong Board rev.1") {
      circuit = std::make_unique<UdongPrototype1>();
    } else if (config.baked().board_name() == "Udong Board rev.2") {
      circuit = std::make_unique<UdongPrototype2>();
    } else if (config.baked().board_name() == "Breadboard") {
      circuit = std::make_unique<Breadboard>();
    } else {
      // TODO
    }
    ConfigureSwitches();

    CalibrateAllZeroPoint();
    for (auto& analog_switch : analog_switches_) {
      // Calibration for updating the lookup table
      analog_switch->Calibrate();
      analog_switch->DumpLastState();
    }

    // Set D-Pad configuration
    d_pad_.SetConfig(config.d_pad_config());

    // Button assignment
    for (const ButtonAssignment& b : config.button_assignments()) {
      std::shared_ptr<Switch> switch_ptr;
      if (b.switch_id().type() == SwitchType::ANALOG_SWITCH) {
        if (analog_switches_.size() <= b.switch_id().id()) {
          // We don't have that many analog switches
          continue;
        }
        switch_ptr = analog_switches_[b.switch_id().id()];
      } else if (b.switch_id().type() == SwitchType::DIGITAL_SWITCH) {
        if (digital_switches_.size() <= b.switch_id().id()) {
          continue;
        }
        switch_ptr = digital_switches_[b.switch_id().id()];
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
  Udong(const std::function<void()>& on_report_sent)
      : on_report_sent_(on_report_sent),
        update_led_(
            10, [&]() { circuit->GetPowerLed().Write(triangle_in_.Read()); }),
        calibration_runner_(1000, [&]() {
          for (auto& analog_switch : this->GetAnalogSwitches()) {
            if (analog_switch->NeedRecalibration()) {
              Serial.printf("Calibrate switch-%d\n", analog_switch->GetId());
              analog_switch->Calibrate();
              // Do NOT calibrate multiple switches at once since it is a
              // bit heavy routine.
              break;
            }
          }
        }) {
    usb_hids_setup_completed = false;

    gamepad_report.d_pad = GAMEPAD_HAT_CENTERED;
    gamepad_report.press_buttons = 0;
  }

  void Loop() {
    uint32_t now = time_us_32();
    update_led_.MaybeRun(now);
    calibration_runner_.MaybeRun(now);
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

    LoadConfig(false);
  }

  void ReloadConfig() {
    LoadConfig(true);
  }

  void CalibrateAllZeroPoint() {
    for (auto& analog_switch : analog_switches_) {
      analog_switch->CalibrateZeroPoint();
    }
  }

  std::vector<std::shared_ptr<AnalogSwitch>> GetAnalogSwitches() {
    return analog_switches_;
  }

  std::vector<std::shared_ptr<DigitalSwitch>> GetDigitalSwitches() {
    return digital_switches_;
  }

  UdongConfig& GetConfig() {
    return config;
  }
};

#endif
