#ifndef DIGITAL_PWM_TB6643_H
#define DIGITAL_PWM_TB6643_H

#include "mbed.h"
#include "Motor.h"

class DP_TB6643 : public Motor
{
public:
    DP_TB6643(PinName IN1, PinName IN2);

    int set();

private:
    DigitalOut _IN1;
    PwmOut _IN2;
};

#endif
