#ifndef DIGITAL_PWM_WHEELS_H
#define DIGITAL_PWM_WHEELS_H

#include "mbed.h"
#include "Wheels.h"
#include "Digital_PWM_TB6643.h"

class DP_Wheels
{
public:
    DP_Wheels(PinName right_IN1, PinName right_IN2, PinName left_IN1, PinName left_IN2);

    void straight();
    void back();
    void turn_right();
    void turn_left();

    void stop();
    void brake();

private:
    DP_TB6643 _right_wheel;
    DP_TB6643 _left_wheel;
};

#endif