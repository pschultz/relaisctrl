#include <iostream>
#include <stdio.h>

#include "argumentparser.h"
#include "exceptions.h"

using namespace std;

ArgumentParser::ArgumentParser(int argc, char** argv):
   argc(argc)
  ,argv(argv)
  ,nArg(0)
{
}

void ArgumentParser::parseArguments(RelaisCtrl* controller)
{
    this->controller = controller;

    if(argc < 2) 
    {
        throw new InvalidArgumentException();
    }

    while( nextArgument() )
    {
        parseArgument();
    }
}

void ArgumentParser::parseArgument()
{

    if(currentArgument.compare("-i") == 0) {
        nextMandatoryArgument();
        controller->setDevice(currentArgument);        
        return;
    }

    if(currentArgument.compare("-t") == 0) {
        throw new TestOnlyException();
    }

    controller->parseCommandArgument(currentArgument);
}

bool ArgumentParser::nextArgument()
{
    if(++nArg >= argc) {
        return false;
    }

    currentArgument = string(argv[nArg]);

    return true;
}

void ArgumentParser::nextMandatoryArgument()
{
    if(!nextArgument())
    {
        throw new InvalidArgumentException();
    }
}

