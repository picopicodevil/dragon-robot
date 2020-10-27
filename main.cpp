#include "mbed.h"
#include "TB6643.h"
#include "Digital_PWM_TB6643.h"
#include "PS3_SBDBT5V.h"
#include "RN4020.h"
#include "QEI.h"
#include "LineTrace.h"
#include "UniqueValue.h"

#if ROBOT_NUMBER == 1
PS3 dualShock3(p14);
#else
RN4020 rn4020_in(p13, p14);
#endif
RN4020 rn4020_out(p9, p10);

BufferedSerial pc(USBTX, USBRX, 115200);

DigitalOut led3(LED3);
DigitalOut led4(LED4);

void link();
void ball_screw();
void wheel();

// main() runs in its own thread in the OS
int main()
{
    DigitalOut led(LED1);
    led = 1;

    printf("RUN\n");

#if ROBOT_NUMBER != 1
    rn4020_in.set_mldp_peripheral();
#endif

    char mac_address[] = "6827190AE562";
    rn4020_out.set_mldp_central(mac_address);

    while (1)
    {
#if ROBOT_NUMBER == 1
        if (dualShock3.readable())
        {
            char data[8];
            dualShock3.get_data(data);

            char circle = dualShock3.get_button(PS3::CIRCLE);

            if (circle == 1)
            {
                char output = 0x80;
                rn4020_out.write(&output, 1);
                break;
            }
        }
#else
        if (rn4020_in.readable())
        {
            char input;
            rn4020_in.read(&input, 1);

            if (input == 0x80)
                break;
        }
#endif
        led = !led;
        ThisThread::sleep_for(100ms);
    }

    debug("Push Circle!\n");

    Thread link_thread;
    Thread ball_screw_thread;
    Thread wheel_thread;

    link_thread.start(link);
    ball_screw_thread.start(ball_screw);
    wheel_thread.start(wheel);

    ThisThread::sleep_for(100ms);

    while (true)
    {
#if ROBOT_NUMBER == 1
        if (dualShock3.readable())
        {
            char input[8];
            dualShock3.get_data(input);

            char cross = dualShock3.get_button(PS3::CROSS);

            if (cross == 1)
            {
                char output = 0xC0;
                rn4020_out.write(&output, 1);
                ThisThread::sleep_for(100ms);
                NVIC_SystemReset();
                break;
            }
        }
#else
        if (rn4020_in.readable())
        {
            char input;
            rn4020_in.read(&input, 1);

            if (input == 0xC0)
            {
                char output = 0xC0;
                rn4020_out.write(&output, 1);
                ThisThread::sleep_for(100ms);
                NVIC_SystemReset();
                break;
            }
        }
#endif

        led = !led;
        ThisThread::sleep_for(100ms);
    }
}

void wheel()
{
    DP_TB6643 wheel_right(p28, p22);
    DP_TB6643 wheel_left(p27, p21);

    LineTrace line_trace(p20, p19, p18);
    line_trace.set_base_color(Color::White);

    while (1)
    {
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

        ThisThread::sleep_for(10ms);
    }
}

void link()
{
    TB6643 motor(p26, p25);
    DigitalIn sw(p12, PullDown);
    QEI encoder(p5, p6, NC, 512, QEI::X4_ENCODING);

    DigitalOut led(LED2);

    Timer timer;
    timer.start();

    const float cycle_time = 5.0f;
    const float reserve_time = 1.0f;

    while (1)
    {
        float elapsed_time = std::chrono::duration<float>{timer.elapsed_time()}.count();

        // printf("%d\r", (int)elapsed_time);

        if (sw == 1)
        {
            led = 1;

            if (elapsed_time < (cycle_time + reserve_time))
            {
                motor.set_duty_cycle(0.00f);
                motor.set_state(State::Brake);
                motor.set();
            }
            else
            {
                motor.set_duty_cycle(0.50f);
                motor.set_state(State::CCW);
                motor.set();
            }
        }
        else if (sw == 0)
        {
            led = 0;

            if (elapsed_time > (cycle_time + reserve_time))
            {
                timer.reset();
            }

            motor.set_duty_cycle(0.50f);
            motor.set_state(State::CCW);
            motor.set();
        }

        ThisThread::sleep_for(10ms);
    }
}

void ball_screw()
{
    TB6643 motor(p24, p23);
    DigitalIn sw(p11, PullDown);
    QEI encoder(p7, p8, NC, 100, QEI::X4_ENCODING);

    DigitalOut led(LED3);

    Timer timer;
    timer.start();

    const float cycle_time = 5.0f;
    const float reserve_time = 1.0f;

    bool is_rise = false;

    while (1)
    {
        float elapsed_time = std::chrono::duration<float>{timer.elapsed_time()}.count();

        // printf("%d %d   \r", (int)elapsed_time, (int)encoder.getRevolutions());

        if (sw == 1)
        {
            encoder.reset();

            if (elapsed_time < (cycle_time + reserve_time))
            {
                motor.set_duty_cycle(0.00f);
                motor.set_state(State::Brake);
                motor.set();
            }
            else
            {
                is_rise = true;

                motor.set_duty_cycle(0.95f);
                motor.set_state(State::CCW);
                motor.set();
            }
        }
        else if (is_rise == true)
        {
            if (elapsed_time > (cycle_time + reserve_time))
            {
                timer.reset();
            }

            led = 1;

            motor.set_duty_cycle(0.95f);
            motor.set_state(State::CCW);
            motor.set();
        }
        else if (is_rise == false)
        {
            led = 0;

            motor.set_duty_cycle(0.95f);
            motor.set_state(State::CW);
            motor.set();
        }

        if (fabsf(encoder.getRevolutions()) > 25.0f)
        {
            encoder.reset();
            is_rise = false;

            motor.set_duty_cycle(0.00f);
            motor.set_state(State::Free);
            motor.set();
        }

        ThisThread::sleep_for(10ms);
    }
}
