#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 5
#define VOTERS_FILE "Voters.txt"
#define CANDIDATES_FILE "Candidates_List.txt"
#define VOTES_FILE "Votes_Record.txt"

typedef struct {
    int socket;
    struct sockaddr_in address;
    char voter_name[200];
    int authenticated;
} client_info;

// Function to handle a client's vote
void handle_vote(client_info *client, FILE *votes_record) {
    char poll_symbol[10];
    char candidate_name[200];

    // Load and display candidates from Candidates_List.txt
    FILE *candidates_file = fopen(CANDIDATES_FILE, "r");
    if (candidates_file == NULL) {
        perror("Error opening candidates file");
        return;
    }

    printf("Candidates:\n");
    while (fgets(candidate_name, sizeof(candidate_name), candidates_file)) {
        printf("%s", candidate_name);
    }
    fclose(candidates_file);

    // Prompt the client for their vote
    printf("\n%s, please enter the poll symbol of your chosen candidate: ", client->voter_name);
    scanf("%s", poll_symbol);

    // Record the vote in Votes_Record.txt
    fprintf(votes_record, "%s: %s\n", client->voter_name, poll_symbol);
    fflush(votes_record);

    // Send a confirmation message to the client
    char confirmation_msg[200];
    snprintf(confirmation_msg, sizeof(confirmation_msg), "Thank you, %s! Your vote for candidate %s has been recorded.\n", client->voter_name, poll_symbol);
    send(client->socket, confirmation_msg, strlen(confirmation_msg), 0);
}

// Function to handle each client
void *client_handler(void *arg) {
    client_info *client = (client_info *)arg;
    FILE *votes_record = fopen(VOTES_FILE, "a");

    if (votes_record == NULL) {
        perror("Error opening votes record file");
        return NULL;
    }

    // Authenticate the client based on Voters_List.txt
    FILE *voters_file = fopen(VOTERS_FILE, "r");
    if (voters_file == NULL) {
        perror("Error opening voters file");
        return NULL;
    }

    char line[200];
    while (fgets(line, sizeof(line), voters_file)) {
        if (strstr(line, client->voter_name) != NULL) {
            client->authenticated = 1;
            break;
        }
    }
    fclose(voters_file);

    if (!client->authenticated) {
        char auth_fail_msg[200] = "Authentication failed. Your name and CNIC do not match.\n";
        send(client->socket, auth_fail_msg, strlen(auth_fail_msg), 0);
    } else {
        char welcome_msg[200];
        snprintf(welcome_msg, sizeof(welcome_msg), "Welcome, %s! You are now authenticated and can cast your vote.\n", client->voter_name);
        send(client->socket, welcome_msg, strlen(welcome_msg), 0);
        handle_vote(client, votes_record);
    }

    fclose(votes_record);
    close(client->socket);
    free(client);
    return NULL;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error creating server socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(2000);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding error");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Listening error");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening...\n");

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (client_socket < 0) {
            perror("Acceptance error");
            exit(EXIT_FAILURE);
        }

        client_info *client = (client_info *)malloc(sizeof(client_info));
        client->socket = client_socket;
        client->address = client_addr;
        client->authenticated = 0;

        printf("New connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        char name_cnic[200];
        recv(client_socket, name_cnic, sizeof(name_cnic), 0);
        strcpy(client->voter_name, name_cnic);

        pthread_t tid;
        pthread_create(&tid, NULL, client_handler, client);
    }

    close(server_socket);
    return 0;
}

