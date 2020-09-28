#include "mbed.h"
#include "Wheels.h"
#include "Digital_PWM_TB6643.h"
#include "RN4020.h"

Wheels wheels(p26, p25, p24, p23);
DP_TB6643 motor(p28, p22);

// RN4020 rn4020_in(p9, p10);
// RN4020 rn4020_out(p13, p14);

BufferedSerial pc(USBTX, USBRX, 115200);

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

// main() runs in its own thread in the OS
int main()
{
    led1 = 1;
    pc.write("RUN\r\n",5);
    while (true) {
        if (pc.readable())
        {
            char character;
            pc.read(&character, 1);

            switch(character){
            case 'w':
                wheels.straight();
                pc.write("STRAI\r", 6);
                break;
            case 'a':
                wheels.turn_left();
                pc.write("LEFT \r", 6);
                break;
            case 's':
                wheels.back();
                pc.write("BACK \r", 6);
                break;
            case 'd':
                wheels.turn_right();
                pc.write("RIGHT\r", 6);
                break;
            case 'b':
                wheels.brake();
                pc.write("BRAKE\r", 6);
                break;
            case 'u':
                motor.set_duty_cycle(0.70f);
                motor.set_state(State::CCW);
                motor.set();
                pc.write("UP_DO\r", 6);
                led4 = !led4;
                break;
            default:
                wheels.brake();
                pc.write("OTHER\r", 6);
                break;
            }
        }
        led1 = !led1;
        ThisThread::sleep_for(100ms);
    }
}

