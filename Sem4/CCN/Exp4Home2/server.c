#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "mystringutils.c"

#define MAXDATASIZE 1000
#define PORT        "3490"
#define BACKLOG     10
#define MAX_USERS   2

#pragma region utilities
void sigchld_handler(int s)
{
    (void)s;

    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;

    errno = saved_errno;
}

#pragma endregion

struct user
{
    char name[MAXDATASIZE];
    int  sockfd;
    char s[INET6_ADDRSTRLEN];
};

struct threadargs
{
    pthread_t thread;

    int                i;
    struct user        user;
    struct threadargs *allthreadargs;
    int               *nconnected_users;
};

void *thread_service(void *arg)
{
    struct threadargs *threadargs = (struct threadargs *)(arg);

    int                i                = threadargs->i;
    int                sockfd           = threadargs->user.sockfd;
    char              *name             = threadargs->user.name;
    int               *nconnected_users = threadargs->nconnected_users;
    struct threadargs *allthreadargs    = threadargs->allthreadargs;
    char              *s                = threadargs->user.s;
    while (1) {
        int  numbytes;
        char client_msg[MAXDATASIZE];
        memset(client_msg, 0, MAXDATASIZE);
        numbytes = read(sockfd, client_msg, MAXDATASIZE - 1);
        if (numbytes <= 0) {
            *threadargs->nconnected_users -= 1;
            threadargs->user.sockfd        = -1;
            memset(threadargs->user.name, 0, MAXDATASIZE);
            if (numbytes == 0)
                printf("read: client closed remote connection.\n");
            else
                perror("read");
            close(sockfd);
            pthread_exit(NULL);
        }

        client_msg[numbytes] = '\0';
        printf("server: received '%s'\n", client_msg);

        char server_response[MAXDATASIZE];
        memset(server_response, 0, MAXDATASIZE);
        char tokenized_client_msg[strlen(client_msg) + 1];
        strncpy(tokenized_client_msg, client_msg, strlen(client_msg) + 1);
        tokenized_client_msg[strlen(client_msg)] = '\0';
        char *token, *rest;
        token = strtok_r(tokenized_client_msg, " ", &rest);
        if (equals(token, "/msg")) {
            token = strtok_r(NULL, " ", &rest);
            for (int j = 0; j < MAXDATASIZE; j++) {
                if (allthreadargs[j].user.sockfd != -1 &&
                    equals(token, allthreadargs[j].user.name)) {
                    char server_response[MAXDATASIZE];
                    sprintf(server_response, "\e[3m%s whispers to you: %s\e[0m",
                            name, rest);
                    write(allthreadargs[j].user.sockfd, server_response,
                          strlen(server_response));
                    break;
                }
            }
        } else if (equals(token, "/list")) {
            printf("made it\n");
            char connected_users[MAXDATASIZE];
            memset(connected_users, 0, MAXDATASIZE);
            for (int j = 0; j < MAXDATASIZE; j++) {
                if (allthreadargs[j].user.sockfd != -1) {
                    char current[MAXDATASIZE];
                    sprintf(current, "%s  ", allthreadargs[j].user.name);
                    strncat(connected_users, current, strlen(current) + 1);
                }
            }
            sprintf(server_response, "\e[1mConnected users: %s\n\e[0m",
                    connected_users);
            write(sockfd, server_response, strlen(server_response));
        } else {
            for (int j = 0; j < MAXDATASIZE; j++) {
                if (allthreadargs[j].user.sockfd != -1) {
                    sprintf(server_response, "(%s)[%s]: %s", s, name,
                            client_msg);
                    write(allthreadargs[j].user.sockfd, server_response,
                          strlen(server_response) + 1);
                }
            }
        }
    }
    close(sockfd);
    pthread_exit(NULL);
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in *)sa)->sin_addr);
    else
        return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main()
{
#pragma region declarations

    int                     list_sockfd;
    struct addrinfo         hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t               sin_size;
    struct sigaction        sa;
    int                     yes = 1;
    // char                    s[INET6_ADDRSTRLEN];
    int                     rv;
    int                     nconnected_users = 0;

#pragma endregion

#pragma region bind and listen

    memset(&hints, 0, sizeof hints);
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((list_sockfd =
                 socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(list_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(list_sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(list_sockfd);
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

    if (listen(list_sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    // I don't know exactly what this does- smth smth zombie process reaping
    sa.sa_handler = sigchld_handler;
    sa.sa_flags   = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

#pragma endregion

    struct threadargs threadargs[MAXDATASIZE];
    for (int i = 0; i < MAXDATASIZE; i++) {
        threadargs[i].user.sockfd      = -1;
        threadargs[i].nconnected_users = &nconnected_users;
        threadargs[i].allthreadargs    = threadargs;
        memset(threadargs[i].user.name, 0, MAXDATASIZE);
    }

    int i = 0;
    while (1) {
        sin_size = sizeof their_addr;
        int sockfd =
            accept(list_sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (sockfd == -1) {
            perror("accept");
            continue;
        }

        if (nconnected_users >= MAX_USERS) {
            write(sockfd, "server full", 12);
            close(sockfd);
            continue;
        } else {
            nconnected_users++;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  threadargs[i].user.s, sizeof threadargs[i].user.s);

        read(sockfd, threadargs[i].user.name, MAXDATASIZE);
        threadargs[i].user.sockfd = sockfd;
        threadargs[i].i           = i;
        printf("server: got connection from %s, %s, sockfd %d\n",
               threadargs[i].user.name, threadargs[i].user.s,
               threadargs[i].user.sockfd);

        pthread_create(&threadargs[i].thread, NULL, thread_service,
                       &threadargs[i]);

        i = (i + 1) % MAXDATASIZE;
    }
    for (size_t t = 0; t < i; t++)
        pthread_join(threadargs[t].thread, NULL);

    return 0;
}
