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

#define PORT "3490"

#define BACKLOG 10

#define MAX_USERS 10

#define MAXDATASIZE 1000

void sigchld_handler(int s)
{
    (void)s;

    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

void* get_in_addr(struct sockaddr* sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    else
        return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int getconvofile_content(char* name, char buf[]);

int main(void)
{
#pragma region declarations

    int list_sockfd, user_sockfd[MAX_USERS];
    struct addrinfo hints, * servinfo, * p;
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

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((list_sockfd = socket(p->ai_family, p->ai_socktype,
            p->ai_protocol)) == -1) {
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

    sa.sa_handler = sigchld_handler;
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

#pragma endregion

    int i = 0;
    FILE* megabuf;                     // MEGA buffer holding ENTIRE convo lol

    while (i < MAX_USERS) {
        sin_size = sizeof their_addr;
        user_sockfd[i] = accept(list_sockfd, (struct sockaddr*)&their_addr, &sin_size);
        if (user_sockfd[i] == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr*)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);
#pragma region update connected_user count
        FILE* connected_users = fopen("connected_users.txt", "a");
        fprintf(connected_users, "%d\n", user_sockfd[i]);
        fclose(connected_users);
#pragma endregion
        if (!fork()) {
            close(list_sockfd);

            while (1) {
                char convo[MAXDATASIZE];
                memset(convo, ' ', MAXDATASIZE);
                int convo_length = getconvofile_content("convo.txt", convo);

                // char server_msg[MAXDATASIZE];
                // memcpy(server_msg, client_msg, numbytes);

                if (write(user_sockfd[i], convo, convo_length) == -1)
                    perror("write");

                // FILE* connected_users = fopen("connected_users.txt", "r");
                // char user_sockfd_i[100];
                // while (fget  9s(user_sockfd_i, 100, connected_users)) {
                //     int user_sock = atoi(user_sockfd_i);
                //     if (write(user_sock, convo, convo_length) == -1)
                //         perror("write");
                // }

                // fclose(connected_users);
                //^^MIND BLOWN!!!


                int numbytes;
                char client_msg[MAXDATASIZE];
                memset(client_msg, ' ', MAXDATASIZE);
                if ((numbytes = read(user_sockfd[i], client_msg, MAXDATASIZE - 1)) == -1) {
                    perror("read");
                    exit(1);
                }
                client_msg[numbytes] = '\0';
                printf("server: received '%s'\n", client_msg);


                if ((megabuf = fopen("convo.txt", "a")) == NULL) {
                    perror("fopen");continue;
                }
                fprintf(megabuf, "%s\n", client_msg);
                fclose(megabuf);
            }
            close(user_sockfd[i]);
            exit(0);
        }

        i++;

    }
    fclose(megabuf);
    fputc('\0', fopen("connected_users.txt", "w"));
    return 0;
}




int getconvofile_content(char* name, char convo[])
{
    FILE* megabuf = fopen(name, "r");
    char buf[MAXDATASIZE];
    memset(buf, ' ', MAXDATASIZE);
    size_t pos = 0;
    while (fgets(buf, MAXDATASIZE, megabuf)) {
        int len = strlen(buf);
        memcpy(convo + pos, buf, len);
        pos += len;
    }
    convo[pos] = '\0';
    fclose(megabuf);
    return pos + 1;
}