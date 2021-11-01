#include "main.h"

void setup() {
    // Serial.begin(115200);

    PumpManager::init();
    Control::init();
}

void loop() {
    Control::readInputs();
    Control::processOutputs();

    delay(100);
}
