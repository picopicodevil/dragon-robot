/*
  *This software includes the work that is distributed in the Apache License 2.0
  */

#ifndef MOTOR_H
#define MOTOR_H

enum class State
{
    Free = 0,
    CW,  // Clock Wise
    CCW, // Counter Clock Wise
    Brake,

    TotalState,
};

/** A Motor is abstract base class for moving the motor
 */
class Motor
{
public:
    Motor();

    /** Initialize data
    *
    */
    void init();

    /** Set the duty cycle
    *
    * @param duty_cycle duty cycle to set
    */
    void set_duty_cycle(float value);

    /** Return the duty cycle
    *
    * @returns
    *    the duty cycle
    */
    float get_duty_cycle() const;

    /** Set the state
    *
    * @param type state to set
    */
    void set_state(State type);

    /** Return the state
    *
    * @returns
    *    the state
    */
    State get_state() const;

    /** Set the pulse period
    *
    * @param s period to set
    */
    void set_pulse_period(float s);

    /** Return the pulse_period
    *
    * @returns
    *    the pulse_period
    */
    float get_pulse_period() const;

    /** Set the pulse frequency
    *
    * @param hz frequency to set
    */
    void set_frequency(float hz);

    /** Return the frequency
    *
    * @returns
    *    the frequency
    */
    float get_frequency() const;

    static constexpr State default_state = State::Brake;
    static constexpr float default_pulse_period = 0.00002f;
    static constexpr float default_frequency = 1.0f / default_pulse_period;

    static constexpr float min_pulse_period = 1.0f / 60000.0f; // 60khz
    static constexpr float max_pulse_period = 1.0f / 10.0f;    // 10hz

protected:
    float _duty_cycle;
    State _state;
    float _pulse_period;
};

#endif
