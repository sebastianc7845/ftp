#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_STRLEN 512
#define MAX_RECV 512
#define CMD_LEN 4

#define SFTP_PORT 41099
#define MAX_CLIENTS 1

#define STDIN_POLL 0
#define LISTEN_POLL 1

#define GET_POLL_IDX(user_idx) (user_idx + 2)
#define GET_USER_IDX(poll_idx) (poll_idx - 2)

#define LOGF(format, ...) fprintf(stderr, "%s:%d:%s(): " format "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define LOGERRNO LOGF("%s", strerror(errno))
#define UNUSED(arg) ((void)arg)

#endif