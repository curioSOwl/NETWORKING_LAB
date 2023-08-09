#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX_CLIENTS 5
#define MAX_MESSAGE_SIZE 100

int clients[MAX_CLIENTS];
int current_client_count = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    char buffer[MAX_MESSAGE_SIZE];

    while (1) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            break; // Connection closed or error occurred, terminate the thread
        }

        buffer[bytes_received] = '\0';
        printf("Received from client %d: %s", client_socket, buffer);

        // Send the message to all other clients (broadcast)
        for (int i = 0; i < current_client_count; i++) {
            if (clients[i] != client_socket) {
                send(clients[i], buffer, strlen(buffer), 0);
            }
        }
    }

    // Remove the client from the list and close the socket
    for (int i = 0; i < current_client_count; i++) {
        if (clients[i] == client_socket) {
            while (i < current_client_count - 1) {
                clients[i] = clients[i + 1];
                i++;
            }
            current_client_count--;
            break;
        }
    }

    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    int socket_desc;
    int k;
    char buffer[MAX_MESSAGE_SIZE];
    struct sockaddr_in server, client;
    socklen_t len;
    pthread_t tid;

    // First create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    // Initialize the server address values
    server.sin_family = AF_INET;
    server.sin_port = htons(3003); // Use htons() to convert port to network byte order
    server.sin_addr.s_addr = INADDR_ANY;

    // Bind the address with the socket
    bind(socket_desc, (struct sockaddr *)&server, sizeof(server));

    // Listen
    k = listen(socket_desc, 5);
    if (k == -1) {
        printf("Error...\n");
        return 1;
    }
    else {
        printf("Listening...\n");
    }

    while (1) {
        // Accept a new connection
        len = sizeof(client);
        int new_socket = accept(socket_desc, (struct sockaddr *)&client, &len);
        if (new_socket < 0) {
            printf("Connection failed...\n");
            continue;
        }
        else {
            printf("Connection established with client %d...\n", new_socket);
            clients[current_client_count] = new_socket;
            current_client_count++;

            // Create a new thread to handle this client
            if (pthread_create(&tid, NULL, handle_client, (void *)&new_socket) != 0) {
                printf("Failed to create thread for client %d...\n", new_socket);
                close(new_socket);
            }
        }
    }

    close(socket_desc);
    return 0;
}
