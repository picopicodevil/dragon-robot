#include "mbed.h"
#include "TB6643.h"
#include "Digital_PWM_TB6643.h"
#include "RN4020.h"
#include "QEI.h"
#include "LineTrace.h"

DP_TB6643 wheel_right(p28, p22);
DP_TB6643 wheel_left(p27, p21);

TB6643 motor1(p26, p25);
TB6643 motor2(p24, p23);

LineTrace line_trace(p20, p19, p18);

// RN4020 rn4020_in(p9, p10);
// RN4020 rn4020_out(p13, p14);

QEI encoder1(p5, p6, NC, 512, QEI::X4_ENCODING);
QEI encoder2(p7, p8, NC, 512, QEI::X4_ENCODING);

DigitalIn limit_sw(p12, PullDown);

BufferedSerial pc(USBTX, USBRX, 115200);

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

int set_wheel();

// main() runs in its own thread in the OS
int main()
{
    led1 = 1;
    pc.write("RUN\r\n", 5);

    int preview_limit_sw_state = 0;

    while (true)
    {
        if (limit_sw == 1)
        {
            1;
        }

        encoder1.getRevolutions();

        encoder2.getRevolutions();

        if (1)
        {
            set_wheel();
        }

        led1 = !led1;
        ThisThread::sleep_for(10ms);
    }
}

int set_wheel()
{
    int value = line_trace.read();

    switch (value)
    {
    case 0:
        wheel_right.set_duty_cycle(0.00f);
        wheel_right.set_state(State::Brake);

        wheel_left.set_duty_cycle(0.00f);
        wheel_left.set_state(State::Brake);
        break;
    case 1:
        wheel_right.set_duty_cycle(0.50f);
        wheel_right.set_state(State::CCW);

        wheel_left.set_duty_cycle(0.20f);
        wheel_left.set_state(State::CCW);
        break;
    case 2:
        wheel_right.set_duty_cycle(0.20f);
        wheel_right.set_state(State::CCW);

        wheel_left.set_duty_cycle(0.50f);
        wheel_left.set_state(State::CCW);
        break;
    default:
        break;
    }

    wheel_right.set();
    wheel_left.set();

    return value;
}
