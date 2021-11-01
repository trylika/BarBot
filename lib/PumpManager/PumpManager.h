#ifndef pump_manager_h
#define pump_manager_h

#include <Arduino.h>
#include <Pump.h>

#define PUMP_MANAGER_PUMP_COUNT 2
#define PUMP_MANAGER_PUMP_ALL_NAME "All"
#define PUMP_MANAGER_PUMP_ALL_ID 0
#define PUMP_MANAGER_PUMP_ONE 1
#define PUMP_MANAGER_PUMP_TWO 2
#define PUMP_MANAGER_FORWARD 0
#define PUMP_MANAGER_BACKWARD 1

namespace PumpManager {
    void init();

    void runForTime(int pumpId, int seconds, int direction);
    void runForTimeAll(int seconds, int direction);

    void run(int pumpId, int direction);
    void runAll(int direction);

    void stop(int pumpId);
    void stopAll();
}

#endif
