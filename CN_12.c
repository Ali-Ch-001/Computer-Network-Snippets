#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> // Added for close function

int main(void)
{
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];

    // Cleaning the Buffers
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Creating Socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc == -1)
    {
        perror("Could Not Create Socket. Error!!!!!");
        return -1;
    }

    printf("Socket Created\n");

    // Specifying the IP and Port of the server to connect
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Now connecting to the server using connect() from the client side
    if (connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection Failed. Error!!!!!");
        return -1;
    }

    printf("Connected\n");

    // Get Input from the User
    printf("Enter Message: ");
    if (fgets(client_message, sizeof(client_message), stdin) == NULL)
    {
        perror("Error reading user input");
        return -1;
    }
    // Remove the trailing newline character if it exists
    if (client_message[strlen(client_message) - 1] == '\n')
        client_message[strlen(client_message) - 1] = '\0';

    // Send the message to the Server
    if (send(socket_desc, client_message, strlen(client_message), 0) < 0)
    {
        perror("Send Failed. Error!!!!");
        return -1;
    }

    // Receive the message back from the server
    if (recv(socket_desc, server_message, sizeof(server_message), 0) < 0)
    {
        perror("Receive Failed. Error!!!!!");
        return -1;
    }

    printf("Server Message: %s\n", server_message);

    // Closing the Socket
    close(socket_desc);

    return 0;
}

