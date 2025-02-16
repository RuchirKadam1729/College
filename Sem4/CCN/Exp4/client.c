#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    struct addrinfo hints, *res;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET6;

    getaddrinfo("fe80::873f:ef2c:c923:9029", "3490", &hints, &res);

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    bind(sockfd, res->ai_addr, res->ai_addrlen);

    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
    {
        perror("setsockopt");
        exit(1);
    }
    connect(sockfd, res->ai_addr, res->ai_addrlen);

    char buffer[100];
    recv(sockfd, buffer, 100,0);
    send(sockfd, "hi from client", 100, 0);
    return 0;
}
