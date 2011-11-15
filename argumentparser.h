#ifndef _ARGUMENT_PARSER_H
#define _ARGUMENT_PARSER_H

#include "relaisctrl.h"

class ArgumentParser {
    private:
        int argc;
        char** argv;
        int nArg;

        RelaisCtrl* controller;

        std::string currentArgument;

        void parseArgument();
        bool nextArgument();
        void nextMandatoryArgument();

    public:
        ArgumentParser(int, char**);
        void parseArguments(RelaisCtrl*);
        
};

#endif
