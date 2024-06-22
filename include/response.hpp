#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "common.h"

class Response {
private:
    char rsp_code;
    char *message;
public:
    Response();
    Response(const char rsp_code, const char message[], size_t msglen);
    void printResponse ();
    uint8_t *serialize(ssize_t *buflen);

    char getResponseCode() const
        { return rsp_code; }
    char *getMessage() const
        { return message; }
 
    void setResponseCode(char rsp_code);
    void setMessage(char message[], size_t msglen);
};

#endif