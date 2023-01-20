#include <Arduino.h>
#include "rotaryEncoder.h"

// ***************************************************************
// Rotatry encoder functions
// ***************************************************************
int number = 5;
int numberLast = 5;
void rotation(const int rotation) {
  switch (rotation) {
    case RE_CLOCKWISE:
      if (++number > 10) {
        number = 10;
      }
      break;
    case RE_ANTICLOCKWISE:
      if (--number < 1) {
        number = 1;
      }
  }
  if (number != numberLast) {
    numberLast = number;
    Serial.println(number);
  }
}

void switcher(const int state) {
  switch (state) {
    case RE_SWITCH_UP:
      Serial.println("Up");
      break;
    case RE_SWITCH_DOWN:
      Serial.println("Down");
      break;
    case RE_SWITCH_SHORT_HOLD:
      Serial.println("Shrt hold");
      break;
    case RE_SWITCH_LONG_HOLD:
      Serial.println("Long hold");
      break;
    case RE_SWITCH_SHORT_RELEASE:
      Serial.println("Short release");
      break;
    case RE_SWITCH_LONG_RELEASE:
      Serial.println("Long release");
      break;
  }
  if (number != numberLast) {
    numberLast = number;
    Serial.println(number);
  }
}

RotaryEncoder re = RotaryEncoder(PIN_A, PIN_B, PIN_SW, PIN_BEEPER);

void setup() {
  Serial.begin(BAUD_RATE);
  re.setRotationHandler(rotation);
  re.setSwitchHandler(switcher);
}

void loop() {
  re.decodeRotaryEncoder();
  //while(re.decodeSwitch() != 0)
  re.decodeSwitch();
}