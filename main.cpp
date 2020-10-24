#include "mbed.h"
#include "TB6643.h"
#include "Digital_PWM_TB6643.h"
#include "RN4020.h"
#include "QEI.h"
#include "LineTrace.h"

// RN4020 rn4020_in(p9, p10);
// RN4020 rn4020_out(p13, p14);

BufferedSerial pc(USBTX, USBRX, 115200);

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

void link();
void ball_screw();
int set_wheel();

// main() runs in its own thread in the OS
int main()
{
    led1 = 1;
    pc.write("RUN\r\n", 5);

    // while (1)
    // {
    //     if (rn4020_in.readable())
    //     {
    //         char input;
    //         rn4020_in.read(&input, 1);

    //         if (input == 0x50)
    //             break;
    //     }
    // }

    Thread link_thread;
    Thread ball_screw_thread;

    link_thread.start(link);
    ball_screw_thread.start(ball_screw);

    while (true)
    {
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
    DP_TB6643 wheel_right(p28, p22);
    DP_TB6643 wheel_left(p27, p21);

    LineTrace line_trace(p20, p19, p18);
    line_trace.set_base_color(Color::White);
    int value = line_trace.read();

    switch (value)
    {
    case 0:
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

void link()
{
    TB6643 motor(p26, p25);
    DigitalIn sw(p12, PullDown);
    QEI encoder(p5, p6, NC, 512, QEI::X4_ENCODING);

    Timer timer;
    timer.start();

    const float cycle_time = 5.0f;
    const float reserve_time = 1.0f;

    while (1)
    {
        float elapsed_time = std::chrono::duration<float>{timer.elapsed_time()}.count();

        if (sw == 1)
        {
            if (elapsed_time > (cycle_time - 2.0f))
            {
                motor.set_duty_cycle(0.0f);
                motor.set_state(State::Brake);
                motor.set();
            }
            else if (elapsed_time > (cycle_time + reserve_time))
            {
                motor.set_duty_cycle(0.50f);
                motor.set_state(State::CW);
                motor.set();
            }
        }
        else if (sw == 0)
        {
            motor.set_duty_cycle(0.50f);
            motor.set_state(State::CW);
            motor.set();
        }

        ThisThread::sleep_for(10ms);
    }
}

void ball_screw()
{
    TB6643 motor(p24, p23);
    DigitalIn sw(p14, PullDown);
    QEI encoder(p7, p8, NC, 512, QEI::X4_ENCODING);

    Timer timer;
    timer.start();

    const float cycle_time = 5.0f;
    const float reserve_time = 1.0f;

    bool is_rise = false;

    while (1)
    {
        float elapsed_time = std::chrono::duration<float>{timer.elapsed_time()}.count();

        if (sw == 1)
        {
            encoder.reset();
                
            timer.stop();

            if (elapsed_time > (cycle_time - 2.0f))
            {
                motor.set_duty_cycle(0.00f);
                motor.set_state(State::Brake);
                motor.set();
            }
            else if (elapsed_time > (cycle_time + reserve_time))
            {
                is_rise = true;
                
                timer.reset();
                timer.start();

                motor.set_duty_cycle(0.50f);
                motor.set_state(State::CW);
                motor.set();
            }
        }
        else if (is_rise == true)
        {
            motor.set_duty_cycle(0.50f);
            motor.set_state(State::CW);
            motor.set();
        }
        else if (is_rise == false)
        {
            motor.set_duty_cycle(0.50f);
            motor.set_state(State::CCW);
            motor.set();
        }

        if (encoder.getRevolutions() > 1500.0f)
        {
            encoder.reset();
            is_rise = false;
        }

        ThisThread::sleep_for(10ms);
    }
}
