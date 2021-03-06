#include "Control.h"

namespace Control {
    unsigned long inputLastChange = 0;
    unsigned long inputChangeDuration = 0;
    int inputLastState = 0;
    int inputState = 0;
    bool inputChanged = false;
    bool inputCmdLongPressProcessed = false;

    LiquidCrystal_PCF8574 lcd(0x27);

    unsigned int pumpSelected = PUMP_MANAGER_PUMP_ALL_ID;
    unsigned int pumpTime = 10;
    unsigned int pumpDirection = PUMP_MANAGER_FORWARD;
    unsigned int processingProgress = 0;

    MENU(drinkMenu, "Select drink", doNothing, noEvent, wrapStyle,
        OP("Get Wacked", poar, enterEvent),
        OP("Cuba Libre", poar, enterEvent),
        OP("Plain", poar, enterEvent)
    );

    SELECT(pumpSelected, pumpSelect, "Pump", doNothing, noEvent, wrapStyle,
        VALUE(PUMP_MANAGER_PUMP_ALL_NAME, PUMP_MANAGER_PUMP_ALL_ID, doNothing, noEvent),
        VALUE("1", PUMP_MANAGER_PUMP_ONE, doNothing, noEvent),
        VALUE("2", PUMP_MANAGER_PUMP_TWO, doNothing, noEvent)
    );

    SELECT(pumpDirection, pumpDirectionSelect, "Direction", doNothing, noEvent, wrapStyle,
        VALUE("Fwd", PUMP_MANAGER_FORWARD, doNothing, noEvent),
        VALUE("Bck", PUMP_MANAGER_BACKWARD, doNothing, noEvent)
    );

    MENU(pumpRun, "Pump run", doNothing, noEvent, noStyle,
        SUBMENU(pumpSelect),
        FIELD(pumpTime, "Time", "sec", 1, 120, 10, 1, doNothing, noEvent, noStyle),
        SUBMENU(pumpDirectionSelect),
        OP("Run", runForTimePump, enterEvent),
        EXIT("Back")
    );

    MENU(pumpFlush, "Pump flush", doNothing, noEvent, noStyle,
        SUBMENU(pumpSelect),
        SUBMENU(pumpDirectionSelect),
        OP("Flush", flushPump, enterEvent),
        EXIT("Back")
    );

    MENU(settingsMenu, "Settings", doNothing, noEvent, noStyle,
        SUBMENU(pumpRun),
        SUBMENU(pumpFlush),
        EXIT("Back")
    );

    MENU(mainMenu, "BARBOT ;)", doNothing, noEvent, noStyle,
        SUBMENU(drinkMenu),
        SUBMENU(settingsMenu)
    );

    // serialIn serial(Serial);
    // MENU_INPUTS(in,&serial);
    chainStream<0> in(NULL);

    MENU_OUTPUTS(out, MENU_MAX_DEPTH,
        LCD_OUT(lcd, {0, 0, 20, 4}),
        NONE
    );

    NAVROOT(nav, mainMenu, MENU_MAX_DEPTH, in, out);

    config navOptions('>', '-', defaultNavCodes, true);

    void init() {
        pinMode(INPUT_ANALOG_PIN, INPUT);

        lcd.begin(20, 4);
        lcd.setBacklight(255);
        lcd.home();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("BARBOT ;)");
        lcd.setCursor(0, 1);
        lcd.print("Hello!");
        delay(1000);

        nav.showTitle = true;
        nav.canExit = false;
        nav.useAccel = true;
        options = &navOptions;
    }

    void readInputs() {
        uint32_t ms = millis();
        int inputNewState = processAnalogInput(analogRead(0));
        if (ms - inputLastChange < INPUT_DEBOUNCE) {
            inputChanged = false;
        } else {
            inputLastState = inputState;
            inputState = inputNewState;
            inputChanged = (inputState != inputLastState);
            if (inputChanged) {
                inputLastChange = ms;
                inputCmdLongPressProcessed = false;
            }
        }
        inputChangeDuration = ms;

        if ((inputState == INPUT_TYPE_ENTER) && (inputChangeDuration - inputLastChange >= INPUT_LONG_PRESS) && (inputCmdLongPressProcessed == false)) {
            inputCmdLongPressProcessed = true;
            nav.doNav(navCmd(escCmd));
            return;
        }

        if ((inputChanged == false) && (inputChangeDuration - inputLastChange < INPUT_LONG_PRESS))  {
            return;
        }

        switch (inputState) {
            case INPUT_TYPE_ENTER:
                if (inputChangeDuration - inputLastChange < INPUT_LONG_PRESS) {
                    nav.doNav(navCmd(enterCmd));
                }
                break;
            case INPUT_TYPE_ESC:
                nav.doNav(navCmd(escCmd));
                break;
            case INPUT_TYPE_UP:
                nav.doNav(navCmd(downCmd));
                break;
            case INPUT_TYPE_DOWN:
                nav.doNav(navCmd(upCmd));
                break;
        }
    }

    int processAnalogInput(int input) {
        if (input > 800) { return INPUT_TYPE_NONE; }
        if (input < 50)   { return INPUT_TYPE_ESC; }
        if (input < 550)  { return INPUT_TYPE_UP; }
        if (input < 700)  { return INPUT_TYPE_DOWN; }
        if (input < 800)  { return INPUT_TYPE_ENTER; }

        return 0;
    }

    void processOutputs() {
        nav.doOutput();
    }

    result runForTimePump(eventMask e, prompt &item) {
        nav.idleOn(runningForTimePump);
        PumpManager::runForTime(pumpSelected, pumpTime, pumpDirection);
        nav.idleOff();

        return proceed;
    }

    result runningForTimePump(menuOut& o, idleEvent e) {
        if (e == idling) {
            o.clear();
            o.setCursor(0, 0);
            o.print("Running...");
            if (pumpSelected == PUMP_MANAGER_PUMP_ALL_ID) {
                o.print("ALL");
            } else {
                o.print(pumpSelected);
            }
            o.setCursor(0, 1);
            o.print("For ");
            o.print(pumpTime);
            o.print("sec");
        }

        if (e == idleEnd) {
            o.clear();
            o.setCursor(0, 0);
            o.print("Running...");
            o.setCursor(0, 1);
            o.print("Done");
            delay(1000);
        }

        return proceed;
    }

    result flushPump(eventMask e, prompt &item) {
        nav.idleOn(flushingPump);
        PumpManager::run(pumpSelected, pumpDirection);

        return proceed;
    }

    result flushingPump(menuOut& o, idleEvent e) {
        if (e == idling) {
            o.clear();
            o.setCursor(0, 0);
            o.print("Flushing...");
            if (pumpSelected == PUMP_MANAGER_PUMP_ALL_ID) {
                o.print("ALL");
            } else {
                o.print(pumpSelected);
            }
            o.setCursor(0, 1);
            o.print("[Select] to stop");
        }

        if (e == idleEnd) {
            PumpManager::stop(pumpSelected);

            o.clear();
            o.setCursor(0, 0);
            o.print("Flushing...");
            o.setCursor(0, 1);
            o.print("Done");
            delay(1000);
        }

        return proceed;
    }

    result poar(eventMask e, prompt &item) {
        nav.idleOn(processing);
        delay(2000);
        nav.idleOff();

        return proceed;
    }

    result processing(menuOut& o, idleEvent e) {
        if (e == idling) {
            o.clear();
            o.setCursor(0, 0);
            o.print("Processing...");
            o.setCursor(0, 1);
            o.print("Please wait");
        }

        if (e == idleEnd) {
            o.clear();
            o.setCursor(0, 0);
            o.print("Have a nice day");
            o.setCursor(0, 1);
            o.print(":)");
            delay(1000);
        }

        return proceed;
    }
}
