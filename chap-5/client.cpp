#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MESSAGE_SIZE 10240000
#define SERVER_PORT 9798
#define SERVER_ADDR "127.0.0.1"

using namespace std;

void send_data(int);

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &servaddr.sin_addr);
    connect(sockfd, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    send_data(sockfd);
    return 0;
}

void send_data(int sockfd) {
    string query(MESSAGE_SIZE, 'a');

    const char *cp;
    cp = query.c_str();
    long remaining = (long) strlen(cp);

    while (remaining) {
        long n = send(sockfd, cp, remaining, 0);
        fprintf(stdout, "send int buffer %ld \n", n);
        if(n < 0) {
            perror("send");
            return;
        }
        remaining -= n;
        cp += n;
    }
}