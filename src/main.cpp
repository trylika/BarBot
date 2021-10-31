#include "main.h"

void setup() {
    // Serial.begin(115200);
    Control::init();
}

void loop() {
    Control::readInputs();
    Control::processOutputs();

    delay(100);
}
