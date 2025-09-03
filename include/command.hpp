#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "common.h"

// FTP commands as strong enum type
enum Cmd {
    USER, QUIT, PORT, TYPE, 
    MODE, STRU, RETR, STOR, 
    NOOP
};

class Command {
private:
    Cmd cmd; /* Used to index CMD c-string array */
             /* Cmd integer value == index of FTP command string */
    char *args;
public:
    Command();
    Command(const char cmd[5], const char args[], int arglen);
    Command(Cmd cmd, const char args[], int arglen);
    void printCommand();
    uint8_t *serialize(ssize_t *buflen);

    Cmd getCommandType();
    const char *getArgs();

    void setCommandType(Cmd command);
    void setCommandType(const char command[]);
    void setArgs(char args[], size_t arglen);

    ~Command();
};

#endif