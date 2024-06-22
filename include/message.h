#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdbool.h>

// Response message
typedef struct rsp {
    char rsp_code;
    char *message;
} Response;

// Command message
typedef struct cmd {
    char cmd[5];
    char *args;
} Command;

// Helper functions that converts raw message to message struct
char *marshall_response(Response *rsp);
char *marshall_command(Command *cmd);

// Functions that take in a parsed message and prints them out
void print_response(Response *rsp);
void print_command(Command *cmd);

#endif