#include <arduino.h>

#ifndef RotaryEncoder_h
#define RotaryEncoder_h

#define RE_CLOCKWISE      1
#define RE_ANTICLOCKWISE -1

#define RE_SWITCH_UP             0
#define RE_SWITCH_DOWN           1
#define RE_SWITCH_SHORT_HOLD     2
#define RE_SWITCH_LONG_HOLD      3
#define RE_SWITCH_SHORT_RELEASE  4
#define RE_SWITCH_LONG_RELEASE   5

typedef void (*RotataryHandler) (const int rotation);
typedef void (*SwitchHandler) (const int state);

class RotaryEncoder {
    static const uint8_t states[];
    private:
        int a,b,sw;
        RotataryHandler rotaryHandler = 0;
        SwitchHandler switchHandler = 0;
        uint8_t lastRaw = 12;
        uint8_t lastState = 6;
        long timer;
        uint8_t lastSwitchState = -1;

    public:
        RotaryEncoder(int a, int b, int sw);
        int decodeRotaryEncoder();
        int decodeSwitch();
        void setRotationHandler(RotataryHandler handler);
        void setSwitchHandler(SwitchHandler handler);
};

#endif