#ifndef pump_manager_h
#define pump_manager_h

#include <Arduino.h>
#include <Pump.h>

#define PUMP_MANAGER_PUMP_COUNT 2
#define PUMP_MANAGER_PUMP_ALL_NAME "All"
#define PUMP_MANAGER_PUMP_ALL_ID 0
#define PUMP_MANAGER_PUMP_ONE 1
#define PUMP_MANAGER_PUMP_TWO 2

namespace PumpManager {
    void init();
    void runSeries(int pumpId, int seconds);
    void runSinglePump(int pumpId, int seconds);
}

#endif
