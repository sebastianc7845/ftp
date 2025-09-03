#include "reply.hpp"

Reply::Reply() {
    memset(code, '\0', REP_LEN);
    msg = new char[MAX_STRLEN + 1];
}

Reply::Reply(const char code[], const char msg[], int msglen) {
    this->setReplyCode(code);

    this->msg = new char[MAX_STRLEN + 1];
    this->setMessage(msg, msglen);
}

const char *Reply::getReplyCode() {
    return code;
}

const char *Reply::getMessage() {
    return msg;
}

void Reply::setReplyCode(const char code[]) {
    strncpy(this->code, code, REP_LEN);
}

void Reply::setMessage(const char msg[], ssize_t msglen) {
    msglen = msglen > MAX_STRLEN ? MAX_STRLEN : msglen;

    strncpy(this->msg, msg, msglen);
    this->msg[msglen] = '\0';
}

uint8_t *Reply::serialize(ssize_t *buflen) {
    uint8_t *bytes = NULL;
    size_t msglen = strlen(msg);
    ssize_t len = REP_LEN + 1 /* space */ + (msglen > MAX_STRLEN ? MAX_STRLEN : msglen) /* min(MAX_STRLEN, arglen) */ + 1 /* NULL CHAR */;
    size_t offset = 0;

    bytes = new uint8_t[len];
    
    memcpy(&(bytes[offset]), code, REP_LEN);
    offset += REP_LEN;

    bytes[offset] = ' ';
    offset += 1;

    memcpy(&(bytes[offset]), msg, msglen);
    offset += msglen;

    bytes[offset] = '\0';
    offset += 1;

    *buflen = len;
    return bytes;
}

void Reply::printReply() {
    if (msg == nullptr) {
        printf("%.3s\n", this->code);
    } else {
        printf("%.3s %s\n", this->code, this->msg);
    }
}

Reply::~Reply() {
    delete[] msg;
}