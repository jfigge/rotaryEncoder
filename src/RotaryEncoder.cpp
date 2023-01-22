#include "RotaryEncoder.h"

const uint8_t RotaryEncoder::states[] = {0, 4, 1, 0, 2, 1, 1, 0, 2, 3,
                                         1, 2, 2, 3, 3, 0, 5, 4, 4, 0,
                                         5, 4, 6, 5, 5, 6, 6, 0};

RotaryEncoder::RotaryEncoder(int a, int b) {
    this->a = a;
    this->b = b;
    this->sw = -1;
    pinMode(a, INPUT);
    pinMode(b, INPUT);
}
RotaryEncoder::RotaryEncoder(int a, int b, int sw) : RotaryEncoder(a, b) {
    this->sw = sw;
    pinMode(sw, INPUT_PULLUP);
    lastSwitchState = PIN_PORT & SW_BYTE;
}

void RotaryEncoder::setRotationHandler(RotataryHandler handler) {
    this->rotaryHandler = handler;
}
void RotaryEncoder::setSwitchHandler(SwitchHandler handler) {
    this->switchHandler = handler;
}

bool RotaryEncoder::setDoubleClickDelay(unsigned int delay) {
    if (delay > 0 && delay < longClkDelay) {
        dblClkDelay = delay;
        return true;
    }
    return false;
}
bool RotaryEncoder::setLockClickDelay(unsigned int delay) {
    if (delay > dblClkDelay) {
        longClkDelay = delay;
        return true;
    }
    return false;
}

int RotaryEncoder::decodeRotaryEncoder() {
    uint8_t raw = PIN_PORT & AB_BYTE;
    int result = RE_NO_ACTION;
    if (lastRaw != raw) {
        lastRaw = raw;
        uint8_t state = states[(raw >> AB_SHIFT) | (lastState << 2)];
        if (state == 0) {
            if (lastState == 6) {
                result = RE_TURN_CLOCKWISE;
            } else if (lastState == 3) {
                result = RE_TURN_ANTICLOCKWISE;
            }
            if (result != 0 && rotaryHandler != 0) {
                rotaryHandler(result);
            }
        }
        lastState = state;
    }
    return result;
}
int RotaryEncoder::decodeSwitch() {
    int result = RE_NO_ACTION;
    if (event != RE_NO_ACTION || sw == -1) {
        result = event;
        event = RE_NO_ACTION;
    } else {
        uint8_t switchState = PIN_PORT & SW_BYTE;
        if (lastSwitchState != switchState) {
            lastSwitchState = switchState;
            if (!switchState) {
                result = RE_SWITCH_DOWN;
                timer = millis();
            } else {
                result = RE_SWITCH_UP;
                if (longHold) {
                    event = RE_SWITCH_LONG_CLICK;
                    longHold = false;
                } else if (dblClkTimer == 0) {
                    dblClkTimer = millis();
                } else {
                    event = millis() - dblClkTimer < dblClkDelay ? RE_SWITCH_DOUBLE_CLICK : RE_SWITCH_CLICK;
                    dblClkTimer = 0;
                }
            }
        } else if (!switchState && !longHold && millis() - timer > longClkDelay) {
            result = RE_SWITCH_LONG_HOLD;
            longHold = true;
        } else if (dblClkTimer != 0 && millis() - dblClkTimer >= dblClkDelay) {
            result = RE_SWITCH_CLICK;
            dblClkTimer = 0;
        }
    }
    if (result != RE_NO_ACTION && switchHandler != 0) {
        switchHandler(result);
    }
    return result;
}