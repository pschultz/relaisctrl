#ifndef _RELAIS_DRIVER_H
#define _RELAIS_DRIVER_H

#include <string>
#include <unistd.h>

#define NOP_BYTE 0
#define INIT_BYTE 1
#define GET_BYTE 2
#define SET_BYTE 3

#define BROADCAST_ID 0
#define NO_DATA 0

#define PORT1 0x01
#define PORT2 0x02
#define PORT3 0x04
#define PORT4 0x08
#define PORT5 0x10
#define PORT6 0x20
#define PORT7 0x40
#define PORT8 0x80
#define ALL_PORTS PORT1 | PORT2 | PORT3 | PORT4 | PORT5 | PORT6 | PORT7 | PORT8

typedef unsigned char uchar;

class RelaisDriver
{
    public:
        RelaisDriver() : devicePath("/dev/ttyS0") {}
        RelaisDriver(std::string device);
        ~RelaisDriver();

        void setup();
        void test();

        void initDevice();
        void nop();

        void setPorts(uchar);
        void setPortsImmediatly(uchar ports) { setPorts(ports); commit(); }
        
        void unsetPorts(uchar);
        void unsetPortsImmediatly(uchar ports) { unsetPorts(ports); commit(); }

        void togglePorts(uchar);
        void togglePortsImmediatly(uchar ports) { togglePorts(ports); commit(); }

        void commit();


    private:
        std::string devicePath;
        int serialPort;

        unsigned char lastResponse[4];
        unsigned char commandBuffer[4];
        uchar currentState;

        bool hasBeenInitialized;

        uchar getChecksum(uchar*);

        void openSerialPort();
        void sendInitCommand();

        void getCurrentState();
};

#endif
