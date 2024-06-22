#include "network.hpp"

ssize_t Send(int sockfd, const uint8_t *src, size_t num_bytes) {
    ssize_t total_bytes_sent = 0, bytes_sent = 0;

    bytes_sent = send(sockfd, src, num_bytes, 0);
    total_bytes_sent += bytes_sent;

    while (total_bytes_sent < (ssize_t)num_bytes && bytes_sent >= 0) {
        bytes_sent = send(sockfd, src + total_bytes_sent, num_bytes - total_bytes_sent, 0);

        total_bytes_sent += bytes_sent;
    }

    if (bytes_sent < 0) {
        LOGERRNO;
        total_bytes_sent = bytes_sent;
    }

    return total_bytes_sent;
}

// Send SFTP message as raw bytes
ssize_t SendRaw(int sockfd, const uint8_t *bytes, ssize_t buflen) {
    uint16_t netbyte_len = htons((uint16_t)buflen);
    ssize_t total_bytes_sent = 0, bytes_sent;

    if ((bytes_sent = Send(sockfd, (uint8_t *) &netbyte_len, sizeof(uint16_t)) < 0)) {
        LOGF("%s", "failed to send msglen");
        return bytes_sent;
    }

    total_bytes_sent += bytes_sent;

    if ((bytes_sent = Send(sockfd, bytes, buflen) < 0)) {
        LOGF("%s", "failed to send bytes");
        return bytes_sent;
    }

    total_bytes_sent += bytes_sent;

    return total_bytes_sent;
}

ssize_t Receive(int sockfd, uint8_t *dst, size_t num_bytes) {
    ssize_t total_bytes_recvd = 0;
    ssize_t bytes_recvd = 0;

    bytes_recvd = recv(sockfd, dst + total_bytes_recvd, num_bytes, 0);
    total_bytes_recvd += bytes_recvd;

    while (total_bytes_recvd < (ssize_t)num_bytes && bytes_recvd > 0) {
        bytes_recvd = recv(sockfd, dst + total_bytes_recvd, num_bytes - total_bytes_recvd, 0);

        total_bytes_recvd += bytes_recvd;
    }

    if (bytes_recvd == 0) {
        LOGF("peer @ sockfd %d closed connection", sockfd);
        total_bytes_recvd = bytes_recvd;
    }
    else if (bytes_recvd < 0) {
        LOGERRNO;
        total_bytes_recvd = bytes_recvd;
    }

    return total_bytes_recvd;
}

// Receive raw SFTP message
ssize_t ReceiveRaw(int sockfd, uint8_t **bytes) {
    uint16_t netbyte_len, len;
    ssize_t bytes_recvd = Receive(sockfd, (uint8_t *)(&netbyte_len), sizeof(uint16_t));

    if (bytes_recvd <= 0)
        return bytes_recvd;


    len = ntohs(netbyte_len);
    *bytes = new uint8_t[len];

    if ((bytes_recvd = Receive(sockfd, *bytes, len)) <= 0) {

        delete[] bytes;
        *bytes = NULL;
        return bytes_recvd;
    }

    return bytes_recvd;
}