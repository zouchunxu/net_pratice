#include "lib/common.h"

static int count;

static void sig_int(int signo) {
    printf("\n received %d datagrams \n", count);
    exit(0);
}

int main() {
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr));

    int r = listen(listenfd, 1024);
    if (r < 0) {
        error(1, errno, "listen failed");
    }

    signal(SIGINT, sig_int);
    signal(SIGPIPE, SIG_DFL);

    struct sockaddr_in client_addr;
    socklen_t socklen = sizeof(client_addr);
    int connfd = accept(listenfd, (struct sockaddr *) &client_addr, &socklen);

    char message[MAXLINE];
    count = 0;

    send(connfd, "hello world", sizeof("hello world") + 1, 0);

    while (true) {
        int n = read(connfd, message, MAXLINE);
        if (n < 0) {
            error(1, errno, "error read");
        } else if (n == 0) {
            error(1, 0, "client closed \n");
        }
        message[n] = 0;
        printf("received %d bytes: %s\n", n, message);
        count++;

        char send_line[MAXLINE];
        sprintf(send_line, "Hi,%s", message);

        sleep(5);

        int write_nc = send(connfd, send_line, strlen(send_line), 0);
        printf("send bytes: %zu \n", write_nc);
        if (write_nc < 0) {
            error(1, errno, "error write");
        }
    }

}