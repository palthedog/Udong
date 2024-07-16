#include <Adafruit_TinyUSB.h>
#include <Adafruit_USBD_CDC.h>
#include <Arduino.h>
#include <unity.h>

#include "proto/rpc.pb.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_encode_res() {
  TEST_ASSERT_TRUE(true);
}

int runUnityTests() {
  UNITY_BEGIN();

  RUN_TEST(test_encode_res);

  return UNITY_END();
}

void setup() {
  while (!TinyUSBDevice.mounted()) {
    analogWrite(D25, 0);
    delay(300);
    analogWrite(D25, 100);
    delay(300);
  }
  analogWrite(D25, 100);

  while (!Serial) {
    analogWrite(D25, 0);
    delay(100);
    analogWrite(D25, 100);
    delay(100);
  }

  runUnityTests();
}

void loop() {
}
