#include <Adafruit_TinyUSB.h>
#include <Arduino.h>

#include "gamepad.h"
#include "io_utils/io.h"
#include "io_utils/led.h"
#include "io_utils/multiplexer.h"
#include "io_utils/soft/hall.h"
#include "io_utils/soft/triangle.h"
#include "switch.h"

// Sample code:
//     https://github.com/adafruit/Adafruit_TinyUSB_Arduino/blob/master/examples/HID/hid_gamepad/hid_gamepad.ino

const uint32_t kAdcBits = 12;
const uint32_t kAnalogMin = 0;
const uint32_t kAnalogMax = (1 << kAdcBits) - 1;
const int32_t kSignedAnalogMin = kAnalogMin - ((1 << (kAdcBits - 1)) - 1);
const int32_t kSignedAnalogMax = kAnalogMax - ((1 << (kAdcBits - 1)) - 1);

const uint8_t kHidDescriptor[] = {
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),
    HID_USAGE(HID_USAGE_DESKTOP_GAMEPAD),
    HID_COLLECTION(HID_COLLECTION_APPLICATION),

    // 12 bit analog stick (X, Y)
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),
    HID_USAGE(HID_USAGE_DESKTOP_X),
    HID_USAGE(HID_USAGE_DESKTOP_Y),
    HID_LOGICAL_MIN_N(kSignedAnalogMin, 2),
    HID_LOGICAL_MAX_N(kSignedAnalogMax, 2),
    HID_REPORT_COUNT(2),
    HID_REPORT_SIZE(16),
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

    // Unsigned 12 bit analog button Z (for debugging)
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),
    HID_USAGE(HID_USAGE_DESKTOP_Z),
    HID_USAGE(HID_USAGE_DESKTOP_RX),
    HID_USAGE(HID_USAGE_DESKTOP_RY),
    HID_USAGE(HID_USAGE_DESKTOP_RZ),
    HID_LOGICAL_MIN_N(kAnalogMin, 2),
    HID_LOGICAL_MAX_N(kAnalogMax, 2),
    HID_REPORT_COUNT(4),
    HID_REPORT_SIZE(16),
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

    // 8 bit DPad/Hat Button Map
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),
    HID_USAGE(HID_USAGE_DESKTOP_HAT_SWITCH),
    HID_LOGICAL_MIN(1),
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

struct Circuit {
  Multiplexer8 mux;
  Input* joy_x_in;
  Input* joy_y_in;

  OutputPin led_pin;
  TriangleInput<0, 255> triangle_in;

  HallInput<
      kAnalogMin,
      kAnalogMax,
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

  Input* analog_switch_0_raw_in;
  Input* analog_switch_1_raw_in;
  AnalogSwitch analog_switch_0;
  AnalogSwitch analog_switch_1;

  Circuit()
      : mux(new OutputPin(D10),
            new OutputPin(D11),
            new OutputPin(D12),
            new InputPin(A0)),
        led_pin(D25),
        analog_switch_soft(&hall_in),
        analog_switch_0_raw_in(mux.GetInput(2)),
        analog_switch_1_raw_in(mux.GetInput(3)),
        analog_switch_0(mux.GetInput(2)),
        analog_switch_1(mux.GetInput(3)) {
    joy_x_in = mux.GetInput(0);
    joy_y_in = mux.GetInput(1);
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
  Serial.begin(115200);
  analogReadResolution(kAdcBits);

  usb_hid.setPollInterval(1);  // 1ms
  usb_hid.setReportDescriptor(kHidDescriptor, sizeof(kHidDescriptor));

  usb_hid.begin();

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

  // wait for
  delay(5000);
  Serial.println("Start");
}

int logt = 0;

void loop() {
  circuit.led_pin.AnalogWrite(circuit.triangle_in.AnalogRead());

  int joy_x = circuit.joy_x_in->AnalogRead();
  int joy_y = circuit.joy_y_in->AnalogRead();

  gamepad_report.x =
      map(joy_x,
          kAnalogMin,
          kAnalogMax + 1,
          kSignedAnalogMin,
          kSignedAnalogMax + 1);
  gamepad_report.y =
      map(joy_y,
          kAnalogMin,
          kAnalogMax + 1,
          kSignedAnalogMin,
          kSignedAnalogMax + 1);

  gamepad_report.z = circuit.analog_switch_1_raw_in->AnalogRead();
  gamepad_report.rx =
      circuit.analog_switch_1.GetLastPressMm() * kAnalogMax / 4.0;

  //// analog switch
  // soft switch(for test)
  int hall = circuit.hall_in.AnalogRead();
  double press_mm = circuit.analog_switch_soft.GetLastPressMm();
  bool as0_on = circuit.analog_switch_soft.IsOn();
  gamepad_report.ry = hall;
  gamepad_report.rz =
      press_mm * kAnalogMax / 4.0;  // [0, 4.0] -> [0, kAnalogMax]
  gamepad_report.UpdateButton(0, as0_on);

  gamepad_report.UpdateButton(1, circuit.analog_switch_0.IsOn());
  gamepad_report.UpdateButton(2, circuit.analog_switch_1.IsOn());

  if (millis() > logt) {
    uint32_t dist_micro = circuit.hall_in.GetDistanceMicros();
    uint32_t micro_tesla =
        circuit.hall_in.DistanceMicrosToMicroTesla(dist_micro);
    Serial.printf(
        "TRUE: dist: %.2f mm, mag: %.2f mT\n",
        dist_micro / 1000.0,
        micro_tesla / 1000.0);
    Serial.printf("ESTI: press: %.2f mm, hall-in: %d\n", press_mm, hall);
    Serial.println("---");
    // TODO: Call it by swith itself.
    Serial.println("Calibrate soft switch");
    circuit.analog_switch_soft.Calibrate();
    Serial.println("Calibrate switch-0");
    circuit.analog_switch_0.Calibrate();
    Serial.println("Calibrate switch-1");
    circuit.analog_switch_1.Calibrate();
    Serial.println("Dump switch-1");
    circuit.analog_switch_1.DumpLookupTable();
    Serial.println("---");
    Serial.println("analog-switch0");
    circuit.analog_switch_1.DumpLastState();
    Serial.println("TODO: Noise updates mag-flux min/max...");

    logt = millis() + 1000;
    delay(1);
  }

  circuit.analog_switch_1.DumpLastState();
  delay(1);

  // temporal D-pad impl.
  gamepad_report.hat = (time_us_32() / 1000000) % 9;

  usb_hid.sendReport(0, &gamepad_report, sizeof(gamepad_report));
}
