#include "Wheels.h"
#include "mbed.h"
#include "TB6643.h"

Wheels::Wheels(PinName right_IN1, PinName right_IN2, PinName left_IN1, PinName left_IN2)
    : _right_wheel(right_IN1, right_IN2), _left_wheel(left_IN1, left_IN2)
{
}

void Wheels::straight()
{
    _right_wheel.set_duty_cycle(0.50f);
    _right_wheel.set_state(State::CCW);

    _left_wheel.set_duty_cycle(0.50f);
    _left_wheel.set_state(State::CW);

    _right_wheel.set();
    _left_wheel.set();
}

void Wheels::back()
{
    _right_wheel.set_duty_cycle(0.50f);
    _right_wheel.set_state(State::CW);

    _left_wheel.set_duty_cycle(0.50f);
    _left_wheel.set_state(State::CCW);

    _right_wheel.set();
    _left_wheel.set();
}

void Wheels::turn_right()
{
    _right_wheel.set_duty_cycle(0.00f);
    _right_wheel.set_state(State::Brake);

    _left_wheel.set_duty_cycle(0.50f);
    _left_wheel.set_state(State::CCW);

    _right_wheel.set();
    _left_wheel.set();
}

void Wheels::turn_left()
{
    _right_wheel.set_duty_cycle(0.50f);
    _right_wheel.set_state(State::CCW);

    _left_wheel.set_duty_cycle(0.00f);
    _left_wheel.set_state(State::Brake);

    _right_wheel.set();
    _left_wheel.set();
}

void Wheels::stop()
{
    _right_wheel.set_duty_cycle(0.00f);
    _right_wheel.set_state(State::Free);

    _left_wheel.set_duty_cycle(0.00f);
    _left_wheel.set_state(State::Free);

    _right_wheel.set();
    _left_wheel.set();
}

void Wheels::brake()
{
    _right_wheel.set_duty_cycle(0.00f);
    _right_wheel.set_state(State::Brake);

    _left_wheel.set_duty_cycle(0.00f);
    _left_wheel.set_state(State::Brake);

    _right_wheel.set();
    _left_wheel.set();
}