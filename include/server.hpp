#ifndef SERVER_HPP
#define SERVER_HPP

#include "common.h"
#include "command.hpp"
#include "response.hpp"

typedef struct user {
    char *username;
    char *pw;

    int userfd;
    struct sockaddr_in last_known_addr;
    socklen_t addrlen;
    // contains working directory of user, it's privileges, etc.
    // filesystem_state fs_state;
} User;

class Server {
private:
    int sockfd;
    uint8_t recv_buf[MAX_RECV];
    // TODO: more fields?
    // user and polls are parallel arrays
    User *known_users[MAX_CLIENTS];
    struct pollfd polls[MAX_CLIENTS + 2];

    int num_users;
    // note that num_polls = num_users + 2

    int GetNewUserIdx();
    void CloseUser(int user_idx);
public:
    Server();
    ~Server();

    int InitSocket();
    int Bind();
    int Listen();
    int AcceptUser();

    ssize_t SendBytes(int user_idx, const uint8_t bytes[], ssize_t num_bytes);
    ssize_t SendResponse(int user_idx, Response *rsp);
    ssize_t ReceiveBytes(int user_idx, uint8_t **bytes);
    ssize_t ReceiveCommand(int user_idx, Command *cmd);

    int CheckPolls();
};

#endif