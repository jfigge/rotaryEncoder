#include <Arduino.h>

#include "rotaryEncoder.h"

RotaryEncoder re = RotaryEncoder(PIN_A, PIN_B, PIN_SW);

void rotation(const int rotation) {
}

void setup() {
    Serial.begin(BAUD_RATE);
    re.setLockClickDelay(1000);
    re.setDoubleClickDelay(200);
}

void loop() {
    int rotation = re.decodeRotaryEncoder();
    if (rotation != RE_NO_ACTION) {
        Serial.print("Rotation: ");
        switch (rotation) {
            case RE_TURN_CLOCKWISE:
                Serial.println("Clockwise");
                break;
            case RE_TURN_ANTICLOCKWISE:
                Serial.println("AntiClockwise");
                break;
        }
    }

    int state = re.decodeSwitch();
    if (state != RE_NO_ACTION) {
        Serial.print("Switch: ");
        switch (state) {
            case RE_SWITCH_UP:
                Serial.println("Up");
                break;
            case RE_SWITCH_DOWN:
                Serial.println("Down");
                break;
            case RE_SWITCH_LONG_HOLD:
                Serial.println("Long hold");
                break;
            case RE_SWITCH_CLICK:
                Serial.println("Click");
                break;
            case RE_SWITCH_LONG_CLICK:
                Serial.println("Long click");
                break;
            case RE_SWITCH_DOUBLE_CLICK:
                Serial.println("Double click");
                break;
        }
    }
}