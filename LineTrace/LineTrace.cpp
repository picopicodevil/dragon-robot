#include "LineTrace.h"
#include "mbed.h"
#include "Motor.h"

LineTrace::LineTrace(PinName left, PinName center, PinName right)
    : _left(left), _center(center), _right(right)
{
    _base_color = Color::Brack;
}

int LineTrace::read()
{
    _place[TracePosition::Left] = get_place(_left);
    _place[TracePosition::Center] = get_place(_center);
    _place[TracePosition::Right] = get_place(_right);

    if (_place[TracePosition::Center] == Place::Line)
    {
        _motor[Left].set_state(State::CCW);
        _motor[Left].set_duty_cycle(0.50f);

        _motor[Right].set_state(State::CW);
        _motor[Right].set_duty_cycle(0.20f);

        return 1;
    }
    else if (_place[TracePosition::Center] == Place::Base)
    {
        _motor[Left].set_state(State::CCW);
        _motor[Left].set_duty_cycle(0.20f);

        _motor[Right].set_state(State::CW);
        _motor[Right].set_duty_cycle(0.50f);

        return 2;
    }

    return -1;
}

void LineTrace::set_base_color(Color color)
{
    _base_color = color;
}

State LineTrace::get_left_state()
{
    return _motor[Left].get_state();
}

float LineTrace::get_left_duty_cycle()
{
    return _motor[Left].get_duty_cycle();
}

State LineTrace::get_right_state()
{
    return _motor[Right].get_state();
}

float LineTrace::get_right_duty_cycle()
{
    return _motor[Right].get_duty_cycle();
}

Color LineTrace::get_color(AnalogIn pin)
{
    float value = pin.read();

    if (value >= threshold)
    {
        return Color::Brack;
    }
    else
    {
        return Color::White;
    }
}

Place LineTrace::get_place(AnalogIn pin)
{
    Color color = get_color(pin);

    if (color == _base_color)
        return Place::Base;
    else
        return Place::Line;
}
