#ifndef REPLY_HPP
#define REPLY_HPP

#include "common.h"

class Reply {
private:
    // reply code
    char code[REP_LEN];
    // reply message
    char *msg;
public:
    Reply();
    Reply(const char code[], const char msg[], int msglen);

    const char *getReplyCode();
    const char *getMessage();

    void setReplyCode(const char code[]);
    void setMessage(const char msg[], ssize_t msglen);

    uint8_t *serialize(ssize_t *buflen);

    void printReply();

    ~Reply();
};

#endif