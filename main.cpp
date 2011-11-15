#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "argumentparser.h"
#include "exceptions.h"

using namespace std;

int usage(char* program_name)
{
    cout << "Usage: " << program_name << " [options] command [...]          " << endl <<
            "                                                               " << endl <<
            "Options:                                                       " << endl <<
            "  -i PATH                     Use serial port located at PATH. " << endl <<
            "                              Defaults to '/dev/ttyS0'.        " << endl <<
            "  -t                          Testing only. Run a series of    " << endl <<
            "                              predefined commands, then exit.  " << endl <<
            "                              All other commands are ignored.  " << endl <<
            "                                                               " << endl <<
            "Commands:                                                      " << endl <<
            "  -[1-8]:                     Turn specified ports off.        " << endl <<
            "  +[1-8]:                     Turn specified ports on.         " << endl <<
            "  ~[1-8]:                     Toggle specified ports.          " << endl <<
            "                                                               " << endl <<
            "Port 1 is the one closest to the RS-232 port.                  " << endl <<
            "All commands do not affect ports that are not specified.       " << endl <<
            "Multiple commands may be specified. They are executed in the   " << endl <<
            "Order they appear on the command line. Every group of command  " << endl <<
            "is executed with one operation on the relais card.             " << endl <<
            "                                                               " << endl <<
            "Example:                                                       " << endl <<
            "                                                               " << endl <<
            "  " << program_name << " +15 +3 ~8 -7                          " << endl <<
            "                                                               " << endl <<
            "First turn on ports 1 and 5, then turn on port 3, then toggle  " << endl <<
            "port 8 and then shutdown port 7. Each action follows the       " << endl <<
            "previous after a short delay.                                  " << endl <<
            "                                                               " << endl <<
            "";

    return EXIT_FAILURE;

}

void execute(int, char**);

int main(int argc, char** argv)
{

    try
    {
        execute(argc, argv);
    }
    catch(InvalidArgumentException* e)
    {
        return usage(basename(argv[0]));
    }
    catch(Exception* e)
    {
        cerr << basename(argv[0]) << ": Error: " << e->getMessage() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void execute(int argc, char **argv)
{
    RelaisCtrl* controller = new RelaisCtrl();

    try
    {
        ArgumentParser argumentParser(argc, argv);
        argumentParser.parseArguments(controller);

        controller->executeCommands();
    }
    catch(TestOnlyException* e)
    {
        controller->setTestMode();
    }
}

