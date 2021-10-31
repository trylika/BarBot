#ifndef main_h
#define main_h

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <menu.h>
#include <menuIO/chainStream.h>
#include <menuIO/liquidCrystalOut.h>
#include <menuIO/serialIn.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void readInputs();
int processAnalogInput(int input);
bool blink(int timeOn,int timeOff);
void setup();
void loop();
result poar(eventMask e, prompt &item);
result processing(menuOut& o, idleEvent e);

#endif
