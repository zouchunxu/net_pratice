#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <zconf.h>
#include <errno.h>

#define SERVER_PORT 9798

ssize_t readn(int, void *, size_t);

void read_data(int);

int main() {
    int listenfd, connfd;
    socklen_t client;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVER_PORT);

    bind(listenfd, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    listen(listenfd, 1024);

    while (true) {
        client = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &client);
        read_data(connfd);
        close(connfd);
    }

    return 0;
}

void read_data(int sockfd) {
    char buf[1024];
    ssize_t n;
    int time = 0;
    while (true) {
        fprintf(stdout, "block in read \n");
        if ((n = readn(sockfd, (void *) buf, 1024)) == 0) {
            return;
        }
        time++;
        fprintf(stdout, "1K read for %d \n", time);
//        sleep(1);
    }
}

ssize_t readn(int fd, void *vptr, size_t size) {
    size_t nleft;
    ssize_t nread;
    char *ptr;

    ptr = (char *) vptr;
    nleft = size;

    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR) {
                nread = 0;
            } else {
                return (-1);
            }
        } else if (nread == 0) {
            break;
        }

        nleft -= nread;
        ptr += nread;
    }
    return size - nleft;
}
