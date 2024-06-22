#include "client.hpp"
#include "network.hpp"

/*----------------------------- Constructors & Destructor -----------------------------*/

Client::Client() {
    // TODO: If failed to create socket, entry code should try to
    // create the socket again 5 seconds later...
    this->InitSocket();
}

Client::~Client() {
    close(sockfd);
}

/*--------------------------------- Private functions ---------------------------------*/

/*---------------------------------- Public functions ----------------------------------*/

int Client::InitSocket() {
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        LOGERRNO;
        
    // TODO: better error handling in main
    return sockfd;
}

int Client::Connect() {
    struct sockaddr_in srv_addr;

    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(SFTP_PORT);
    inet_pton(AF_INET, "127.0.0.1", &(srv_addr.sin_addr));

    return connect(sockfd, (struct sockaddr *) &srv_addr, sizeof(struct sockaddr_in));
}


// Send SFTP message as raw bytes
ssize_t Client::SendBytes(const uint8_t *bytes, ssize_t buflen) {

    return SendRaw(sockfd, bytes, buflen);
}

ssize_t Client::SendCommand(Command *cmd) {
    ssize_t num_bytes = 0, ret;
    uint8_t *bytes = cmd->serialize(&num_bytes);

    ret = SendBytes(bytes, num_bytes);

    return ret;
}

ssize_t Client::ReceiveBytes(uint8_t **bytes) {
    return ReceiveRaw(sockfd, bytes);
}

ssize_t Client::ReceiveResponse(Response *rsp) {
    uint8_t *bytes = NULL;
    ssize_t bytes_recvd = ReceiveBytes(&bytes);

    if (bytes_recvd > 0) {
        rsp->setResponseCode((char)bytes[0]);
        rsp->setMessage((char *) (bytes + 1), bytes_recvd - 2 /* NULL CHAR + rsp code */);

        delete[] bytes;
    }

    return bytes_recvd;
}

const std::string& Client::GetUsername() {
    return username;
}

void Client::SetUsername(std::string &username) {
    this->username = username;
}

void Client::SetUsername(const char username[]) {
    this->username = username;
}

const std::string& Client::GetPassword() {
    return password;
}

void Client::SetPassword(std::string &password) {
    this->password = password;
}

