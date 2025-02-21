#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT "3490"

#define MAXDATASIZE 1000

// utility
int strlen_newline(char *str)
{
    int len = 0;
    while (str[len] != '\n' && str[len] != '\0')
    {
        len++;
    }
    return len;
}
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in *)sa)->sin_addr);

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void write_to_file(const char *filename, const char *str)
{
    FILE *client_convo_file = fopen(filename, "a");
    fputs(str, client_convo_file);
    fclose(client_convo_file);
}

void get_file_content(char *buf, const char *filename)
{
    char raw_buf[MAXDATASIZE];
    FILE *client_convo_file = fopen(filename, "r");
    int j = 0;
    while (fgets(raw_buf, MAXDATASIZE, client_convo_file))
    {
        int i = 0;

        while (1)
        {
            if (raw_buf[i] == '\0')
                continue;
            if (raw_buf[i] == '\n')
            {
                buf[j++] = raw_buf[i];
                break;
            }

            buf[j++] = raw_buf[i];
            i++;
        }
    }
    fclose(client_convo_file);
}

struct threadargs
{
    pthread_t thread;
    int sockfd;
    char *name;
};

void *thread_listener(void *arg)
{
    struct threadargs threadargs = *(struct threadargs *)arg;
    char filename[100];
    sprintf(filename, "%s%s", threadargs.name, PORT);
    int sockfd = threadargs.sockfd;

    while (1)
    {
        int numbytes;
        char server_msg[MAXDATASIZE];

        memset(server_msg, 0, MAXDATASIZE);
        if ((numbytes = read(sockfd, server_msg, MAXDATASIZE - 1)) == -1)
        {
            perror("read");
            pthread_exit(NULL);
        }
        server_msg[numbytes] = '\0';
        write_to_file(filename, server_msg);
        char buf[MAXDATASIZE];
        get_file_content(buf, filename);
        system("clear");
        printf("%s", buf);
    }
}

int getname(char name[])
{
    printf("Enter username: ");
    fgets(name, 100, stdin);
    name[strlen_newline(name)] = '\0';
    return strlen_newline(name);
}

int main(int argc, char *argv[])
{
    char name[100];
    getname(name);

    int sockfd;

    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2)
    {
        fprintf(stderr, "usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            perror("client: connect");
            close(sockfd);
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
              s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo);

    write(sockfd, name, strlen(name));
    char buf[100];
    read(sockfd, buf, 100);
    puts(buf);
    struct threadargs threadargs;
    threadargs.sockfd = sockfd;
    threadargs.name = name;
    pthread_create(&threadargs.thread, NULL, thread_listener, &threadargs);
    while (1)
    {
        char client_msg[MAXDATASIZE];
        char usr_msg[MAXDATASIZE];
        fgets(usr_msg, MAXDATASIZE, stdin);
        sprintf(client_msg, "[%s]: %s", name, usr_msg);
        write(sockfd, client_msg, strlen(client_msg));
    }
    close(sockfd);

    return 0;
}
