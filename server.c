#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "admin/adminoptions.h"
#include "admin/admintasks.h"
#include "employee/empoptions.h"
#include "customer/customeroptions.h"

#define PORT 8091
#define BUFFER_SIZE 10240

void *handle_client(void *socket_desc) {
    int new_socket = *(int *)socket_desc;
    char buffer[BUFFER_SIZE];
    bool logout = false;

    const char *menu = "Select an option:\n"
                        "1. Customer Login\n"
                        "2. Employee Login\n"
                        "3. Manager Login\n"
                        "4. Admin Login\n"
                        "5. Exit\n";
    write(new_socket, menu, strlen(menu));
    bzero(buffer, BUFFER_SIZE);
    int read_size = read(new_socket, buffer, BUFFER_SIZE);
    if (read_size == 0) {
        perror("receive failed");
        printf("Try again\n");
        bzero(buffer, BUFFER_SIZE);
        write(new_socket, menu, strlen(menu));
    } else if (read_size == -1) {
        perror("receive failed");
        bzero(buffer, BUFFER_SIZE);
        write(new_socket, menu, strlen(menu));
    }
    const int option = atoi(buffer);

    while ( logout == false) {

        switch (option) {

            case 1:
                customer_menu(new_socket);
                break;

            case 2:
                emp_menu(new_socket);
                break;

            case 3:
                manager_menu(new_socket);
                break;

            case 4:
                admin_menu(new_socket);
                break;


            case 5:
                write(new_socket, "Exit\n", 5);
                logout = true;
                close(new_socket);
                free(socket_desc);
                return NULL;
                break;


            default:
                write(new_socket, "Invalid option. Please select again\n", 37);
                bzero(new_socket, BUFFER_SIZE);
                write(new_socket, menu, strlen(menu));
                bzero(new_socket, BUFFER_SIZE);
                break;

        }
        write(new_socket, menu, strlen(menu));
        bzero(buffer, BUFFER_SIZE);
    }

    printf("Client disconnected\n");
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