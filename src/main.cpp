#include <Arduino.h>

void setup() {
  pinMode(25, OUTPUT);
}

void loop() {
  int i;
  for (i = 0; i < 256; i ++)
  {
    analogWrite(25, i);
    delay(8);
  }

  for (i = 0; i < 256; i ++)
  {
    analogWrite(25, 255 - i);
    delay(8);
  }
  delay(1000);
}
