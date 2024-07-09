#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// Function to invert a string
void invertString(char *str) {
    int len = strlen(str);
    int i;
    int j;
    for ( i = 0, j = len - 1; i < j; i++, j--) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

// Function to process and invert the words
void processAndInvertWords(char *input, char *output) {
    char *token = strtok(input, " ");
    while (token != NULL) {
        int hasVowel = 0;
        int i;
        for (i = 0; token[i] != '\0'; i++) {
            if (strchr("AEIOUaeiou", token[i]) != NULL) {
                hasVowel = 1;
                break;
            }
        }
        if (hasVowel) {
            invertString(token);
        }
        strcat(output, token);
        strcat(output, " ");
        token = strtok(NULL, " ");
    }
}

int main(void) {
    int socket_desc, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;
    char client_message[2000], server_message[2000];

    memset(client_message, '\0', sizeof(client_message));
    memset(server_message, '\0', sizeof(server_message));

    // Creating Socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0) {
        perror("Could Not Create Socket. Error!!!!!");
        return -1;
    }

    printf("Socket Created\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Binding Function
    if (bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind Failed. Error!!!!!");
        return -1;
    }

    printf("Bind Done\n");

    if (listen(socket_desc, 1) < 0) {
        perror("Listening Failed. Error!!!!!");
        return -1;
    }

    printf("Listening for Incoming Connections.....\n");

    while (1) {
        client_size = sizeof(client_addr);
        client_sock = accept(socket_desc, (struct sockaddr *)&client_addr, &client_size);

        if (client_sock < 0) {
            perror("Accept Failed. Error!!!!!!");
            continue; // Continue listening for the next connection
        }

        printf("Client Connected with IP: %s and Port No: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        if (recv(client_sock, client_message, sizeof(client_message), 0) < 0) {
            perror("Receive Failed. Error!!!!!\n");
            close(client_sock);
            continue; // Continue listening for the next connection
        }

        printf("Received from Client: %s\n", client_message);

        // Process and invert words
        char inverted_message[2000] = {'\0'};
        processAndInvertWords(client_message, inverted_message);

        printf("Inverted Message: %s\n", inverted_message);

        // Send the inverted message back to the client
        if (send(client_sock, inverted_message, strlen(inverted_message), 0) < 0) {
            perror("Send Failed. Error!!!!!\n");
        }

        memset(client_message, '\0', sizeof(client_message));
        memset(inverted_message, '\0', sizeof(inverted_message));

        // Close the client socket
        close(client_sock);
    }

    // Closing the Server Socket
    close(socket_desc);

    return 0;
}

