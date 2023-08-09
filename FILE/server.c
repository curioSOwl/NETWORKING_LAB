#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    FILE *f2;
    int sock_desc, temp_sock_desc;
    char buffer[100];
    struct sockaddr_in server, client;
    socklen_t len;

    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc == -1) {
        perror("Error creating socket");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(3004);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock_desc, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Error binding socket");
        return 1;
    }

    if (listen(sock_desc, 5) == -1) {
        perror("Error listening");
        return 1;
    }

    len = sizeof(client);
    temp_sock_desc = accept(sock_desc, (struct sockaddr *)&client, &len);
    if (temp_sock_desc == -1) {
        perror("Error accepting connection");
        return 1;
    }

    int k = recv(temp_sock_desc, buffer, 100, 0);
    printf("%s",buffer);
    if (k == -1) {
        perror("Error receiving data");
        return 1;
    }

    f2 = fopen(buffer, "r");
    if (f2 == NULL) {
        perror("Error opening file");
        return 1;
    }

    int byt;
    while ((byt = fread(buffer, 1, 100, f2)) > 0) {
        send(temp_sock_desc, buffer, byt, 0);
    }

    fclose(f2);
    close(sock_desc);
    close(temp_sock_desc);
    return 0;
}
