#include "ball_screw.h"
#include "mbed.h"
#include "TB6643.h"
#include "QEI.h"

void ball_screw()
{
    TB6643 motor(p26, p25);
    // DigitalIn sw(p11, PullDown);
    QEI encoder(p7, p8, NC, 100, QEI::X4_ENCODING);

    DigitalOut led(LED3);

    Timer timer;
    timer.start();

    constexpr float cycle_time = 5.0f;
    constexpr float reserve_time = 1.0f;

    constexpr float duty_cycle = 0.98f;

    bool is_rise = false;

    while (1)
    {
        float elapsed_time = std::chrono::duration<float>{timer.elapsed_time()}.count();

        // printf("%d %d   \r", (int)elapsed_time, (int)encoder.getRevolutions());

        led = is_rise;

        if (is_rise == true)
        {
            motor.set_duty_cycle(duty_cycle);
            motor.set_state(State::CCW);
            motor.set();
        }
        else if (is_rise == false)
        {
            motor.set_duty_cycle(duty_cycle);
            motor.set_state(State::CW);
            motor.set();
        }

        if (fabsf(encoder.getRevolutions()) > 15.0f)
        {
            encoder.reset();
            is_rise = !is_rise;

            motor.set_duty_cycle(0.00f);
            motor.set_state(State::Brake);
            motor.set();

            while (elapsed_time < (cycle_time + reserve_time))
            {
                elapsed_time = std::chrono::duration<float>{timer.elapsed_time()}.count();
                ThisThread::sleep_for(10ms);
            }

            timer.reset();
        }

        ThisThread::sleep_for(10ms);
    }
}
