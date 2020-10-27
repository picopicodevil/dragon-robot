#ifndef RN4020_H
#define RN4020_H

#include "mbed.h"

class RN4020: public BufferedSerial
{
    public:
    RN4020(PinName tx, PinName rx, int baud = 115200);

    int reboot();
    int init();

    int set_mldp_central(const char *mac_address, const char *bit_pattern = "92000000");
    int set_mldp_peripheral(const char *bit_pattern = "32000000");

    char* get_mac_address();

    private:

    bool _is_cmd_mode;
    bool _is_mldp_mode;

    char _mac_address[13];

    int wait_until(const char *string);
    void flush();
};

#endif