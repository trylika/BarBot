#ifndef control_h
#define control_h

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <menu.h>
#include <menuIO/chainStream.h>
#include <menuIO/liquidCrystalOut.h>
#include <menuIO/serialIn.h>

#define INPUT_ANALOG_PIN A0

#define INPUT_DEBOUNCE 25
#define INPUT_LONG_PRESS 1000

#define INPUT_TYPE_NONE 0
#define INPUT_TYPE_SELECT 1
#define INPUT_TYPE_LEFT 2
#define INPUT_TYPE_RIGHT 3
#define INPUT_TYPE_UP 4
#define INPUT_TYPE_DOWN 5

#define MENU_MAX_DEPTH 4

namespace Control {
    extern unsigned int pumpSelected;
    extern unsigned int pumpTime;
    extern unsigned int processingProgress;

    void init();
    void readInputs();
    int processAnalogInput(int input);
    void processOutputs();

    result poar(eventMask e, prompt &item);
    result processing(menuOut& o, idleEvent e);
}

#endif
