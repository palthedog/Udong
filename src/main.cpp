#include <Arduino.h>

#include "Adafruit_TinyUSB.h"

const uint8_t JOY_X_PIN = A0;
const uint8_t JOY_Y_PIN = A1;

// Sample code:
//     https://github.com/adafruit/Adafruit_TinyUSB_Arduino/blob/master/examples/HID/hid_gamepad/hid_gamepad.ino

// TODO: Update descriptor to set proper number of buttons
uint8_t const desc_hid_report[] = {TUD_HID_REPORT_DESC_GAMEPAD()};

Adafruit_USBD_HID usb_hid;

hid_gamepad_report_t gamepad_report;

void setup() {
#if defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_ARCH_RP2040)
  // Manual begin() is required on core without built-in support for TinyUSB
  // such as mbed rp2040
  TinyUSB_Device_Init(0);
#endif

  Serial.begin(115200);

  usb_hid.setPollInterval(1);  // 1ms
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));

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
  gamepad_report.z = 0;
  gamepad_report.rz = 0;
  gamepad_report.rx = 0;
  gamepad_report.ry = 0;
  gamepad_report.hat = 0;
  gamepad_report.buttons = 0;
  usb_hid.sendReport(0, &gamepad_report, sizeof(gamepad_report));
}

void loop() {
  int joy_x = analogRead(JOY_X_PIN);
  int joy_y = analogRead(JOY_Y_PIN);

  Serial.printf("x: %d, y: %d\n", joy_x, joy_y);

  gamepad_report.x = (int8_t)map(joy_x, 0, 1023, -127, 127);
  gamepad_report.y = (int8_t)map(joy_y, 0, 1023, -127, 127);
  // gamepad_report.buttons = (!digitalRead(BTN) << 0);
  usb_hid.sendReport(0, &gamepad_report, sizeof(gamepad_report));

  return;
}
