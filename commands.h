#ifndef _COMMAND_H
#define _COMMAND_H

#include <stdio.h>
#include "relaisdriver.h"

class Command
{
    public:
        Command(uchar ports) :
           ports(ports)
        {
            nextInQueue = 0;
        }

        bool hasNext() { return nextInQueue != 0; }
        Command* next() { return nextInQueue; }
        void setNext(Command* command) { nextInQueue = command; }

        virtual void execute(RelaisDriver* driver) = 0;
        void executeAll(RelaisDriver* driver)
        {
            this->execute(driver);
            if(hasNext()) next()->executeAll(driver);
        }

    protected:
        uchar ports;

    private:
        Command* nextInQueue;
};

class NopCommand : public Command
{
    public:
        NopCommand() : Command(0)
        {
        }

        void execute(RelaisDriver* driver);
};

class InitCommand : public Command
{
    public:
        InitCommand() : Command(0)
        {
        }

        void execute(RelaisDriver* driver);
};

class SetCommand : public Command
{
    public:
        SetCommand(uchar ports) : Command(ports) {}
        void execute(RelaisDriver* driver);
};

class UnsetCommand : public Command
{
    public:
        UnsetCommand(uchar ports) : Command(ports) {}
        void execute(RelaisDriver* driver);
};

class ToggleCommand : public Command
{
    public:
        ToggleCommand(uchar ports) : Command(ports) {}
        void execute(RelaisDriver* driver);
};


#endif
