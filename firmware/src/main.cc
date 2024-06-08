#include <Adafruit_TinyUSB.h>
#include <Arduino.h>

#define TELEPLOT 1

#include "gamepad.h"
#include "io_utils/io.h"
#include "io_utils/multiplexer.h"
#include "io_utils/noise_filter.h"
#include "io_utils/soft/hall.h"
#include "io_utils/soft/triangle.h"
#include "switch.h"

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
  AnalogInput* joy_x_in;
  AnalogInput* joy_y_in;

  AnalogOutputPin led_pin;
  TriangleInput triangle_in;
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
  AnalogInput* analog_switch_0_raw_in;
  AnalogInput* analog_switch_1_raw_in;
  AnalogSwitch analog_switch_0;
  AnalogSwitch analog_switch_1;

  AnalogInputPin adc_600mv_input;

  Circuit()
      : mux(new DigitalOutputPin(D10),
            new DigitalOutputPin(D11),
            new DigitalOutputPin(D12),
            new AnalogInputPin(A0)),
        led_pin(D25),
        // analog_switch_soft(99, &hall_in),
        analog_switch_0_raw_in(mux.GetInput(3)),
        analog_switch_1_raw_in(mux.GetInput(3)),
        analog_switch_0(0, mux.GetInput(3)),
        analog_switch_1(1, new NoiseFilter<8, 1, 1>(mux.GetInput(3))),
        adc_600mv_input(A2) {
    joy_x_in = mux.GetInput(0);
    joy_y_in = mux.GetInput(1);
  }

  void CalibrateAllZeroPoint() {
    analog_switch_0.CalibrateZeroPoint();
    analog_switch_1.CalibrateZeroPoint();
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

  // Set HIGH to GPIO23 to reduce switching noise
  // https://datasheets.raspberrypi.com/pico/pico-datasheet.pdf
  // Driving high the SMPS mode pin (GPIO23), to force the power supply into PWM
  // mode, can greatly reduce the inherent
  // ripple of the SMPS at light load, and therefore the ripple on the ADC
  // supply. This does reduce the power efficiency of the board at light load,
  // so the low-power PFM mode can be re-enabled between infrequent ADC
  // measurements by driving GPIO23 low once more. See Section 4.4.
  pinMode(23, OUTPUT);
  digitalWrite(23, HIGH);

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

  circuit.CalibrateAllZeroPoint();

  // wait for
  // delay(5000);
  // Serial.println("Start");
}

int logt = 0;
int cnt = 0;

inline int16_t map_u16_s16(uint16_t v) {
  return (uint32_t)v - (1 << 15);
}

void loop() {
  circuit.led_pin.Write(circuit.triangle_in.Read());

  uint16_t joy_x = circuit.joy_x_in->Read();
  uint16_t joy_y = circuit.joy_y_in->Read();
  gamepad_report.x = map_u16_s16(joy_x);
  gamepad_report.y = map_u16_s16(joy_y);

  //// analog switch
  // soft switch(for test)
  /*
  uint16_t hall = circuit.hall_in.Read();
  double press_mm = circuit.analog_switch_soft.GetLastPressMm();
  bool as0_on = circuit.analog_switch_soft.IsOn();
  gamepad_report.UpdateButton(0, as0_on);
  */
  uint16_t hall = 0;
  double press_mm = 0.0;
  bool as0_on = false;
  gamepad_report.UpdateButton(0, as0_on);

  // analog switch 0
  gamepad_report.UpdateButton(1, circuit.analog_switch_0.IsOn());
  gamepad_report.z = circuit.analog_switch_0_raw_in->Read() / 2;
  gamepad_report.rx =
      circuit.analog_switch_0.GetLastPressMm() / 4.0 * UINT16_MAX / 2;
  // analog switch 1
  gamepad_report.ry = circuit.analog_switch_1_raw_in->Read() / 2;
  gamepad_report.rz =
      circuit.analog_switch_1.GetLastPressMm() / 4.0 * UINT16_MAX / 2;
  gamepad_report.UpdateButton(2, circuit.analog_switch_1.IsOn());

  if (millis() > logt) {
    /*
      uint32_t dist_micro = circuit.hall_in.GetDistanceMicros();
      uint32_t micro_tesla =
          circuit.hall_in.DistanceMicrosToMicroTesla(dist_micro);
      Serial.printf(
          "TRUE: dist: %.2f mm, mag: %.2f mT\n",
          dist_micro / 1000.0,
          micro_tesla / 1000.0);
      Serial.printf("ESTI: press: %.2f mm, hall-in: %d\n", press_mm, hall);
      Serial.println("---");
      // Serial.println("Calibrate soft switch");
      // circuit.analog_switch_soft.Calibrate();
      */
    // TODO: Call it by swith itself.
    Serial.println("Calibrate switch-0");
    circuit.analog_switch_0.Calibrate();
    Serial.println("Calibrate switch-1");
    circuit.analog_switch_1.Calibrate();

    Serial.println("* Dump switch-1");
    circuit.analog_switch_1.DumpLookupTable();
    circuit.analog_switch_1.DumpLastState();

    // Serial.println("* Dump switch-soft");
    // circuit.analog_switch_soft.DumpLookupTable();

    int adc_const = circuit.adc_600mv_input.Read();
    Serial.printf(
        "ADC-600mV: %d (%.1lf mV)\n", adc_const, adc_const * 3300.0 / 65536.0);

    const double kLogPeriod_sec = 0.7;
    Serial.printf("%.1lf loop/sec\n", cnt / kLogPeriod_sec);
    delay(1);

    logt = millis() + kLogPeriod_sec * 1000;
    cnt = 0;
  }
  cnt++;

#if TELEPLOT
  Serial.printf(
      ">ADC-600-mV: %lf\n", circuit.adc_600mv_input.Read() * 3300.0 / 65536.0);
  // circuit.analog_switch_1.DumpLastState();
  delay(1);  // for safety. it should be removed in prod
#endif

  // temporal D-pad impl.
  gamepad_report.hat = (time_us_32() / 1000000) % 9;

  usb_hid.sendReport(0, &gamepad_report, sizeof(gamepad_report));
}
