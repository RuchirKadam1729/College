#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include "mystringutils.c"

#define MAXDATASIZE 1000

#define PORT "3490"

#define BACKLOG 10

#define MAX_USERS 10

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
    int sockfd;
};

struct threadargs
{
    pthread_t thread;

    int i;
    struct user *users;
    int *connected_users;
};

void *thread_listener(void *arg)
{
    struct threadargs threadargs = *(struct threadargs *)(arg);
    int i = threadargs.i;
    int sockfd = threadargs.users[i].sockfd;
    struct user *users = threadargs.users;
    int *connected_users = threadargs.connected_users;
    write(users[i].sockfd, "hi from server", 100);
    while (1)
    {
        int numbytes;
        char client_msg[MAXDATASIZE];
        memset(client_msg, ' ', MAXDATASIZE);
        if ((numbytes = read(sockfd, client_msg, MAXDATASIZE - 1)) == -1)
        {
            *threadargs.connected_users -= 1;
            memset(&threadargs.users[i].name, 0, sizeof(struct user));
            threadargs.users[i].sockfd = -1;
            perror("read");
            pthread_exit(NULL);
        }
        client_msg[numbytes] = '\0';
        printf("server: received '%s'\n", client_msg);

        char *token;
        char *rest;
        token = strtok_r(client_msg, " ", &rest);

        write(users[i].sockfd, client_msg, strlen(client_msg));
        for (int j = 0; j < MAXDATASIZE; j++)
        {
            if (users[j].sockfd != -1)
            {
                write(users[j].sockfd, client_msg, strlen(client_msg));
            }
        }
        // if (equals(token, "/msg"))
        // {
        // token = strtok_r(client_msg, " ", &rest);// on username
        // for (int i = 0; i < MAXDATASIZE; i++)
        // {
        // if (equals(users[i].name, token))
        // {
        // write(users[i].sockfd, rest, strlen(rest));
        // }
        // }
        // char server_msg[MAXDATASIZE];
        // strcpy(server_msg, "no such user");
        // write(sockfd, server_msg, strlen(server_msg));
        // }
        // else {
        // for (int i = 0; i < MAXDATASIZE; i++) {
        // if (users[i].sockfd != -1) {
        // write(users[i].sockfd, client_msg, strlen(client_msg));
        // }
        // }
        // }
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

    int list_sockfd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

#pragma endregion

#pragma region bind and listen

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((list_sockfd = socket(p->ai_family, p->ai_socktype,
                                  p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        if (setsockopt(list_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }

        if (bind(list_sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(list_sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(list_sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler;
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

#pragma endregion

    struct user users[MAXDATASIZE];
    for (int i = 0; i < MAXDATASIZE; i++)
    {
        users[i].sockfd = -1;
        memset(users[i].name, 0, MAXDATASIZE);
    }
    struct threadargs threadargs[MAXDATASIZE];
    int i = 0;
    int connected_users = 0;
    while (connected_users <= MAX_USERS)
    {
        sin_size = sizeof their_addr;
        int sockfd = accept(list_sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (sockfd == -1)
        {
            perror("accept");
            continue;
        }
        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);

        read(sockfd, users[i].name, MAXDATASIZE);
        users[i].sockfd = sockfd;
        printf("server: got connection from %s, %s, sockfd %d\n", users[i].name, s, users[i].sockfd);
        threadargs[i].users = users;
        threadargs[i].i = i;
        threadargs[i].connected_users = &connected_users;

        pthread_create(&threadargs[i].thread, NULL, thread_listener, &threadargs[i]);
        connected_users++;
        i++;
    }
    for (size_t t = 0; t < i; t++)
    {
        pthread_join(threadargs[t].thread, NULL);
    }

    return 0;
}
