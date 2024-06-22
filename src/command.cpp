#include <stdio.h>
#include <string.h>
#include "command.hpp"

Command::Command() {
    args = NULL;
    args = new char[MAX_STRLEN + 1];
}

Command::Command(const char cmd[5], const char args[], int arglen) {
    // TODO: check that cmd parameter is a valid command type
    strncpy(this->cmd, cmd, CMD_LEN);
    this->cmd[CMD_LEN] = '\0';

    this->args = new char[MAX_STRLEN + 1];

    arglen = arglen > MAX_STRLEN ? MAX_STRLEN : arglen;
    strncpy(this->args, args, arglen);
    this->args[arglen] = '\0';
}

void Command::setCommandType(char cmd[5]) {
    // TODO: check that parameter is a valid command type
    strncpy(this->cmd, cmd, CMD_LEN);
    this->cmd[CMD_LEN] = '\0';
}

void Command::setArgs(char *args, size_t arglen) {
    arglen = arglen > MAX_STRLEN ? MAX_STRLEN : arglen;

    strncpy(this->args, args, arglen);
    this->args[MAX_STRLEN] = '\0';
}

void Command::printCommand() {
    if (args != NULL)
        printf("%s %s\n", cmd, args);
    else
        printf("%s\n", cmd);

}

uint8_t *Command::serialize(ssize_t *buflen) {
    uint8_t *bytes = NULL;
    size_t arglen = strlen(args);
    ssize_t len = CMD_LEN + 1 /* space */ + (arglen > MAX_STRLEN ? MAX_STRLEN : arglen) + 1 /* NULL CHAR */;
    size_t offset = 0;

    bytes = new uint8_t[len];
    
    memcpy(&(bytes[offset]), cmd, CMD_LEN);
    offset += CMD_LEN;

    bytes[offset] = ' ';
    offset += 1;

    memcpy(&(bytes[offset]), args, arglen);
    offset += arglen;

    bytes[offset] = '\0';
    offset += 1;

    *buflen = len;
    return bytes;
}

Command::~Command() {
    delete[] args;
}