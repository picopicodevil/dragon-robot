#ifndef TB6643_H
#define TB6643_H

#include "mbed.h"
#include "Motor.h"

class TB6643 : public Motor
{
public:
    TB6643(PinName IN1, PinName IN2);

    int set(Motor &motor);

private:
    DigitalOut _IN1;
    DigitalOut _IN2;
};

#endif
