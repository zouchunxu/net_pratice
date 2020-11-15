//
// Created by 邹春旭 on 2020/11/15.
//

#include "lib/common.h"
#include "message_objecte.h"

#define MAMAXLINE 4096
#define KEEP_ALIVE_TIME 10
#define KEEP_ALIVE_INTERVAL 3
#define KEEP_ALIVE_PROBETIMES 3

int main() {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    inet_pton(socket_fd, "127.0.0.1", &server_addr.sin_addr);

    socklen_t server_len = sizeof(server_addr);
    int connect_rt = connect(socket_fd, (struct sockaddr *) &server_addr, server_len);
    if (connect_rt < 0) {
        error(1, errno, "connect failed");
    }

    char recv_line[MAXLINE + 1];
    int n;
    fd_set readmask;
    fd_set allreads;

    struct timeval tv;
    int heartbeats = 0;

    tv.tv_sec = KEEP_ALIVE_TIME;
    tv.tv_usec = 0;

    messageObject messageObject1;

    FD_ZERO(&allreads);
    FD_SET(0, &allreads);
    FD_SET(socket_fd, &allreads);

    while (true) {
        readmask = allreads;
        int rc = select(socket_fd + 1, &readmask, NULL, NULL, &tv);
        if (rc < 0) {
            error(1, errno, "select failed");
        }
        if (rc == 0) {
            if (++heartbeats > KEEP_ALIVE_PROBETIMES) {
                error(1, 0, "connection dead\n");
            }
            printf("sending heartbeat #%d\n", heartbeats);
            messageObject1.type = htonl(MSG_PING);
            rc = send(socket_fd, (char *) &messageObject1, sizeof(messageObject1), 0);
            if (rc < 0) {
                error(1, errno, "send failure");
            }
            tv.tv_sec = KEEP_ALIVE_INTERVAL;
            continue;
        }
        if (FD_ISSET(socket_fd, &readmask)) {
            n = read(socket_fd, recv_line, MAXLINE);
            if (n < 0) {
                error(1, errno, "read error");
            } else if (n == 0) {
                error(1, 0, "server terminated \n");
            }
            printf("received heartbeat,make heartbeats to 0 \n");
            heartbeats = 0;
            tv.tv_sec = KEEP_ALIVE_TIME;
        }
    }
}
