#include "Pump.h"

Pump::Pump(
    uint8_t pinForward,
    uint8_t pinBackward,
    uint8_t pinSpeed,
    int speedTrimLow,
    int speedTrimHigh
) {
    this->pinForward = pinForward;
    this->pinBackward = pinBackward;

    this->pinSpeed = pinSpeed;
    this->speedTrimLow = speedTrimLow;
    this->speedTrimHigh = speedTrimHigh;

    pinMode(this->pinForward, OUTPUT);
    pinMode(this->pinBackward, OUTPUT);
    if (this->pinSpeed != PUMP_NO_SPEED) {
        pinMode(this->pinSpeed, OUTPUT);
    }

    this->motorStop();
}

void Pump::forward() {
    this->setSpeed(PUMP_SPEED_MAX);
}

void Pump::backward() {
    this->setSpeed(PUMP_SPEED_MIN);
}

void Pump::setSpeed(int speed) {
    this->speed = constrain(speed, PUMP_SPEED_MIN, PUMP_SPEED_MAX); // Constrain speed

    if (this->speed > PUMP_SPEED_ZERO) {
        this->motorSpeed(this->constrainMotorSpeed(this->speed));
        this->motorForward();
    } else if (this->speed < PUMP_SPEED_ZERO) {
        this->motorSpeed(this->constrainMotorSpeed(this->speed));
        this->motorBackward();
    } else {
        this->motorStop();
    }
}

int Pump::getSpeed() {
    return this->speed;
}

void Pump::stop() {
    this->speed = PUMP_SPEED_ZERO;
    this->motorStop();
}

void Pump::brake() {
    this->speed = PUMP_SPEED_ZERO;
    this->motorBrake();
}

void Pump::motorForward() {
    digitalWrite(this->pinForward, HIGH); // On
    digitalWrite(this->pinBackward, LOW); // Off
}

void Pump::motorBackward() {
    digitalWrite(this->pinForward, LOW); // Off
    digitalWrite(this->pinBackward, HIGH); // On
}

void Pump::motorStop() {
    this->motorSpeed(PUMP_SPEED_ZERO);

    digitalWrite(this->pinForward, LOW); // Off
    digitalWrite(this->pinBackward, LOW); // Off
}

void Pump::motorBrake() {
    this->motorSpeed(PUMP_SPEED_ZERO);

    digitalWrite(this->pinForward, HIGH); // On
    digitalWrite(this->pinBackward, HIGH); // On
}

int Pump::constrainMotorSpeed(int speed) {
    int pwm = map(abs(speed), PUMP_SPEED_ZERO, PUMP_SPEED_MAX, this->speedTrimLow, this->speedTrimHigh); // Map speed to trimmed speed
    pwm = constrain(pwm, PUMP_PWM_MIN, PUMP_PWM_MAX); // Constrain possible PWM speed

    return pwm;
}

void Pump::motorSpeed(int speed) {
    if (this->pinSpeed != PUMP_NO_SPEED) {
        analogWrite(this->pinSpeed, speed);
    }
}
