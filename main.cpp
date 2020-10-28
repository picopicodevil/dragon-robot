#include "mbed.h"
#include "TB6643.h"
#include "Digital_PWM_TB6643.h"
#include "PS3_SBDBT5V.h"
#include "RN4020.h"
#include "QEI.h"
#include "LineTrace.h"
#include "UniqueValue.h"
#include "link.h"
#include "ball_screw.h"

BufferedSerial pc(USBTX, USBRX, 115200);

void wheel();

#if ROBOT_NUMBER == 1
int is_receive_reset_code(PS3 &dualShock3);
#else
int is_receive_reset_code(RN4020 &rn4020_in);
#endif

void send_reset_code(RN4020 &rn4020_out);

// main() runs in its own thread in the OS
int main()
{
#if ROBOT_NUMBER == 1
    PS3 dualShock3(p14);
#else
    RN4020 rn4020_in(p13, p14);
#endif

#if ROBOT_NUMBER != 5
    RN4020 rn4020_out(p9, p10);
#endif

    DigitalOut led(LED1);
    led = 1;

    printf("RUN\n");

    ThisThread::sleep_for(2s); // rn4020のCMD出力を待つ目的で記述

#if ROBOT_NUMBER != 1
    rn4020_in.set_mldp_peripheral();
#endif

#if ROBOT_NUMBER != 5
    rn4020_out.set_mldp_central(MAC_ADDRESS);
#endif

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
#if ROBOT_NUMBER != 5
                char output = 0x80;
                rn4020_out.write(&output, 1);
#endif
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
        if (is_receive_reset_code(dualShock3)) {
#else
        if (is_receive_reset_code(rn4020_in)) {
#endif

#if ROBOT_NUMBER != 5
            send_reset_code(rn4020_out);
#endif
            NVIC_SystemReset();
        }
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

#if ROBOT_NUMBER == 1
int is_receive_reset_code(PS3 &dualShock3)
{
    if (dualShock3.readable())
    {
        char input[8];
        dualShock3.get_data(input);

        char cross = dualShock3.get_button(PS3::CROSS);

        if (cross == 1)
        {
            return 1;
        }
    }
    reutrn 0;
}
#else
int is_receive_reset_code(RN4020 &rn4020_in)
{
    if (rn4020_in.readable())
    {
        char input;
        rn4020_in.read(&input, 1);

        if (input == 0xC0)
        {
            return 1;
        }
    }
    return 0;
}
#endif

void send_reset_code(RN4020 &rn4020_out)
{
#if ROBOT_NUMBER != 5
    char output = 0xC0;
    rn4020_out.write(&output, 1);
    ThisThread::sleep_for(100ms);
#endif
}
