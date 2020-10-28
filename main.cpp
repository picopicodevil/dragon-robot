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
int is_receive_start_code(PS3 &serialInput);
#else
int is_receive_start_code(RN4020 &serialInput);
#endif
void send_start_code(RN4020 &rn4020_out);

#if ROBOT_NUMBER == 1
int is_receive_reset_code(PS3 &serialInput);
#else
int is_receive_reset_code(RN4020 &serialInput);
#endif
void send_reset_code(RN4020 &rn4020_out);

// main() runs in its own thread in the OS
int main()
{
    DigitalOut led(LED1);
    led = 1;

    printf("RUN\n");

    ThisThread::sleep_for(2s); // rn4020のCMD出力を待つ

#if ROBOT_NUMBER == 1
    PS3 serialInput(p14);
#else
    RN4020 serialInput(p13, p14);
    serialInput.set_mldp_peripheral();
#endif

#if ROBOT_NUMBER != 5
    RN4020 rn4020_out(p9, p10);
    rn4020_out.set_mldp_central(MAC_ADDRESS);
#endif

    while (1)
    {
        if (is_receive_start_code(serialInput))
        {
#if ROBOT_NUMBER != 5
            send_start_code(rn4020_out);
#endif
            break;
        }

        led = !led;
        ThisThread::sleep_for(100ms);
    }

    Thread link_thread;
    Thread ball_screw_thread;
    Thread wheel_thread;

    link_thread.start(link);
    ball_screw_thread.start(ball_screw);
    wheel_thread.start(wheel);

    while (true)
    {
        if (is_receive_reset_code(serialInput))
        {
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
int is_receive_start_code(PS3 &serialInput)
#else
int is_receive_start_code(RN4020 &serialInput)
#endif
{
    if (serialInput.readable())
    {
#if ROBOT_NUMBER == 1
        char data[8];
        serialInput.get_data(data);

        if (serialInput.get_button(PS3::CIRCLE))
            return 1;
#else
        char data;
        serialInput.read(&data, 1);

        if (data == 0x80)
            return 1;
#endif
    }
    return 0;
}

void send_start_code(RN4020 &rn4020_out)
{
#if ROBOT_NUMBER != 5
    char output = 0x80;
    rn4020_out.write(&output, 1);
    ThisThread::sleep_for(100ms);
#endif
}

#if ROBOT_NUMBER == 1
int is_receive_reset_code(PS3 &serialInput)
#else
int is_receive_reset_code(RN4020 &serialInput)
#endif
{
    if (serialInput.readable())
    {
#if ROBOT_NUMBER == 1
        char data[8];
        serialInput.get_data(data);

        if (serialInput.get_button(PS3::CROSS))
            return 1;
#else
        char data;
        serialInput.read(&data, 1);

        if (data == 0xC0)
            return 1;
#endif
    }
    return 0;
}

void send_reset_code(RN4020 &rn4020_out)
{
#if ROBOT_NUMBER != 5
    char output = 0xC0;
    rn4020_out.write(&output, 1);
    ThisThread::sleep_for(100ms);
#endif
}
