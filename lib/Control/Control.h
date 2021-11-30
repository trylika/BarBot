#ifndef control_h
#define control_h

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <menu.h>
#include <menuIO/chainStream.h>
#include <menuIO/PCF8574Out.h>
#include <menuIO/serialIn.h>
#include <PumpManager.h>

#define INPUT_ANALOG_PIN A0

#define INPUT_DEBOUNCE 25
#define INPUT_LONG_PRESS 1000

#define INPUT_TYPE_NONE 0
#define INPUT_TYPE_ENTER 1
#define INPUT_TYPE_ESC 2
#define INPUT_TYPE_UP 3
#define INPUT_TYPE_DOWN 4

#define MENU_MAX_DEPTH 4

namespace Control {
    extern unsigned int processingProgress;

    void init();
    void readInputs();
    int processAnalogInput(int input);
    void processOutputs();

    result runForTimePump(eventMask e, prompt &item);
    result runningForTimePump(menuOut& o, idleEvent e);

    result flushPump(eventMask e, prompt &item);
    result flushingPump(menuOut& o, idleEvent e);

    result poar(eventMask e, prompt &item);
    result processing(menuOut& o, idleEvent e);
}

#endif
