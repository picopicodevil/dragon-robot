#include "link.h"
#include "mbed.h"
#include "TB6643.h"
#include "QEI.h"

void link()
{
    TB6643 motor(p24, p23);
    DigitalIn sw(p12, PullDown);
    QEI encoder(p5, p6, NC, 512, QEI::X4_ENCODING);

    DigitalOut led(LED2);

    Timer timer;
    timer.start();

    const float cycle_time = 5.0f;
    const float reserve_time = 1.0f;

    bool is_push = false;

    while (1)
    {
        float elapsed_time = std::chrono::duration<float>{timer.elapsed_time()}.count();

        // printf("%d\r", (int)elapsed_time);

        constexpr float ccw_duty_cycle = 0.35f;

        if (sw == 1)
        {
            led = 1;

            if (elapsed_time < (cycle_time + reserve_time))
            {
                is_push = true;

                motor.set_duty_cycle(0.00f);
                motor.set_state(State::Brake);
                motor.set();
            }
            else
            {
                motor.set_duty_cycle(ccw_duty_cycle);
                motor.set_state(State::CCW);
                motor.set();
            }
        }
        else if (sw == 0)
        {
            led = 0;

            if (elapsed_time > (cycle_time + reserve_time))
            {
                is_push = false;

                timer.reset();
            }

            if (is_push == true)
            {
                motor.set_duty_cycle(0.00f);
                motor.set_state(State::Brake);
                motor.set();
            }
            else
            {
                motor.set_duty_cycle(ccw_duty_cycle);
                motor.set_state(State::CCW);
                motor.set();
            }
        }

        ThisThread::sleep_for(10ms);
    }
}