#ifndef NETWORK_H
#define NETWORK_H

#include "common.h"

ssize_t Send(int sockfd, const uint8_t *src, size_t num_bytes);
ssize_t SendRaw(int sockfd, const uint8_t *bytes, ssize_t buflen);
ssize_t Receive(int sockfd, uint8_t *dst, size_t num_bytes);
ssize_t ReceiveRaw(int sockfd, uint8_t **bytes);

#endif