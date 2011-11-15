#ifndef _RELAIS_CTRL_H
#define _RELAIS_CTRL_H

#include <string.h>

#include "relaisdriver.h"
#include "commands.h"

class RelaisCtrl
{
    public:
        RelaisCtrl();
        void setDevice(std::string);
        void setTestMode();
        void parseCommandArgument(std::string);
        void executeCommands();

    private:
        std::string device;
        std::string argumentPattern;

        RelaisDriver* driver;
        Command* commandQueue;
        Command* tail;

        void validateArgument(std::string);
        void appendCommandToQueue(Command*);
};

#endif
