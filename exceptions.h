#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include <stdio.h>

class Exception {
    public:
        Exception(const char* message) : message(message) {}
        Exception(std::string message) : message(message.c_str()) {}
        Exception() : message("") {}

        const char* getMessage() { return message; }

    protected:
        const char* message;
};

class TestOnlyException : Exception {
    public: 
        TestOnlyException() : Exception() {}
};

class InvalidArgumentException : Exception {
    public:
        InvalidArgumentException() : Exception() {}
        InvalidArgumentException(std::string message) : Exception(message) {}
        InvalidArgumentException(const char* message) : Exception(message) {}
};

#endif
