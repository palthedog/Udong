#include <Arduino.h>

#include "Adafruit_TinyUSB.h"
#include "led/led.h"

const uint8_t LED_PIN = D25;

const uint8_t JOY_X_PIN = A0;
const uint8_t JOY_Y_PIN = A1;

// Sample code:
//     https://github.com/adafruit/Adafruit_TinyUSB_Arduino/blob/master/examples/HID/hid_gamepad/hid_gamepad.ino

const uint8_t kHidDescriptor[] = {
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),
    HID_USAGE(HID_USAGE_DESKTOP_GAMEPAD),
    HID_COLLECTION(HID_COLLECTION_APPLICATION),

    // 10 bit analog stick (X, Y)
    HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),
    HID_USAGE(HID_USAGE_DESKTOP_X),
    HID_USAGE(HID_USAGE_DESKTOP_Y),
    HID_LOGICAL_MIN_N(-511, 2),
    HID_LOGICAL_MAX_N(512, 2),
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
  int16_t x;    ///< Delta x  movement of left analog-stick
  int16_t y;    ///< Delta y  movement of left analog-stick
  uint8_t hat;  ///< Buttons mask for currently pressed buttons in the DPad/hat
  uint16_t buttons;  ///< Buttons mask for currently pressed buttons
};

Adafruit_USBD_HID usb_hid;

GamepadReport gamepad_report;

Led led(LED_PIN);

void setup() {
  led.StartBlink(1000);

#if defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_ARCH_RP2040)
  // Manual begin() is required on core without built-in support for TinyUSB
  // such as mbed rp2040
  TinyUSB_Device_Init(0);
#endif

  Serial.begin(115200);

  usb_hid.setPollInterval(1);  // 1ms
  usb_hid.setReportDescriptor(kHidDescriptor, sizeof(kHidDescriptor));

  usb_hid.begin();

  // Wait for the USB device to be mounted
  while (!TinyUSBDevice.mounted()) {
    delay(1);
  }
  pinMode(JOY_X_PIN, INPUT);
  pinMode(JOY_Y_PIN, INPUT);

  if (!usb_hid.ready()) {
    Serial.print("ERRROR: Failed to begin Gamepad device");
    return;
  }

  gamepad_report.x = 0;
  gamepad_report.y = 0;
  gamepad_report.hat = GAMEPAD_HAT_CENTERED;
  gamepad_report.buttons = 0;
  usb_hid.sendReport(0, &gamepad_report, sizeof(gamepad_report));
}

void loop() {
  led.Step();

  int joy_x = analogRead(JOY_X_PIN);
  int joy_y = analogRead(JOY_Y_PIN);

  // map [0, 1023] to  [-511, 512]
  gamepad_report.x = joy_x - 511;
  gamepad_report.y = joy_y - 511;

  // temporal D-pad impl.
  gamepad_report.hat = (time_us_32() / 1000000) % 9;

  uint32_t tmp = (time_us_32() / 1000000) % 17;
  if (tmp == 0) {
    gamepad_report.buttons = 0;
  } else {
    gamepad_report.buttons = 1 << (tmp - 1);
  }

  usb_hid.sendReport(0, &gamepad_report, sizeof(gamepad_report));

  Serial.printf("rx: %d, ry: %d\n", gamepad_report.x, gamepad_report.y);

  return;
}
