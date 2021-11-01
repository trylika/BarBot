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

    }

    void runSeries(int pumpId, int seconds) {
        if (pumpId == PUMP_MANAGER_PUMP_ALL_ID) {
            for (int i = 0; i < PUMP_MANAGER_PUMP_COUNT; i++) {
                runSinglePump(i, seconds);
            }
        } else {
            for (int i = 0; i < PUMP_MANAGER_PUMP_COUNT; i++) {
                if (pumpIds[i] == pumpId) {
                    runSinglePump(i, seconds);
                }
            }
        }
    }

    void runSinglePump(int pumpId, int seconds) {
        pumps[pumpId].forward();
        delay(seconds * 1000);
        pumps[pumpId].stop();
    }
}
