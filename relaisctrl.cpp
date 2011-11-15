#include <stdio.h>
#include <pcre.h>
#include <bitset>

#include "relaisctrl.h"
#include "exceptions.h"

#define OVECCOUNT 30    /* should be a multiple of 3 */

using namespace std;

RelaisCtrl::RelaisCtrl()
{
    device = "/dev/ttyS0";
    argumentPattern = "[+-~][1-8]+";
    commandQueue = new NopCommand();
    tail = commandQueue;
}

void RelaisCtrl::setDevice(string device)
{
    this->device = device;
}

void RelaisCtrl::setTestMode()
{
    driver = new RelaisDriver(device);
    driver->setup();
    printf("TESTING ONLY\n");

    commandQueue->execute(driver);
    driver->test();
}

void RelaisCtrl::executeCommands()
{
    driver = new RelaisDriver(device);
    driver->setup();
    commandQueue->executeAll(driver);
}

void RelaisCtrl::parseCommandArgument(string argument)
{
    validateArgument(argument);
    
    uchar ports = 0;
    for(uint i = 1; i < argument.length(); ++i)
    {
        switch(argument[i])
        {
            case '1':
                ports |= PORT1;
                break;
            case '2':
                ports |= PORT2;
                break;
            case '3':
                ports |= PORT3;
                break;
            case '4':
                ports |= PORT4;
                break;
            case '5':
                ports |= PORT5;
                break;
            case '6':
                ports |= PORT6;
                break;
            case '7':
                ports |= PORT7;
                break;
            case '8':
                ports |= PORT8;
                break;
            default:
                throw new InvalidArgumentException();
        }
    }

    Command* nextCommand;
    switch(argument[0])
    {
        case '+':
            nextCommand = new SetCommand(ports);
            break;
        case '-':
            nextCommand = new UnsetCommand(ports);
            break;
        case '~':
            nextCommand = new ToggleCommand(ports);
            break;
        default:
            throw new InvalidArgumentException();
    }

    appendCommandToQueue(nextCommand);
}

void RelaisCtrl::appendCommandToQueue(Command* command)
{
    tail->setNext(command);
    tail = command;
}

void RelaisCtrl::validateArgument(string argument)
{
    const char* error;
    int erroroffset;
    int ovector[OVECCOUNT];
    int rc;

    pcre* pattern = pcre_compile(
        argumentPattern.c_str(),
        0,               // default options
        &error,          // for error message
        &erroroffset,    // for error offset
        NULL
    );

    if(pattern == NULL)
    {
        throw new Exception();
    }

    rc = pcre_exec(
        pattern,              /* the compiled pattern */
        NULL,                 /* no extra data - we didn't study the pattern */
        argument.c_str(),     /* the subject string */
        argument.length(),    /* the length of the subject */
        0,                    /* start at offset 0 in the subject */
        0,                    /* default options */
        ovector,              /* output vector for substring information */
        OVECCOUNT             /* number of elements in the output vector */
    );

    if(rc == PCRE_ERROR_NOMATCH)
    {
        throw new InvalidArgumentException();
    }
    else if(rc < 0)
    {
        throw new Exception();
    }
}

