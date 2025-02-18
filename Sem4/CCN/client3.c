#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080

int client_socket;

void *receive_messages(void *arg) {
    char buffer[1024];
    int bytes_read;

    while ((bytes_read = read(client_socket, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("Received: %s", buffer);
    }
    return NULL;
}

int main() {
    struct sockaddr_in address;
    char buffer[1024];

    // Creating socket file descriptor
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "192.168.0.195", &address.sin_addr) <= 0)
    {
        perror("Invalid address/ Address not supported");
        return -1;
    }

    // Connect to the chat server
    if (connect(client_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Connection failed");
        return -1;
    }

    pthread_t tid;
    pthread_create(&tid, NULL, receive_messages, NULL);

    while (1) {
        char message[1024];
        printf("Enter message: ");
        fgets(message, sizeof(message), stdin);
        send(client_socket, message, strlen(message), 0);
    }

    close(client_socket);
    return 0;
}