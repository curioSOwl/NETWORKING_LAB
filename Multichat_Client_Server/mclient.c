#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX_MESSAGE_SIZE 100

void *receive_messages(void *arg) {
    int socket_desc = *(int *)arg;
    char buffer[MAX_MESSAGE_SIZE];

    while (1) {
        int bytes_received = recv(socket_desc, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            break; // Connection closed or error occurred, terminate the thread
        }

        buffer[bytes_received] = '\0';
        printf("Received: %s", buffer);
    }

    pthread_exit(NULL);
}

int main() {
    int socket_desc;
    int k;
    char buffer[MAX_MESSAGE_SIZE];
    struct sockaddr_in server;
    pthread_t tid;

    // First create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    // Initialize the server address values
    server.sin_family = AF_INET;
    server.sin_port = htons(3003); // Use htons() to convert port to network byte order
    server.sin_addr.s_addr = INADDR_ANY; // Use the server IP address

    // Connect
    k = connect(socket_desc, (struct sockaddr *)&server, sizeof(server));
    if (k == -1) {
        printf("Error connecting to server...\n");
        return 1;
    }
    else {
        printf("Connection established with server...\n");
    }

    // Create a new thread to receive messages from the server
    if (pthread_create(&tid, NULL, receive_messages, (void *)&socket_desc) != 0) {
        printf("Failed to create thread to receive messages...\n");
        close(socket_desc);
        return 1;
    }

    // Send and receive messages
    while (1) {
        printf("Enter the message to send: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(socket_desc, buffer, strlen(buffer), 0);
    }

    close(socket_desc);
    return 0;
}
