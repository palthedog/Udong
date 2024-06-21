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

  analogReadResolution(kAdcBits);

  // Set HIGH to GPIO23 to reduce switching noise
  // https://datasheets.raspberrypi.com/pico/pico-datasheet.pdf
  // Driving high the SMPS mode pin (GPIO23), to force the power supply into
  // PWM mode, can greatly reduce the inherent ripple of the SMPS at light
  // load, and therefore the ripple on the ADC supply. This does reduce the
  // power efficiency of the board at light load, so the low-power PFM mode
  // can be re-enabled between infrequent ADC measurements by driving GPIO23
  // low once more. See Section 4.4.
  /*
  pinMode(23, OUTPUT);
  digitalWrite(23, HIGH);
  */

  udong.usb_hid.setPollInterval(1);  // 1ms
  udong.usb_hid.setReportDescriptor(kHidDescriptor, sizeof(kHidDescriptor));

  if (!udong.usb_hid.begin()) {
    Serial.println("Failed to begin USB_HID");
  }

  // Wait for the USB device to be mounted
  while (!TinyUSBDevice.mounted()) {
    delay(1);
  }

  if (!udong.usb_hid.ready()) {
    Serial.print("ERRROR: Failed to begin Gamepad device");
    return;
  }

  udong.usb_hid.sendReport(
      0, &udong.gamepad_report, sizeof(udong.gamepad_report));

  // wait for
  /*
  delay(5000);
  Serial.println("*** Start ***");
  Serial.println("*** Start ***");
  Serial.println("*** Start ***");
  */

  if (!LittleFS.begin()) {
    Serial.println("Failed to initialize LittleFS");
  }

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
  udong.circuit->analog_switches[2]->TelePrint();
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
  Circuit& circuit = *udong.circuit;
  GamepadReport& gamepad_report = udong.gamepad_report;
  Adafruit_USBD_HID& usb_hid = udong.usb_hid;

  uint32_t now = time_us_32();

  calibration_runner.MaybeRun(now);
  teleplot_runner.MaybeRun(now);

  circuit.led_pin.Write(circuit.triangle_in.Read());

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
  for (auto& analog_switch : circuit.analog_switches) {
    gamepad_report.UpdateButton(analog_switch->GetId(), analog_switch->IsOn());
  }

  // temporal D-pad impl.
  // gamepad_report.hat = (time_us_32() / 1000000) % 9;
  // gamepad_report.x = INT16_MAX * cos(M_PI * 2 * time_us_32() / 1000 /
  // 1000.0); gamepad_report.y = INT16_MAX * sin(M_PI * 2 * time_us_32() / 1000
  // / 1000.0);

  if (usb_hid.ready()) {
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
  } else {
    delayMicroseconds(50);
  }
}
