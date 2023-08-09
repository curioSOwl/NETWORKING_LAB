#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int socket_desc;
    int k;
    char buffer[100];
    struct sockaddr_in server, client;
    socklen_t len;
    
    // First create socket
    socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
    
    // Initialize the server address values
    server.sin_family = AF_INET;
    server.sin_port = htons(3003); // Use htons() to convert port to network byte order
    server.sin_addr.s_addr = INADDR_ANY;
    
    // Bind the address with the socket
    bind(socket_desc, (struct sockaddr *)&server, sizeof(server));
    
    // Receive
    len = sizeof(client);
    recvfrom(socket_desc, buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &len);
    printf("Data received: %s\n", buffer);
    
    // Send
    printf("Enter the message to send: ");
    fgets(buffer, sizeof(buffer), stdin);
    sendto(socket_desc, buffer, strlen(buffer), 0, (struct sockaddr *)&client, len);
    printf("Success! Message sent...\n");
    
    close(socket_desc);
    return 0;
}
