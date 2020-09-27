/*
  *This software includes the work that is distributed in the Apache License 2.0
  */

#include "Motor.h"

Motor::Motor()
{
    init();
}

void Motor::init()
{
    _duty_cycle = 0.0f;
    _state = (State)default_state;
}

void Motor::set_duty_cycle(float value)
{
    if ((0.00f <= value) && (value <= 1.00f))
    {
        _duty_cycle = value;
    }
}

float Motor::get_duty_cycle() const
{
    return _duty_cycle;
}

void Motor::set_state(State type)
{
    if ((State::Free <= type) && (type < State::TotalState))
    {
        _state = type;
    }
}

State Motor::get_state() const
{
    return _state;
}

void Motor::set_pulse_period(float s)
{
    if ((min_pulse_period <= s) && (s <= max_pulse_period))
    {
        _pulse_period = s;
    }
}

float Motor::get_pulse_period() const
{
    return _pulse_period;
}

void Motor::set_frequency(float hz)
{
    set_pulse_period(1.0f / hz);
}

float Motor::get_frequency() const
{
    return 1 / get_pulse_period();
}
