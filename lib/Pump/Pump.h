#ifndef Pump_h
#define Pump_h

#include <Arduino.h>

#define PUMP_SPEED_MIN -255
#define PUMP_SPEED_ZERO 0
#define PUMP_SPEED_MAX 255
#define PUMP_NO_SPEED 0
#define PUMP_PWM_MIN 0
#define PUMP_PWM_MAX 255

class Pump{
    public:
        Pump(
            uint8_t pinForward,
            uint8_t pinBackward,
            uint8_t pinSpeed = PUMP_NO_SPEED,
            int speedTrimLow = PUMP_SPEED_ZERO,
            int speedTrimHigh = PUMP_SPEED_MAX
        );

        void forward();
        void backward();
        void setSpeed(int speed);
        int getSpeed();
        void stop();
        void brake();
    private:
        uint8_t pinForward;
        uint8_t pinBackward;
        uint8_t pinSpeed;
        int speedTrimLow;
        int speedTrimHigh;

        int speed = PUMP_SPEED_ZERO;

        void motorForward();
        void motorBackward();
        void motorStop();
        void motorBrake();
        int constrainMotorSpeed(int speed);
        void motorSpeed(int speed);
};

#endif
