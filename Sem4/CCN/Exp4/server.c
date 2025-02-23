#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PORT "3490"

#define BACKLOG 10

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in *)sa)->sin_addr);

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

struct resultset
{
    int chars;
    int words;
    int sentences;
};

struct resultset *process_string(char *start, struct resultset *resultset)
{
    char *reader = start;
    int   i      = 0;
    while (reader[i] == ' ') {
        i++;
    }
    if (reader[i] != '\n') {
        resultset->words = 1;
    }

    while (reader[i] != '\n' && reader[i] != '\0') {
        resultset->chars++;
        if (reader[i] == ' ') {
            resultset->words++;
            while (reader[i] == ' ') {
                i++;
            }
            if (reader[i] == '\n' || reader[i] == '\0') {
                break;
            }
        }
        if (reader[i] == '.') {
            resultset->sentences++;
        }
        i++;
    }

    return resultset;
}

void sprint_resultset(char *buf, struct resultset resultset)
{
    sprintf(buf, "chars: %d, words: %d, sentences: %d", resultset.chars,
            resultset.words, resultset.sentences);
}

int main(void)
{
    int                     sockfd, new_fd;
    struct addrinfo         hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t               sin_size;
    struct sigaction        sa;
    int                     yes = 1;
    char                    s[INET6_ADDRSTRLEN];
    int                     rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) ==
            -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) ==
            -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL) {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while (1) {
        sin_size = sizeof their_addr;
        new_fd   = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);

        if (!fork()) {
            close(sockfd);
#define MAX_SIZE 5000
            char client_msg[MAX_SIZE];
            char server_msg[MAX_SIZE];
            int  numbytes;
            while ((numbytes =
                        recv(new_fd, client_msg, sizeof(client_msg), 0)) != 0) {
                if (numbytes == -1) {
                    perror("recv");
                    break;
                }

                client_msg[numbytes] = '\0';
                struct resultset resultset;
                process_string(client_msg, &resultset);
                sprint_resultset(server_msg, resultset);

                send(new_fd, server_msg, sizeof(server_msg), 0);
            }
            close(new_fd);
            exit(0);
        }
        close(new_fd);
    }

    return 0;
}
