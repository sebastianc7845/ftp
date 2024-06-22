#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "common.h"

class Command {
private:
    char cmd[5];
    char *args;
public:
    Command();
    Command(const char cmd[5], const char args[], int arglen);
    void printCommand();
    uint8_t *serialize(ssize_t *buflen);

    char getCommandType();
    char *getArgs();

    void setCommandType(char command[5]);
    void setArgs(char args[], size_t arglen);

    ~Command();
};

#endif