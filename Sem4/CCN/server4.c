#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <ifaddrs.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define USERNAME_LENGTH 32

typedef struct {
    int socket;
    char username[USERNAME_LENGTH];
} Client;

Client clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void list_users(int client_socket) {
    char user_list[1024] = "Connected users:\n";
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        strcat(user_list, clients[i].username);
        strcat(user_list, "\n");
    }
    pthread_mutex_unlock(&clients_mutex);
    send(client_socket, user_list, strlen(user_list), 0);
}

void send_private_message(char *username, char *message, int sender_socket) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].username, username) == 0) {
            send(clients[i].socket, message, strlen(message), 0);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg) {
    Client client = *(Client *)arg;
    char buffer[1024];
    int bytes_read;

    while ((bytes_read = read(client.socket, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("%s: %s", client.username, buffer);

        if (strncmp(buffer, "/msg ", 5) == 0) {
            char *token = strtok(buffer + 5, " ");
            if (token) {
                char recipient[USERNAME_LENGTH];
                strncpy(recipient, token, USERNAME_LENGTH);
                char *message = strtok(NULL, "\0");
                if (message) {
                    char private_message[1024];
                    snprintf(private_message, sizeof(private_message), "[Private from %s]: %s", client.username, message);
                    send_private_message(recipient, private_message, client.socket);
                }
            }
        } else if (strcmp(buffer, "/users\n") == 0) {
            list_users(client.socket);
        } else {
            // Broadcast to all other clients
            pthread_mutex_lock(&clients_mutex);
            for (int i = 0; i < client_count; i++) {
                if (clients[i].socket != client.socket) {
                    send(clients[i].socket, buffer, bytes_read, 0);
                }
            }
            pthread_mutex_unlock(&clients_mutex);
        }
    }

    // Remove client from the list
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].socket == client.socket) {
            clients[i] = clients[--client_count]; // Replace with last client
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    close(client.socket);
    return NULL;
}

void get_ipconfig() {
    struct ifaddrs *ifaddr, *ifa;
    getifaddrs(&ifaddr);
    printf("IP Configuration:\n");
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
            char ip[INET_ADDRSTRLEN];
            getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), ip, sizeof(ip), NULL, 0, NI_NUMERICHOST);
            printf("%s: %s\n", ifa->ifa_name, ip);
        }
    }
    freeifaddrs(ifaddr);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsock opt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Chat server started on port %d\n", PORT);
    get_ipconfig(); // Display IP configuration on server start

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        pthread_mutex_lock(&clients_mutex);
        if (client_count < MAX_CLIENTS) {
            Client new_client;
            new_client.socket = new_socket;
            snprintf(new_client.username, USERNAME_LENGTH, "User %d", client_count + 1); // Assign a default username
            clients[client_count++] = new_client;

            pthread_t tid;
            pthread_create(&tid, NULL, handle_client, (void *)&clients[client_count - 1]);
        } else {
            printf("Max clients reached. Connection rejected.\n");
            close(new_socket);
        }
        pthread_mutex_unlock(&clients_mutex);
    }

    // Closing the listening socket (THIS LINE WILL NEVER BE REACHED)
    close(server_fd);
    return 0;
}