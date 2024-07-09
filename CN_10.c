#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>

#define MAX_CLIENTS 3
#define PORT 12345

void *handleClient(void *arg) {
    int client_socket = *((int *)arg);
    char buffer[1024];
    int disconnect = 0;

    while (!disconnect) {
        memset(buffer, 0, sizeof(buffer));

        if (recv(client_socket, buffer, sizeof(buffer), 0) <= 0) {
            perror("Receive error");
            break;
        }

        printf("Client says: %s", buffer);

        if (strcmp(buffer, "DISCONNECT\n") == 0) {
            disconnect = 1;
        }

        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t thread_id;
    int client_count = 0;

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind error");
        exit(1);
    }

    // Listen for connections
    if (listen(server_socket, 5) == -1) {
        perror("Listen error");
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        if (client_count >= MAX_CLIENTS) {
            // Server full
            printf("Server full. Rejecting connection...\n");
            client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
            send(client_socket, "Server full\n", 13, 0);
            close(client_socket);
        } else {
            client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
            client_count++;

            if (pthread_create(&thread_id, NULL, handleClient, (void *)&client_socket) != 0) {
                perror("Thread creation error");
                exit(1);
            }

            pthread_detach(thread_id);
        }
    }

    close(server_socket);
    return 0;
}

