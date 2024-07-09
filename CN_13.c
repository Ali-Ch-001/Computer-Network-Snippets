#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{
    int socket_desc, client_sock, client_size;
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];

    // Counter for client IDs
    int client_id = 0;

    // Cleaning the Buffers
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));

    // Creating Socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc < 0)
    {
        perror("Could Not Create Socket. Error!!!!!");
        return -1;
    }

    printf("Socket Created\n");

    // Binding IP and Port to socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // BINDING FUNCTION
    if (bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind Failed. Error!!!!!");
        return -1;
    }

    printf("Bind Done\n");

    // Put the socket into Listening State
    if (listen(socket_desc, 1) < 0)
    {
        perror("Listening Failed. Error!!!!!");
        return -1;
    }

    printf("Listening for Incoming Connections.....\n");

    while (1)
    {
        // Accept the incoming Connections
        client_size = sizeof(client_addr);
        client_sock = accept(socket_desc, (struct sockaddr *)&client_addr, &client_size);

        if (client_sock < 0)
        {
            perror("Accept Failed. Error!!!!!!");
            continue; // Continue listening for the next connection
        }

        client_id++;

        printf("Client%d Connected with IP: %s and Port No: %i\n", client_id, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Receive the message from the client
        if (recv(client_sock, client_message, sizeof(client_message), 0) < 0)
        {
            perror("Receive Failed. Error!!!!!\n");
            return -1;
        }

        printf("Client%d sends: \"%s\"\n", client_id, client_message);

        // Prepare the server response
        snprintf(server_message, sizeof(server_message), "Hello I am server. Your received id is %d", client_id);

        // Send the message back to client
        if (send(client_sock, server_message, strlen(server_message), 0) < 0)
        {
            perror("Send Failed. Error!!!!!\n");
            return -1;
        }

        memset(server_message, '\0', sizeof(server_message));
        memset(client_message, '\0', sizeof(client_message));

        // Closing the Client Socket
        close(client_sock);
    }

    // Note: The server will never reach this point because it listens forever.

    // Closing the Server Socket
    close(socket_desc);

    return 0;
}

