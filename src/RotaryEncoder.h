#include <arduino.h>

#ifndef RotaryEncoder_h
#define RotaryEncoder_h

#define RE_HAPTIC_ERROR -1
#define RE_NO_ACTION 0
#define RE_SWITCH_UP 1
#define RE_SWITCH_DOWN 2
#define RE_SWITCH_LONG_HOLD 3
#define RE_SWITCH_CLICK 4
#define RE_SWITCH_LONG_CLICK 5
#define RE_SWITCH_DOUBLE_CLICK 6
#define RE_TURN_CLOCKWISE 7
#define RE_TURN_ANTICLOCKWISE 8

typedef void (*RotataryHandler)(const int rotation);
typedef void (*SwitchHandler)(const int state);
typedef void (*HapticHandler)(const int type);

class RotaryEncoder {
    static const uint8_t states[];

   private:
    int a, b, sw;
    unsigned long timer, dblClkTimer = 0;
    int lastRaw = 12;
    int lastState = 6;
    int lastSwitchState = 0;
    bool longHold = false;
    int event = RE_NO_ACTION;
    unsigned long hlfClkDelay = 133, dblClkDelay = 200, longClkDelay = 1000;
    RotataryHandler rotaryHandler = 0;
    SwitchHandler switchHandler = 0;

   public:
    RotaryEncoder(int a, int b);
    RotaryEncoder(int a, int b, int sw);
    int decodeRotaryEncoder();
    int decodeSwitch();
    void setRotationHandler(RotataryHandler handler);
    void setSwitchHandler(SwitchHandler handler);
    bool setDoubleClickDelay(unsigned int delay);
    bool setLockClickDelay(unsigned int delay);
};

#endif