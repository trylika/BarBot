#include "main.h"

void setup() {
    // Serial.begin(115200);
    Control::init();

    Pump* pumpOne = new Pump(38, 36, 37);
}

void loop() {
    Control::readInputs();
    Control::processOutputs();

    delay(100);
}
