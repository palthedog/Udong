#include <Adafruit_TinyUSB.h>
#include <Adafruit_USBD_CDC.h>
#include <Arduino.h>

#include <memory>

#define TELEPLOT 0

#include "io_utils/io.h"
#include "io_utils/multi_sampling.h"
#include "io_utils/multiplexer.h"
#include "io_utils/soft/hall.h"
#include "serial.h"
#include "throttling.h"
#include "udong.h"

// Sample code:
//     https://github.com/adafruit/Adafruit_TinyUSB_Arduino/blob/master/examples/HID/hid_gamepad/hid_gamepad.ino

const uint32_t kAdcBits = 12;

SerialHandler serial_handler;

void OnReportSent();
Udong udong(OnReportSent);

uint32_t sent_time = 0;
void OnReportSent() {
  serial_handler.PushAnalogSwitchState(udong);

  uint32_t now = time_us_32();
  uint32_t dt_ms = (now - sent_time) / 1000;
  if (dt_ms > 10) {
    Serial.printf(
        "Low polling rate detected: %lu Hz, dt: %lu ms\n", 1000 / dt_ms, dt_ms);
  }

  sent_time = now;
}

void setup() {
  analogReadResolution(kAdcBits);

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
  TinyUSBDevice.setVersion(0x0200);             // USB2.0
  TinyUSBDevice.setLanguageDescriptor(0x0409);  // English/US
  TinyUSBDevice.setProductDescriptor("Udong");
  // TODO: We may need to append a unique serial number after colon.
  TinyUSBDevice.setSerialDescriptor("f13g.com:");

  Serial.begin(921600);  // Well, baud rate doesn't matter for USB CDC.
  Serial.setStringDescriptor("Udong");
  TinyUSBDevice.addInterface(Serial);

  if (!udong.SetupHidDevices()) {
    Serial.println("Failed to setup HID devices");
  }

  // wait for 5 seconds so that we can connect to the serial console before
  // boot.
  /*
  for (int i = 0; i < 5; i++) {
    delay(1000);
    Serial.println("**** Starting ****");
  }
  //*/

  udong.Setup();
}

inline int16_t map_u16_s16(uint16_t v) {
  return (uint32_t)v - (1 << 15);
}

Throttling teleplot_runner(10, []() {
#if TELEPLOT
  for (size_t i = 0; i < 2; i++) {
    udong.GetAnalogSwitches()[i]->TelePrint();
  }
#endif
});

void loop() {
  serial_handler.HandleSerial(udong);

#if TELEPLOT
  teleplot_runner.MaybeRun(time_us_32());
#endif

  udong.Loop();
}
