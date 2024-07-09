
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// Function to check if a character is a vowel
int isVowel(char c) {
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}

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

int main(void) {
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];

    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0) {
        perror("Could Not Create Socket. Error!!!!!\n");
        return -1;
    }

    printf("Socket Created\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection Failed. Error!!!!!\n");
        return -1;
    }

    printf("Connected\n");

    // Get Input from the User
    printf("Enter Message: ");
    fgets(client_message, sizeof(client_message), stdin);

    if (send(socket_desc, client_message, strlen(client_message), 0) < 0) {
        perror("Send Failed. Error!!!!\n");
        return -1;
    }

    if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0) {
        perror("Receive Failed. Error!!!!!\n");
        return -1;
    }

    printf("Server Message: %s\n", server_message);

    // Process and invert words with no vowels
    char result_message[2000];
    memset(result_message, '\0', sizeof(result_message));
    
    char *token = strtok(server_message, " ");
    while (token != NULL) {
        int hasVowel = 0;
        int i;
        for (i = 0; token[i] != '\0'; i++) {
            if (isVowel(token[i])) {
                hasVowel = 1;
                break;
            }
        }
        if (!hasVowel) {
            invertString(token);
        }
        strcat(result_message, token);
        strcat(result_message, " ");
        token = strtok(NULL, " ");
    }

    printf("Client Message: %s\n", result_message);

    close(socket_desc);
    return 0;
}

