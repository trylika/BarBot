#include "Control.h"

namespace Control {
    unsigned long inputLastChange = 0;
    unsigned long inputChangeDuration = 0;
    int inputLastState = 0;
    int inputState = 0;
    bool inputChanged = false;
    bool inputCmdLongPressProcessed = false;

    LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

    unsigned int processingProgress = 0;

    MENU(drinkMenu, "Select drink", doNothing, noEvent, wrapStyle,
        OP("Get Wacked", poar, enterEvent),
        OP("Cuba Libre", poar, enterEvent),
        OP("Plain", poar, enterEvent)
    );

    unsigned int pumpSelected = 0;
    SELECT(pumpSelected, pumpSelect, "Pump", doNothing, noEvent, wrapStyle,
        VALUE("All", 0, doNothing, noEvent),
        VALUE("First", 1, doNothing, noEvent),
        VALUE("Second", 2, doNothing, noEvent)
    );

    unsigned int pumpTime = 10;
    MENU(pumpMenu, "Pump control", doNothing, noEvent, noStyle,
        SUBMENU(pumpSelect),
        FIELD(pumpTime, "Time", "sec", 0, 120, 10, 5, doNothing, noEvent, noStyle),
        OP("Run", poar, enterEvent),
        EXIT("Back")
    );

    MENU(settingsMenu, "Settings", doNothing, noEvent, noStyle,
        SUBMENU(pumpMenu),
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
        LIQUIDCRYSTAL_OUT(lcd, {0, 0, 16, 2}),
        NONE
    );

    NAVROOT(nav, mainMenu, MENU_MAX_DEPTH, in, out);

    config navOptions('>', '-', defaultNavCodes, true);

    void init() {
        pinMode(INPUT_ANALOG_PIN, INPUT);

        lcd.begin(16, 2);
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

        if ((inputState == INPUT_TYPE_SELECT) && (inputChangeDuration - inputLastChange >= INPUT_LONG_PRESS) && (inputCmdLongPressProcessed == false)) {
            inputCmdLongPressProcessed = true;
            nav.doNav(navCmd(escCmd));
            return;
        }

        if ((inputChanged == false) && (inputChangeDuration - inputLastChange < INPUT_LONG_PRESS))  {
            return;
        }

        switch (inputState) {
            case INPUT_TYPE_SELECT:
            case INPUT_TYPE_LEFT:
                if (inputChangeDuration - inputLastChange < INPUT_LONG_PRESS) {
                    nav.doNav(navCmd(enterCmd));
                }
                break;
            case INPUT_TYPE_RIGHT:
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
        if (input > 1000) { return INPUT_TYPE_NONE; }
        if (input < 50)   { return INPUT_TYPE_RIGHT; }
        if (input < 250)  { return INPUT_TYPE_UP; }
        if (input < 400)  { return INPUT_TYPE_DOWN; }
        if (input < 555)  { return INPUT_TYPE_LEFT; }
        if (input < 800)  { return INPUT_TYPE_SELECT; }

        return 0;
    }

    void processOutputs() {
        nav.doOutput();
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

    result poar(eventMask e, prompt &item) {
        nav.idleOn(processing);
        delay(2000);
        nav.idleOff();

        return proceed;
    }
}
