#include <Adafruit_TinyUSB.h>
#include <Arduino.h>

#include "gamepad.h"
#include "io_utils/io.h"
#include "io_utils/led.h"
#include "io_utils/multiplexer.h"
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
    HID_USAGE(HID_USAGE_DESKTOP_RX),
    HID_USAGE(HID_USAGE_DESKTOP_RZ),
    HID_LOGICAL_MIN_N(kAnalogMin, 2),
    HID_LOGICAL_MAX_N(kAnalogMax, 2),
    HID_REPORT_COUNT(2),
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
  uint16_t rx;
  uint16_t ry;

  uint8_t hat;  ///< Buttons mask for currently pressed buttons in the DPad/hat
  uint16_t buttons;  ///< Buttons mask for currently pressed buttons
};

const uint8_t LED_PIN = D25;

struct Circuit {
  Multiplexer8 mux;
  Input* joy_x_in;
  Input* joy_y_in;

  Input* joy_rx_in;
  Input* joy_ry_in;

  OutputPin ledPin;
  TictocInput<0, 255> tictoc;

  Circuit()
      : mux(new OutputPin(D10), new OutputPin(D11), new OutputPin(D12),
            new InputPin(A0)),
        ledPin(D25) {
    joy_x_in = mux.GetInput(0);
    joy_y_in = mux.GetInput(1);

    joy_rx_in = mux.GetInput(2);
    joy_ry_in = mux.GetInput(3);
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
  gamepad_report.rx = 0;
  gamepad_report.ry = 0;
  gamepad_report.hat = GAMEPAD_HAT_CENTERED;
  gamepad_report.buttons = 0;
  usb_hid.sendReport(0, &gamepad_report, sizeof(gamepad_report));
}

int logt = 0;

void loop() {
  circuit.ledPin.AnalogWrite(circuit.tictoc.AnalogRead());

  int joy_x = circuit.joy_x_in->AnalogRead();
  int joy_y = circuit.joy_y_in->AnalogRead();

  gamepad_report.x =
      map(joy_x, kAnalogMin, kAnalogMax, kSignedAnalogMin, kSignedAnalogMax);
  gamepad_report.y =
      map(joy_y, kAnalogMin, kAnalogMax, kSignedAnalogMin, kSignedAnalogMax);

  int rx = circuit.joy_rx_in->AnalogRead();
  int ry = circuit.joy_ry_in->AnalogRead();
  gamepad_report.rx = rx;
  gamepad_report.ry = ry;

  if (millis() > logt) {
    Serial.printf(
        "rx: %d, V: %dmV\n", rx, map(rx, kAnalogMin, kAnalogMax, 0, 3300));
    Serial.printf(
        "ry: %d, V: %dmV\n", ry, map(ry, kAnalogMin, kAnalogMax, 0, 3300));
    Serial.println("---");
    logt = millis() + 1000;
  }

  // temporal D-pad impl.
  gamepad_report.hat = (time_us_32() / 1000000) % 9;

  uint32_t tmp = (time_us_32() / 1000000) % 17;
  if (tmp == 0) {
    gamepad_report.buttons = 0;
  } else {
    gamepad_report.buttons = 1 << (tmp - 1);
  }

  usb_hid.sendReport(0, &gamepad_report, sizeof(gamepad_report));
}
