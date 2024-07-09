#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];

    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket < 0) {
        printf("Could not create socket. Error!!!!!\n");
        return -1;
    }

    printf("Socket Created\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Connection Failed. Error!!!!!\n");
        return -1;
    }

    printf("Connected\n");

    printf("Enter your name and CNIC (e.g., Name/CNIC): ");
    gets(client_message);

    send(client_socket, client_message, strlen(client_message), 0);

    recv(client_socket, server_message, sizeof(server_message), 0);
    printf("Server Message: %s\n", server_message);

    close(client_socket);

    return 0;
}

