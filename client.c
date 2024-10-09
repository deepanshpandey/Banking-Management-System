#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8091
#define BUFFER_SIZE 102400

int main() {
    int sock;
    struct sockaddr_in server;
    char message[BUFFER_SIZE], server_reply[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Could not create socket");
        return 1;
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connect failed");
        return 1;
    }

    printf("Connected to server\n");

    // Receive the menu from the server
    bzero(server_reply, BUFFER_SIZE);
    if (read(sock, server_reply, BUFFER_SIZE) < 0) {
        perror("Recv failed");
        return 1;
    }
    printf("%s", server_reply);

    while (1) {
        // printf("Client input\n");
        fgets(message, BUFFER_SIZE, stdin);
        //message[strcspn(message, "\n")] = '\0';

        if (write(sock, message, strlen(message)) < 0) {
            return 1;
        }

        bzero(server_reply, BUFFER_SIZE);
        if (read(sock, server_reply, BUFFER_SIZE) < 0) {
            perror("Recv failed");
            break;
        }

        printf("%s\n", server_reply);
    }

    close(sock);
    return 0;
}