#include "ball_screw.h"
#include "mbed.h"
#include "TB6643.h"
#include "QEI.h"

void ball_screw()
{
    TB6643 motor(p26, p25);
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