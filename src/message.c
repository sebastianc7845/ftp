#include "message.h"
#include <stdio.h>

void print_response(Response *rsp) {
    if (rsp->message != NULL)
        printf("%c%s\n", rsp->rsp_code, rsp->message);
    else
        printf("%c\n", rsp->rsp_code);
}

void print_command(Command *cmd) {
    if (cmd->args != NULL)
        printf("%s %s\n", cmd->cmd, cmd->args);
    else
        printf("%s\n", cmd->cmd);

}