#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <bitset>
#include <string.h>
#include <errno.h>

#include "relaisdriver.h"
#include "exceptions.h"

#define COMMAND_DELAY usleep(100000) 

using namespace std;

RelaisDriver::RelaisDriver(string device) :
   devicePath(device)
{
    serialPort = -1;
    currentState = 0;
}
RelaisDriver::~RelaisDriver() 
{
    if(serialPort != -1)
    {
        close(serialPort);
    }
}

void RelaisDriver::openSerialPort()
{

    errno = 0;
    serialPort = open(devicePath.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

    if(serialPort == -1)
    {
        string message("unable to open serial port '");
        message += devicePath + "': " + strerror(errno);

        throw new Exception(message);
    }

}

void RelaisDriver::setup()
{
    struct termios options;

    openSerialPort();

    fcntl(serialPort, F_SETFL, 0);
		
    /* get the current options */
    tcgetattr(serialPort, &options);

    /* set in and out speed */
    cfsetispeed(&options, B19200);
    cfsetospeed(&options, B19200);

    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE; /* Mask the character size bits */
    options.c_cflag |= CS8;    /* Select 8 data bits */

    /* set raw input, 1 second timeout */
    options.c_cflag     |= (CLOCAL | CREAD);
    options.c_lflag     &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag     &= ~OPOST;
    options.c_cc[VMIN]  = 0;
    options.c_cc[VTIME] = 10;

    /* set the options */
    tcsetattr(serialPort, TCSAFLUSH, &options);

    getCurrentState();
}

void RelaisDriver::initDevice()
{
    sendInitCommand();
    commandBuffer[1] = BROADCAST_ID;
}

void RelaisDriver::sendInitCommand()
{
    commandBuffer[0] = INIT_BYTE;
    commandBuffer[1] = 1;

    commit();
    commandBuffer[1] = BROADCAST_ID;
}

void RelaisDriver::getCurrentState()
{
    commandBuffer[0] = GET_BYTE;
    commandBuffer[1] = 1;
    currentState = ALL_PORTS;

    commit();

    currentState = lastResponse[2];
}

void RelaisDriver::unsetPorts(uchar ports)
{
    commandBuffer[0] = SET_BYTE;
    currentState &= ~ports;
}

void RelaisDriver::setPorts(uchar ports)
{
    commandBuffer[0] = SET_BYTE;
    currentState |= ports;
}

void RelaisDriver::togglePorts(uchar ports)
{
    commandBuffer[0] = SET_BYTE;
    currentState ^= ports;
}

void RelaisDriver::test()
{
    initDevice();

    printf("setting ports\n");
    printf("port1: %d\n", PORT1);
    printf("port2: %d\n", PORT2);
    printf("port3: %d\n", PORT3);
    printf("port4: %d\n", PORT4);
    printf("port5: %d\n", PORT5);
    printf("port6: %d\n", PORT6);
    printf("port7: %d\n", PORT7);
    printf("port8: %d\n", PORT8);

    printf("Setting all ports\n");
    setPortsImmediatly(ALL_PORTS);
    
    printf("Unsetting all ports\n");
    unsetPortsImmediatly(ALL_PORTS);

    printf("Setting port 2\n");
    setPortsImmediatly(PORT2);

    printf("Setting port 3\n");
    setPortsImmediatly(PORT3);

    printf("Unsetting port 2\n");
    unsetPortsImmediatly(PORT2);

    printf("Setting ports 5 and 6\n");
    setPorts(PORT5);
    setPorts(PORT6);
    commit();

    printf("Toggling ports 5 and 8\n");
    togglePorts(PORT5);
    togglePorts(PORT8);
    commit();

    printf("Toggling port 8\n");
    togglePortsImmediatly(PORT8);

}

void RelaisDriver::nop()
{
    commandBuffer[0] =
    commandBuffer[1] =
    commandBuffer[2] =
    commandBuffer[3] =
        0;

    commit();
}

void RelaisDriver::commit()
{
    if(serialPort == -1)
    {
        throw new Exception("Serial port closed. Did you call setup()?");
    }

    commandBuffer[2] = currentState;
    commandBuffer[3] = getChecksum(commandBuffer);

    errno = 0;
    if (!write(serialPort, commandBuffer, 4)) 
    {
        close(serialPort);
        string message("IO error when writing to serial port: ");
        message += strerror(errno);
        throw new Exception(message);
    }

    lastResponse[0] = lastResponse[1] = lastResponse[2] = lastResponse[3] = 0;

    read(serialPort, lastResponse, 4);
    if(lastResponse[1] == 0)
    {
        read(serialPort, lastResponse, 4);
    }

    COMMAND_DELAY;
    if(commandBuffer[0] != GET_BYTE)
    {
        getCurrentState();
    }
}

uchar RelaisDriver::getChecksum(uchar* frame) {
    return frame[0] ^ frame[1] ^ frame[2];
}

