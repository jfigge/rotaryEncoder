#include "RotaryEncoder.h"

const uint8_t RotaryEncoder::states[] = {0,4,1,0,2,1,1,0,2,3,1,2,2,3,3,0,5,4,4,0,5,4,6,5,5,6,6,0};

RotaryEncoder::RotaryEncoder(int a, int b, int sw) {
  this->a = a;
  this->b = b;
  this->sw = sw; 

  pinMode(a, INPUT);
  pinMode(b, INPUT);
  pinMode(sw, INPUT_PULLUP);
}

RotaryEncoder::RotaryEncoder(int a, int b, int sw, int hapticPin) : RotaryEncoder(a, b, sw) {
  this->h = hapticPin;
  pinMode(h, INPUT);
}

void RotaryEncoder::setRotationHandler(RotataryHandler handler) {
  this->rotaryHandler = handler;
}
void RotaryEncoder::setSwitchHandler(SwitchHandler handler) {
  this->switchHandler = handler;
}
void RotaryEncoder::setHapticHandler(HapticHandler handler) {
    this->hapticHandler = handler;
}

int RotaryEncoder::decodeRotaryEncoder() {
  uint8_t raw = PIN_PORT & AB_BYTE;
  int result = 0;
  if (lastRaw != raw) {
    lastRaw = raw;
    uint8_t state = states[(raw >> AB_SHIFT) | (lastState << 2)];
    if (state == 0) {
      if (lastState == 6) {
        result = RE_CLOCKWISE;
      } else if (lastState == 3) {
        result = RE_ANTICLOCKWISE;
      }
      if (rotaryHandler != 0) {
        rotaryHandler(result);
      }
    }
    lastState = state;
  }
  return result;
}

int RotaryEncoder::decodeSwitch() {
  int result = RE_SWITCH_UP;
  static int lastResult = RE_SWITCH_UP;
  uint8_t switchState = PIN_PORT & SW_BYTE;
  if (lastSwitchState != switchState) {
    lastSwitchState = switchState;
    if (!switchState) {
      timer = millis();
      result = RE_SWITCH_DOWN;
      //beep(1);
    } else {
      timer = millis() - timer;
      result = timer > 1000 ? RE_SWITCH_LONG_RELEASE : RE_SWITCH_SHORT_RELEASE;
      //beep(2);
    }
  } else if (!switchState) {
    result = millis() - timer > 1000 ? RE_SWITCH_LONG_HOLD : RE_SWITCH_SHORT_HOLD;
  }

  if (result != lastResult) {
    lastResult = result;
   // if (switchHandler != 0) {
   //   switchHandler(result);
   // }
  }
  return result;
}

// void RotaryEncoder::beep(int type) {
//   if (beeperPin != -1) {
//     if (beeperHandler != 0) {
//       beeperHandler(b);
//     } else {
//         Serial.println(type);
//       switch (type) {
//       case RE_SWITCH_UP:
//       case RE_SWITCH_DOWN:
//         tone(PIN_BEEPER, 400);
//         delay(2);
//         break;
//       case 3: // Error
//         tone(PIN_BEEPER, 1000);
//         delay(10);
//         break;
//       default: // Menu
//         tone(PIN_BEEPER, 250);
//         delay(1);
//         tone(PIN_BEEPER, 250);
//         delay(1);
//         tone(PIN_BEEPER, 250);
//         delay(1);
//       }
//       noTone(PIN_BEEPER);
//     }
//   }
// }