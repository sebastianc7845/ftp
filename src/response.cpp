#include <stdio.h>
#include <string.h>
#include "response.hpp"

Response::Response(){
    rsp_code = '\0';
    message = new char[MAX_STRLEN + 1];
}

Response::Response(const char rsp_code, const char message[], size_t msglen) {
    this->rsp_code = rsp_code;

    this->message = new char[MAX_STRLEN + 1];

    msglen = msglen > MAX_STRLEN ? MAX_STRLEN : msglen;
    strncpy(this->message, message, msglen);
    this->message[msglen] = '\0';
}

void Response::setResponseCode(char rsp_code) {
    this->rsp_code = rsp_code;
}

void Response::setMessage(char message[], size_t msglen) {
    msglen = msglen > MAX_STRLEN ? MAX_STRLEN : msglen;

    strncpy(this->message, message, msglen);
    this->message[MAX_STRLEN] = '\0';
}

void Response::printResponse() {
    if (message != NULL)
        printf("%c%s\n", rsp_code, message);
    else
        printf("%c\n", rsp_code);
}

uint8_t *Response::serialize(ssize_t *buflen) {
    uint8_t *bytes = NULL;
    size_t msglen = strlen(message);
    size_t len = 1 /* rsp code */ + (msglen > MAX_STRLEN ? MAX_STRLEN : msglen) + 1 /* NULL CHAR */;

    bytes = new uint8_t[len];

    bytes[0] = (uint8_t)rsp_code;
    memcpy(&(bytes[1]), message, msglen);
    bytes[len - 1] = '\0';

    *buflen = len;
    return bytes;
}