#include "RN4020.h"
#include "mbed.h"
#include <cstring>

RN4020::RN4020(PinName tx, PinName rx, int baud)
    :BufferedSerial(tx, rx, baud)
{
}

int RN4020::set_mldp_central(const char *mac_address, const char *bit_pattern)
{
    sync();

    flush();

    // 初期化
    write("SF,1\n", 5);

    wait_until("AOK");

    // RN4020機能変更MLDP親機に設定
    write("SR,", 3);
    write(bit_pattern, 8);
    write("\n", 1);

    wait_until("AOK");

    // 再起動
    write("R,1\n", 4);

    wait_until("Reboot");
    wait_until("CMD");

    // 子機MACアドレスを指定
    write("E,0,", 4);
    write(mac_address, 12);
    write("\n", 1);

    wait_until("AOK");
    wait_until("Connected");

    // MLDPモードに移行
    write("I\n", 2);

    wait_until("MLDP");


    return 0;
}

int RN4020::set_mldp_peripheral(const char *bit_pattern)
{
    sync();

    flush();

    // 初期化
    write("SF,1\n", 5);

    wait_until("AOK");

    // RN4020機能変更MLDP子機に設定
    write("SR,", 3);
    write(bit_pattern, 8);
    write("\n",1);

    wait_until("AOK");

    // 再起動
    write("R,1\n", 4);
    
    wait_until("Reboot");
    wait_until("CMD");

    wait_until("Connected");
    wait_until("MLDP");

    return 0;
}

char* RN4020::get_mac_address()
{
    write("D\n",2);

    ThisThread::sleep_for(1s);

    wait_until("BTA=");
    
    read(_mac_address, 12);

    _mac_address[12] = '\0';

    while(readable())
    {
        char character;
        read(&character,1);
        write(&character, 1);
    }

    return _mac_address;
}

int RN4020::wait_until(const char *string)
{
    int index = 0;
    char buffer[32];

    Timer t;
    t.start();

    while(std::chrono::duration<float>{t.elapsed_time()}.count() < time_limit)
    {
        if (readable())
        {
            char character;
            read(&character,1);

            if ((character != '\n') && (character != '\r'))
            {
                buffer[index++] = character;
                buffer[index] = '\0';
                // debug(buffer);

                if (strcmp(buffer, string) == 0)
                {
                    debug("%s", string);
                    debug("\n");
                    return 1;
                }
            }
        }
        ThisThread::sleep_for(100ms);
    }
    return 0;
}

void RN4020::flush()
{
    while(readable())
    {
        char tmp;
        read(&tmp, 1);
    }
}