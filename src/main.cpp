#include <Arduino.h>
#include "rotaryEncoder.h"

RotaryEncoder re = RotaryEncoder(PIN_A, PIN_B, PIN_SW);

void setup() {
    Serial.begin(BAUD_RATE);    
}

void loop() {
    int direction = re.decodeRotaryEncoder();
    if (direction != RE_NO_ACTION) {
        Serial.println(direction);
    }
    int switcher = re.decodeSwitch();
    if (switcher != RE_NO_ACTION) {
        Serial.println(switcher);
    }
}