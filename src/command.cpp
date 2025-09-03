#include <stdio.h>
#include <string.h>
#include "command.hpp"

// number of FTP commands
static const int NUM_CMDS = 9;
// FTP commands in c-string array
static const char *CMD_STRINGS[]  = {
    "USER", "QUIT", "PORT", "TYPE", 
    "MODE", "STRU", "RETR", "STOR", 
    "NOOP"
};

Command::Command() {
    args = NULL;
    args = new char[MAX_STRLEN + 1];
}

Command::Command(const char cmd[CMD_LEN], const char args[], int arglen) {
    // TODO: check that parameter is a valid command type
    // TODO: OPTIMIZE

    this->setCommandType(cmd);

    this->args = new char[MAX_STRLEN + 1];

    arglen = arglen > MAX_STRLEN ? MAX_STRLEN : arglen;
    strncpy(this->args, args, arglen);
    this->args[arglen] = '\0';
}

Command::Command(Cmd cmd, const char args[], int arglen) {
    this->cmd = cmd;

    this->args = new char[MAX_STRLEN + 1];

    arglen = arglen > MAX_STRLEN ? MAX_STRLEN : arglen;
    strncpy(this->args, args, arglen);
    this->args[arglen] = '\0';
}

void Command::setCommandType(Cmd cmd) {
    this->cmd = cmd;
}

void Command::setCommandType(const char command[]) {
    int i = 0;
    bool match_found = false;
    // iterate over FTP command list
    while (i < NUM_CMDS && !match_found) {
        // if param matches current FTP list entry
        if (strncmp(command, CMD_STRINGS[i], CMD_LEN) == 0) {
            // set cmd to appropriate cmd enum
            this->cmd = (Cmd)i;
            match_found = true;
        } else {
            i++;
        }
    }

    LOGF("Parameter: %s, this->cmd: %d", command, this->cmd);
}

void Command::setArgs(char *args, size_t arglen) {
    arglen = arglen > MAX_STRLEN ? MAX_STRLEN : arglen;

    strncpy(this->args, args, arglen);
    this->args[MAX_STRLEN] = '\0';
}

Cmd Command::getCommandType() {
    return this->cmd;
}

const char *Command::getArgs() {
    return this->args;
}

void Command::printCommand() {
    if (args != NULL)
        printf("%s %s\n", CMD_STRINGS[cmd], args);
    else
        printf("%s\n", CMD_STRINGS[cmd]);

}

uint8_t *Command::serialize(ssize_t *buflen) {
    uint8_t *bytes = NULL;
    size_t arglen = strlen(args);
    ssize_t len = CMD_LEN + 1 /* space */ + (arglen > MAX_STRLEN ? MAX_STRLEN : arglen) /* min(MAX_STRLEN, arglen) */ + 1 /* NULL CHAR */;
    size_t offset = 0;

    bytes = new uint8_t[len];
    
    memcpy(&(bytes[offset]), CMD_STRINGS[this->cmd], CMD_LEN);
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