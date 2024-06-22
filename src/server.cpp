#include "server.hpp"
#include "network.hpp"

/*----------------------------- Constructors & Destructor -----------------------------*/

Server::Server() {
    // TODO: If failed to create socket, entry code should try to
    // create the socket again 5 seconds later...
    InitSocket();

    // Initialize poll array
    // initialize stdin poll
    polls[STDIN_POLL].fd = STDIN_FILENO;
    polls[STDIN_POLL].events = POLLIN;
    polls[STDIN_POLL].revents = 0;
    // initialize listen poll
    polls[LISTEN_POLL].fd = sockfd;
    polls[LISTEN_POLL].events = POLLIN;
    polls[LISTEN_POLL].revents = 0;

    // initialize user and poll array
    // for each user
    for (int i = 0; i < MAX_CLIENTS; i++) {
        // initialize poll
        polls[GET_POLL_IDX(i)].fd = -1;
        polls[GET_POLL_IDX(i)].events = 0;

        // initialize user ptr to null
        known_users[i] = NULL;
    }

    if (Bind() < 0) {
        LOGERRNO;
    }

    if (Listen() < 0) {
        LOGERRNO;
    }

    // initialize number of users
    num_users = 0;
}

Server::~Server() {
    close(sockfd);
    // TODO: free user arr
}

/*--------------------------------- Private functions ---------------------------------*/

int Server::GetNewUserIdx() {
    int ret = -1;

    // for each poll
    for (int i = 0; i < MAX_CLIENTS && ret < 0; i++)
        // if user is null
        if (known_users[i] == NULL)
            // ret = curr idx
            ret = i;

    // return idx
    return ret;
}

void Server::CloseUser(int user_idx) {
    // "free" poll entry
    polls[GET_POLL_IDX(user_idx)].fd = -1;
    polls[GET_POLL_IDX(user_idx)].events = 0;
    polls[GET_POLL_IDX(user_idx)].revents = 0;

    // close connection to this user
    close(known_users[user_idx]->userfd);

    // free this user
    delete known_users[user_idx];
    // init ptr to null
    known_users[user_idx] = NULL;
}

/*---------------------------------- Public functions ----------------------------------*/

int Server::InitSocket() {
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        LOGERRNO;
        
    // TODO: better error handling in main
    return sockfd;
}

int Server::Bind() {
    struct sockaddr_in srv_addr;

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = INADDR_ANY;
    srv_addr.sin_port = htons(SFTP_PORT);

    return bind(sockfd, (struct sockaddr *) &srv_addr, sizeof(struct sockaddr_in));
}

int Server::Listen() {
    return listen(sockfd, MAX_CLIENTS);
}

int Server::AcceptUser() {
    char addrstr[INET_ADDRSTRLEN + 1];
    int user_idx = -1;
    int ret = -1;

    User newUser;

    newUser.addrlen = sizeof(newUser.last_known_addr);
    newUser.userfd = 
        accept(sockfd, (struct sockaddr *) &(newUser.last_known_addr), &(newUser.addrlen));

    inet_ntop(AF_INET, &(newUser.last_known_addr), addrstr, INET_ADDRSTRLEN);
    addrstr[INET_ADDRSTRLEN] = '\0';

    LOGF("received connection from client at address: %s", addrstr);

    user_idx = GetNewUserIdx();

    if (newUser.userfd < 0 || user_idx < 0)
        LOGF("%s", "failed to accept connection. Max number of clients reached");

    known_users[user_idx] = new User;

    memcpy(known_users[user_idx], &(newUser), sizeof(newUser));

    polls[GET_POLL_IDX(user_idx)].fd = newUser.userfd;
    polls[GET_POLL_IDX(user_idx)].events = POLLIN;
    polls[GET_POLL_IDX(user_idx)].revents = 0;

    return ret;
}

ssize_t Server::SendBytes(int user_idx, const uint8_t *bytes, ssize_t buflen) {
    return SendRaw(known_users[user_idx]->userfd, bytes, buflen);
}

ssize_t Server::SendResponse(int user_idx, Response *rsp) {
    ssize_t len, ret;
    uint8_t *bytes = rsp->serialize(&len);

    ret = SendBytes(user_idx, bytes, len);

    delete[] bytes;

    return ret;
}

// receive SFTP message as raw bytes
ssize_t Server::ReceiveBytes(int user_idx, uint8_t **bytes) {
    ssize_t ret = ReceiveRaw(known_users[user_idx]->userfd, bytes);

    if (ret == 0) {
        LOGF("user @ idx %d closed connection", user_idx);

        CloseUser(user_idx);
    }
    
    return ret;
}

ssize_t Server::ReceiveCommand(int user_idx, Command *cmd) {
    uint8_t *bytes = NULL;
    ssize_t bytes_recvd = ReceiveBytes(user_idx, &bytes);

    if (bytes_recvd > 0) {
        if (bytes_recvd > CMD_LEN + 1) {
            cmd->setCommandType((char *)bytes);
            cmd->setArgs((char *) (bytes + (CMD_LEN + 1)), bytes_recvd - (CMD_LEN + 1) - 1);
        } else
            LOGF("%s", "invalid command format");

        delete[] bytes;
    }

    return bytes_recvd;
}

// returns -1 if there was some error with call to poll
// returns -2 if there was no polls ready
int Server::CheckPolls() {
    int ret = poll(polls, MAX_CLIENTS + 2, 0);

    // if a poll ready
    if (ret > 0) {
        // check stdin poll
        // if ready, handle input
        // if (polls[STDIN_POLL].revents != 0) {
        //     LOGF("%s", "standard input received");
        //     ret = STDIN_POLL;
        //     polls[STDIN_POLL].revents = 0;
        // }
        // check listening poll
        // if ready
        if (polls[LISTEN_POLL].revents & POLLIN) {
            // return listen idx
            ret = LISTEN_POLL;
            polls[LISTEN_POLL].revents = 0;
        }

        // else, check user polls
        // for each user
        for (int i = 0; i < MAX_CLIENTS; i++) {
            // if ready
            if (polls[GET_POLL_IDX(i)].revents & POLLIN) {
                // return poll idx
                ret = GET_POLL_IDX(i);
                polls[GET_POLL_IDX(i)].revents &= ~POLLIN;
            }
        }
    // if no polls ready, return -2
    } else if (ret == 0)
        ret = -2;
    // if error with poll(), print msg
    else
        LOGERRNO;


    return ret;
}