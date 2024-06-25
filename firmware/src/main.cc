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
Udong udong;

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
  TinyUSBDevice.addInterface(Serial);

  Serial.begin(921600);
  Serial.setTimeout(100);
  Serial.setStringDescriptor("Udong");

  if (!udong.SetupHidDevices()) {
    Serial.println("Failed to setup HID devices");
  }

  // wait for
  /*
  for (int i = 0; i < 5; i++) {
    delay(1000);
  }
  */

  udong.Setup();
}

inline int16_t map_u16_s16(uint16_t v) {
  return (uint32_t)v - (1 << 15);
}

// every 100ms
Throttling teleplot_runner(100, []() {
#if TELEPLOT
  udong.circuit->analog_switches[0]->TelePrint();
  Serial.flush();
  udong.circuit->analog_switches[1]->TelePrint();
  /*
  Serial.printf(
      ">ADC-600-mV: %lf\n",
      udong.circuit->adc_600mv_input.Read() * 3300.0 / 65536.0);
  */
  Serial.flush();
  delay(1);
#endif
});

bool need_to_save_calibration_store = false;
Throttling calibration_runner(100, []() {
  bool calibratin_has_run = false;
  for (auto& analog_switch : udong.circuit->analog_switches) {
    if (analog_switch->NeedRecalibration()) {
      Serial.printf("Calibrate switch-%d\n", analog_switch->GetId());
      analog_switch->Calibrate();
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
    udong.circuit->calibration_store.SaveIntoFile();
    udong.circuit->calibration_store.ClearUpdatedFlag();
    unsigned long to = time_us_32();
    Serial.printf(">save(us): %lu\n", to - from);
  }
});

uint32_t last_report_t = 1000;

void loop() {
  serial_handler.HandleSerial(udong);

  // TODO: Refactoring...
  uint32_t now = time_us_32();
  calibration_runner.MaybeRun(now);
  teleplot_runner.MaybeRun(now);
  udong.Loop();
}
