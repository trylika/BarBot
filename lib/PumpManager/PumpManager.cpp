#include "PumpManager.h"

namespace PumpManager {
    const int pumpIds[PUMP_MANAGER_PUMP_COUNT] = {
        PUMP_MANAGER_PUMP_ONE,
        PUMP_MANAGER_PUMP_TWO,
    };

    Pump pumps[PUMP_MANAGER_PUMP_COUNT] = {
        Pump(52, 53),
        Pump(50, 51),
    };

    void init() {
        for (int i = 0; i < PUMP_MANAGER_PUMP_COUNT; i++) {
            pumps[i].begin();
        }
    }

    void runForTime(int pumpId, int seconds, int direction) {
        if (pumpId == PUMP_MANAGER_PUMP_ALL_ID) {
            runForTimeAll(seconds, direction);
        } else {
            for (int i = 0; i < PUMP_MANAGER_PUMP_COUNT; i++) {
                if (pumpIds[i] == pumpId) {
                    if (direction == PUMP_MANAGER_FORWARD) {
                        pumps[i].forward();
                    } else if (direction == PUMP_MANAGER_BACKWARD) {
                        pumps[i].backward();
                    }

                    delay(seconds * 1000);

                    pumps[i].stop();

                    return;
                }
            }
        }
    }

    void runForTimeAll(int seconds, int direction) {
        if (direction == PUMP_MANAGER_FORWARD) {
            for (int i = 0; i < PUMP_MANAGER_PUMP_COUNT; i++) {
                pumps[i].forward();
            }

        } else if (direction == PUMP_MANAGER_BACKWARD) {
            for (int i = 0; i < PUMP_MANAGER_PUMP_COUNT; i++) {
                pumps[i].backward();
            }
        }

        delay(seconds * 1000);

        for (int i = 0; i < PUMP_MANAGER_PUMP_COUNT; i++) {
            pumps[i].stop();
        }
    }

    void run(int pumpId, int direction) {
        if (pumpId == PUMP_MANAGER_PUMP_ALL_ID) {
            runAll(direction);
        } else {
            for (int i = 0; i < PUMP_MANAGER_PUMP_COUNT; i++) {
                if (pumpIds[i] == pumpId) {
                    if (direction == PUMP_MANAGER_FORWARD) {
                        pumps[i].forward();
                    } else if (direction == PUMP_MANAGER_BACKWARD) {
                        pumps[i].backward();
                    }
                    return;
                }
            }
        }
    }

    void runAll(int direction) {
        if (direction == PUMP_MANAGER_FORWARD) {
            for (int i = 0; i < PUMP_MANAGER_PUMP_COUNT; i++) {
                pumps[i].forward();
            }

        } else if (direction == PUMP_MANAGER_BACKWARD) {
            for (int i = 0; i < PUMP_MANAGER_PUMP_COUNT; i++) {
                pumps[i].backward();
            }
        }
    }

    void stop(int pumpId) {
        if (pumpId == PUMP_MANAGER_PUMP_ALL_ID) {
            stopAll();
        } else {
            for (int i = 0; i < PUMP_MANAGER_PUMP_COUNT; i++) {
                if (pumpIds[i] == pumpId) {
                    pumps[i].stop();
                    return;
                }
            }
        }
    }

    void stopAll() {
        for (int i = 0; i < PUMP_MANAGER_PUMP_COUNT; i++) {
            pumps[i].stop();
        }
    }
}
