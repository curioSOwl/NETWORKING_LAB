#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    FILE *f1;
    int sock_desc;
    char buffer[100];
    struct sockaddr_in server, client;

    f1 = fopen("out.txt", "w");
    if (f1 == NULL) {
        perror("Error opening file");
        return 1;
    }

    sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc == -1) {
        perror("Error creating socket");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(3004);
    server.sin_addr.s_addr = INADDR_ANY;

    if (connect(sock_desc, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("Error connecting");
        return 1;
    }

    printf("Connection established.\n");
    printf("Enter the file name: ");
    fgets(buffer, 100, stdin);
    buffer[strlen(buffer)-1]='\0';

    int k = send(sock_desc, buffer, 100, 0);
    if (k == -1) {
        perror("Error sending data");
        return 1;
    }

    int byt;
    while ((byt = recv(sock_desc, buffer, 100, 0)) > 0) {
        fwrite(buffer, 1, byt, f1);
    }

    fclose(f1);
    close(sock_desc);
    return 0;
}
