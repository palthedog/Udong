#include <Adafruit_TinyUSB.h>
#include <Adafruit_USBD_CDC.h>
#include <Arduino.h>

#define TELEPLOT 1

#include "io_utils/io.h"
#include "io_utils/multiplexer.h"
#include "io_utils/noise_filter.h"
#include "io_utils/soft/hall.h"
#include "io_utils/soft/triangle.h"
#include "switch/analog_switch.h"
#include "throttling.h"

// Sample code:
//     https://github.com/adafruit/Adafruit_TinyUSB_Arduino/blob/master/examples/HID/hid_gamepad/hid_gamepad.ino

const uint32_t kAdcBits = 12;

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
  std::vector<AnalogSwitch> analog_switches;

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
    const double kA2 = 60;
    const double kA3 = 30;
    double kHallIcSensitivities_[kButtonCount] = {
        kA3,
        kA3,
        kA3,
    };

    for (int id = 0; id < kButtonCount; id++) {
      analog_switch_multi_sampled_ins.push_back(
          new NoiseFilter<4, 0, 0>(analog_switch_raw_ins[id]));

      analog_switches.push_back(AnalogSwitch(

          id,
          analog_switch_multi_sampled_ins[id],
          calibration_store.GetSwitchRef(id),
          kHallIcSensitivities_[id]));
    }

    // Dummy switch for logging
    for (int id = 0; id < kButtonCount; id++) {
      analog_switches.push_back(AnalogSwitch(
          id + 10,
          analog_switch_raw_ins[id],
          calibration_store.GetSwitchRef(id + 10),
          kHallIcSensitivities_[id]));
    }
  }

  void CalibrateAllZeroPoint() {
    for (auto& analog_switch : analog_switches) {
      analog_switch.CalibrateZeroPoint();
    }
  }
};

Circuit circuit;
GamepadReport gamepad_report;
Adafruit_USBD_HID usb_hid;

void setup() {
#if defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_ARCH_RP2040)
  // Manual begin() is required on core without built-in support for TinyUSB
  // such as mbed rp2040
  TinyUSB_Device_Init(0);
#endif

  TinyUSBDevice.clearConfiguration();

  // https://github.com/obdev/v-usb/blob/master/usbdrv/USB-IDs-for-free.txt
  const uint16_t VID = 0x16c0;
  const uint16_t PID = 0x27dc;
  TinyUSBDevice.setID(VID, PID);
  TinyUSBDevice.setLanguageDescriptor(0x0409);  // English/US
  TinyUSBDevice.setManufacturerDescriptor("Udong");
  TinyUSBDevice.setProductDescriptor("Udong");
  // TODO: We may need to append a unique serial number after colon.
  TinyUSBDevice.setSerialDescriptor("f13g.com:");
  TinyUSBDevice.addInterface(Serial);

  Serial.begin(921600);
  Serial.setTimeout(100);

  analogReadResolution(kAdcBits);

  // Set HIGH to GPIO23 to reduce switching noise
  // https://datasheets.raspberrypi.com/pico/pico-datasheet.pdf
  // Driving high the SMPS mode pin (GPIO23), to force the power supply into
  // PWM mode, can greatly reduce the inherent ripple of the SMPS at light
  // load, and therefore the ripple on the ADC supply. This does reduce the
  // power efficiency of the board at light load, so the low-power PFM mode
  // can be re-enabled between infrequent ADC measurements by driving GPIO23
  // low once more. See Section 4.4.
  pinMode(23, OUTPUT);
  digitalWrite(23, HIGH);

  usb_hid.setPollInterval(1);  // 1ms
  usb_hid.setReportDescriptor(kHidDescriptor, sizeof(kHidDescriptor));

  if (!usb_hid.begin()) {
    Serial.println("Failed to begin USB_HID");
  }

  // Wait for the USB device to be mounted
  while (!TinyUSBDevice.mounted()) {
    delay(1);
  }

  if (!usb_hid.ready()) {
    Serial.print("ERRROR: Failed to begin Gamepad device");
    return;
  }

  gamepad_report.x = 0;
  gamepad_report.y = 0;
  gamepad_report.z = 0;
  gamepad_report.rx = 0;
  gamepad_report.ry = 0;
  gamepad_report.rz = 0;
  gamepad_report.hat = GAMEPAD_HAT_CENTERED;
  gamepad_report.buttons = 0;
  usb_hid.sendReport(0, &gamepad_report, sizeof(gamepad_report));

  /*
  // wait for
  circuit.led_pin.Write(UINT16_MAX);
  delay(5000);
  circuit.led_pin.Write(0);
  Serial.println("*** Start ***");
  Serial.println("*** Start ***");
  Serial.println("*** Start ***");
  */

  if (!LittleFS.begin()) {
    Serial.println("Failed to initialize LittleFS");
  }
  circuit.calibration_store.LoadFromFile();
  circuit.CalibrateAllZeroPoint();
  // We need to call Calibrate after loading calibration data to update all
  // related data (e.g. lookup-table)
  for (auto& analog_switch : circuit.analog_switches) {
    analog_switch.Calibrate();
    analog_switch.DumpLastState();
  }
}

inline int16_t map_u16_s16(uint16_t v) {
  return (uint32_t)v - (1 << 15);
}

String serial_buffer;
void handleCmd() {
  if (serial_buffer == "dump") {
    for (auto& analog_switch : circuit.analog_switches) {
      Serial.printf("* Dump switch-%d\n", analog_switch.GetId());
      analog_switch.DumpLastState();
      analog_switch.DumpLookupTable();
    }
    return;
  }
  if (serial_buffer == "reset") {
    Serial.println("Reset all calibration data");
    circuit.calibration_store.Reset();
    circuit.CalibrateAllZeroPoint();
    for (auto& analog_switch : circuit.analog_switches) {
      analog_switch.Calibrate();
    }
  }
}

void handleSerial() {
  while (Serial.available()) {
    char ch = Serial.read();
    if (ch == '\n') {
      handleCmd();
      serial_buffer = "";
    } else {
      serial_buffer += ch;
    }
  }
}

// every 1ms
Throttling teleplot_runner(1, []() {
#if TELEPLOT
  // delay(1);  // for safety. it should be removed in prod
  circuit.analog_switches[0].TelePrint();
  circuit.analog_switches[3].TelePrint();
  Serial.printf(
      ">ADC-600-mV: %lf\n", circuit.adc_600mv_input.Read() * 3300.0 / 65536.0);
  // In case we sent too much Serial.print, usb_hid hangs without delay(1) for
  // unknown reason...
  Serial.flush();
  delay(1);  // for safety. it should be removed in prod
#endif
});

bool need_to_save_calibration_store = false;
Throttling calibration_runner(100, []() {
  bool calibratin_has_run = false;
  for (auto& analog_switch : circuit.analog_switches) {
    if (analog_switch.NeedRecalibration()) {
      Serial.printf("Calibrate switch-%d\n", analog_switch.GetId());
      analog_switch.Calibrate();
      calibratin_has_run = true;

      need_to_save_calibration_store = true;

      // Do NOT calibrate multiple switches at once since it is a bit heavy
      // routine.
      break;
    }
  }

  // TODO: It's much much slower routine.
  //       Run it only when the game pad is fully idle
  //       (e.g. all keys are not pressed for 10 seconds)
  if (need_to_save_calibration_store && !calibratin_has_run) {
    need_to_save_calibration_store = false;

    unsigned long from = time_us_32();
    circuit.calibration_store.SaveIntoFile();
    circuit.calibration_store.ClearUpdatedFlag();
    unsigned long to = time_us_32();
    Serial.printf(">save(us): %lu\n", to - from);
  }
});

uint32_t last_report_t = 1000;

void loop() {
  handleSerial();

  uint32_t now = time_us_32();

  calibration_runner.MaybeRun(now);
  teleplot_runner.MaybeRun(now);

  circuit.led_pin.Write(circuit.triangle_in.Read());

  // Debug info
  gamepad_report.z = circuit.analog_switch_raw_ins[0]->Read() / 2;
  gamepad_report.rx =
      circuit.analog_switches[0].GetLastPressMm() / 4.0 * UINT16_MAX / 2;
  gamepad_report.ry = circuit.analog_switch_raw_ins[1]->Read() / 2;
  gamepad_report.rz =
      circuit.analog_switches[1].GetLastPressMm() / 4.0 * UINT16_MAX / 2;

  // analog switches
  for (auto& analog_switch : circuit.analog_switches) {
    gamepad_report.UpdateButton(analog_switch.GetId(), analog_switch.IsOn());
  }

  gamepad_report.x = INT16_MAX * cos(M_PI * 2 * time_us_32() / 1000 / 1000.0);
  gamepad_report.y = INT16_MAX * sin(M_PI * 2 * time_us_32() / 1000 / 1000.0);

  // temporal D-pad impl.
  gamepad_report.hat = (time_us_32() / 1000000) % 9;

  if (usb_hid.ready()) {
#if TELEPLOT
    uint32_t now = time_us_32();
    Serial.printf(">report-dt(ms): %lf\n", (now - last_report_t) / 1000.0);
    last_report_t = now;
#endif
    if (!usb_hid.sendReport(0, &gamepad_report, sizeof(gamepad_report))) {
      Serial.println("Failed to send report");
    }
  }
}
