#ifndef LINE_TRACE_H
#define LINE_TRACE_H

#include "mbed.h"
#include "Motor.h"

enum class Color
{
    Brack,
    White,
};

enum class Place
{
    Base,
    Line,
};

class LineTrace
{
public:
    enum TracePosition
    {
        Right = 0,
        Center = 1,
        Left = 2,
    };

    LineTrace(PinName left, PinName center, PinName right);

    int read();
    void set_base_color(Color color);

    State get_left_state();
    float get_left_duty_cycle();
    State get_right_state();
    float get_right_duty_cycle();

private:
    AnalogIn _right;
    AnalogIn _center;
    AnalogIn _left;

    Color _color[3];
    Place _place[3];
    Motor _motor[3]; // TracePositionの列挙型を使いたいから

    Color _base_color;

    Color get_color(AnalogIn pin);
    Place get_place(AnalogIn pin);

    static constexpr float threshold = 0.7f;
};

#endif
