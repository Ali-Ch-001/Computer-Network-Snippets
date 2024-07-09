#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];
    int disconnect = 0;

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection error");
        exit(1);
    }

    printf("Connected to server. Enter 'DISCONNECT' to exit.\n");

    while (!disconnect) {
        printf("Enter a message: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(client_socket, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "DISCONNECT\n") == 0) {
            disconnect = 1;
        }

        memset(buffer, 0, sizeof(buffer));

        if (recv(client_socket, buffer, sizeof(buffer), 0) <= 0) {
            perror("Receive error");
            break;
        }

        printf("Server says: %s", buffer);
    }

    close(client_socket);
    return 0;
}

