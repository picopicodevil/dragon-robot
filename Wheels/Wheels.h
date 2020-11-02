#ifndef WHEELS_H
#define WHEELS_H

#include "mbed.h"
#include "TB6643.h"

enum class WheelSide
{
    Right = 0,
    Left,
};

class Wheels
{
public:
    Wheels(PinName right_IN1, PinName right_IN2, PinName left_IN1, PinName left_IN2);

    void straight();
    void back();
    void turn_right();
    void turn_left();

    void stop();
    void brake();

private:
    TB6643 _right_wheel;
    TB6643 _left_wheel;
};

#endif