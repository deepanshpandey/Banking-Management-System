#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "db.h"
#include "admin/admintasks.h"

#define PORT 8091
#define BUFFER_SIZE 10240

void *handle_client(void *socket_desc) {
    int new_socket = *(int *)socket_desc;
    char buffer[BUFFER_SIZE];
    int read_size;

    const char *menu = "Select an option:\n"
                        "1. Customer Login\n"
                        "2. Employee Login\n"
                        "3. Manager Login\n"
                        "4. Admin Login\n";
    send(new_socket, menu, strlen(menu), 0);

    while ((read_size = read(new_socket, buffer, BUFFER_SIZE)) > 0) {
        int option = atoi(buffer);

        switch (option) {
            case 1:
                write(new_socket, "Customer Login\n", 23);
                break;
            case 2:
                write(new_socket, "Employee Login\n", 23);
                break;
            case 3:
                write(new_socket, "Manager Login\n", 23);
                break;
            case 4: {
                char email[50], password[50];
                char* msg="Enter Email: ";
                write(new_socket, msg, sizeof(msg));
                read(new_socket, email, sizeof(email)-1);
                email[strcspn(email, "\n")] = '\0';
                msg="Enter Password: ";
                write(new_socket, msg, sizeof(msg));
                read(new_socket, password, sizeof(password)-1);
                password[strcspn(password, "\n")] = '\0';
                if (verify_admin(email, password) == 1) {
                    write(new_socket, "Admin Login Successful\n", 23);
                } else {
                    write(new_socket, "Invalid Admin Credentials\n", 26);
                    write(new_socket, menu, strlen(menu));
                }
                break;
            }
            default:
                write(new_socket, "Invalid option. Please select again\n", 37);
                bzero(new_socket, BUFFER_SIZE);
                write(new_socket, menu, strlen(menu));
                break;
        }
    }

    if (read_size == 0) {
        printf("Client disconnected\n");
    } else if (read_size == -1) {
        perror("recv failed");
    }

    close(new_socket);
    free(socket_desc);
    return NULL;
}

int main() {
    int server_fd, new_socket, *new_sock;
    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Could not create socket");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        return 1;
    }

    listen(server_fd, 3);

    printf("Waiting for incoming connections...\n");
    while ((new_socket = accept(server_fd, (struct sockaddr *)&client, &client_len))) {
        printf("Connection accepted\n");

        pthread_t client_thread;
        new_sock = malloc(1);
        *new_sock = new_socket;

        if (pthread_create(&client_thread, NULL, handle_client, (void *)new_sock) < 0) {
            perror("Could not create thread");
            return 1;
        }

        printf("Handler assigned\n");
    }

    if (new_socket < 0) {
        perror("Accept failed");
        return 1;
    }

    close(server_fd);
    return 0;
}