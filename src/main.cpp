#include "main.h"

#define INPUT_DEBOUNCE 25
#define INPUT_LONG_PRESS 1000
long lastChange = 0;
long changeTime = 0;
int lastState = 0;
int state = 0;
bool changed = false;
bool longPressProcessed = false;

#define MAX_DEPTH 4

MENU(drinkMenu, "Drink", doNothing, noEvent, wrapStyle,
    OP("Get Wacked", poar, enterEvent),
    OP("Cuba Libre", poar, enterEvent),
    OP("Plain", poar, enterEvent)
);

int pumpSelected=0;
SELECT(pumpSelected, pumpSelect, "Pump", doNothing, noEvent, wrapStyle,
    VALUE("All", 0, doNothing, noEvent),
    VALUE("First", 1, doNothing, noEvent),
    VALUE("Second", 2, doNothing, noEvent)
);

int pumpTime = 0;
MENU(pumpMenu, "Control Pump", doNothing, noEvent, noStyle,
    SUBMENU(pumpSelect),
    FIELD(pumpTime, "Time", "sec", 0, 120, 10, 5, doNothing, noEvent, noStyle),
    OP("Run", poar, enterEvent),
    EXIT("Back")
);

MENU(settingsMenu, "Settings", doNothing, noEvent, noStyle,
    SUBMENU(pumpMenu),
    EXIT("Back")
);

MENU(mainMenu, "Barbot", doNothing, noEvent, noStyle,
    SUBMENU(drinkMenu),
    SUBMENU(settingsMenu)
);

// serialIn serial(Serial);
// MENU_INPUTS(in,&serial);
chainStream<0> in(NULL);

MENU_OUTPUTS(out, MAX_DEPTH,
    LIQUIDCRYSTAL_OUT(lcd, {0, 0, 16, 2}),
    NONE
);

NAVROOT(nav, mainMenu, MAX_DEPTH, in, out);

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

void readInputs() {
    uint32_t ms = millis();
    int newStatus = processAnalogInput(analogRead(0));
    if (ms - lastChange < INPUT_DEBOUNCE) {
        changed = false;
    } else {
        lastState = state;
        state = newStatus;
        changed = (state != lastState);
        if (changed) {
            lastChange = ms;
            longPressProcessed = false;
        }
    }
    changeTime = ms;

    if ((state == 5) && (changeTime - lastChange >= INPUT_LONG_PRESS) && (longPressProcessed == false)) {
        longPressProcessed = true;
        nav.doNav(navCmd(escCmd));
        return;
    }

    if ((changed == false) && (changeTime - lastChange < INPUT_LONG_PRESS))  {
        return;
    }

    switch (state) {
        case 0:
            nav.doNav(navCmd(noCmd));
            break;
        case 1:
            nav.doNav(navCmd(escCmd));
            // nav.doNav(navCmd(rightCmd));
            break;
        case 2:
            nav.doNav(navCmd(upCmd));
            break;
        case 3:
            nav.doNav(navCmd(downCmd));
            break;
        case 4:
            nav.doNav(navCmd(leftCmd));
            break;
        case 5:
            if (changeTime - lastChange < INPUT_LONG_PRESS) {
                nav.doNav(navCmd(enterCmd));
            }
            break;
    }
}

int processAnalogInput(int input) {
//  if (state > 1000) { nav.doNav(navCmd(noCmd)); return; }
//  if (state < 50)   { nav.doNav(navCmd(rightCmd)); return; }
//  if (state < 195)  { nav.doNav(navCmd(upCmd)); return; }
//  if (state < 380)  { nav.doNav(navCmd(downCmd)); return; }
//  if (state < 555)  { nav.doNav(navCmd(leftCmd)); return; }
//  if (state < 790)  { nav.doNav(navCmd(enterCmd)); return; }

//  escCmd

    if (input > 1000) { return 0; }
    if (input < 50)   { return 1; }
    if (input < 250)  { return 2; }
    if (input < 400)  { return 3; }
    if (input < 555)  { return 4; }
    if (input < 800)  { return 5; }

    return 0;
}

void setup() {
    Serial.begin(115200);

    pinMode(A0, INPUT);

    lcd.begin(16, 2);
    lcd.setCursor(0,0);
    lcd.print("BarBot");
    delay(1000);

    nav.showTitle = true;
    nav.canExit = false;
    nav.useAccel = true;
}

void loop() {
    readInputs();
    nav.doOutput();

    delay(100);
}
