#include "lib/common.h"

#include <iostream>

static int count;

static void sig_int(int signo) {
    printf("\nreceived %d datagrams\n", count);
    exit(0);
}

int main() {
    std::cout << "hello world" << std::endl;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERV_PORT);

    bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr));

    int rt2 = listen(listenfd, LISTENQ);
    if (rt2 < 0) {
        error(1, errno, "listen failed");
    }

    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);

    int connfd = accept(listenfd, (struct sockaddr *) &client, &client_len);
    if (connfd < 0) {
        error(1, errno, "bind failed");
    }

    char message[MAXLINE];
    count = 0;

    while (true) {
        int n = read(connfd, message, MAXLINE);
        if (n < 0) {
            error(1, errno, "read failed");
        } else if (n == 0) {
            error(1, 0, "client closed \n");
        }
        message[n] = 0;
        printf("received %d bytes: %s\n", n, message);
        count++;

        char send_line[MAXLINE];
        sprintf(send_line, "Hi, %s", message);

        sleep(5);

        int write_nc = send(connfd, send_line, strlen(send_line), 0);
        printf("send bytes: %zu \n", write_nc);
        if (write_nc < 0) {
            error(1, errno, "error write");
        }
    }

}