##About
This is a driver software and cli for the \<insert board name here>.

##Compiling
Dependencies are

- libpcre
- Maybe more. Have to check on a clean system

To compile run

    make

##Usage

    relaisctrl [options] command [...]

###Options

    -i PATH      Use serial port located at PATH.
                 Defaults to '/dev/ttyS0'.
    -t           Testing only. Run a series of
                 predefined commands, then exit.
                 All other commands are ignored.

###Commands

    -[1-8]:      Turn specified ports off.
    +[1-8]:      Turn specified ports on.
    ~[1-8]:      Toggle specified ports.

Port 1 is the one closest to the RS-232 port on the board.
All commands do not affect ports that are not specified.
Multiple commands may be specified. They are executed in the 
order they appear on the command line. Every group of command
is executed with one operation on the relais card.

###Example

    relaisctrl +15 +3 ~8 -7

First turn on ports 1 and 5, then turn on port 3, then toggle
port 8 and then shutdown port 7. Each action follows the                                                                                                             
previous after a short delay.

You may have to set `LD_LIBRARY_PATH` to the location of `librelaisdriver.so`,
e. g. `LD_LIBRARY_PATH=. ./relaisctrl +15 +3 ~8 -7` right after `make`.
