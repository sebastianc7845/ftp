#ifndef CLIENT_H
#define CLIENT_H

#include "common.h"
#include "command.hpp"
#include "response.hpp"
#include <string>

class Client {
private:
    std::string username;
    std::string password;

    int sockfd;
public:
    Client();
    ~Client();

    int InitSocket();
    int Connect();
    
    ssize_t SendBytes(const uint8_t *bytes, ssize_t buflen);
    ssize_t SendCommand(Command *cmd);
    ssize_t ReceiveBytes(uint8_t **bytes);
    ssize_t ReceiveResponse(Response *rsp);

    const std::string &GetUsername();
    void SetUsername(std::string &username);
    void SetUsername(const char username[]);

    const std::string &GetPassword();
    void SetPassword(std::string &password);
};

// typedef struct cli {
//     int sockfd;
// } Client;

// // Helper function that handles connecting to the server
// int client_connect(Client *c);

// // Helper function that recvs messages from the server
// int client_recv(Client *c, char *buf, size_t buflen);

// // Helper function that sends messages to the server
// int client_send(Client *c, char **buf);


#endif