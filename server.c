#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>

#include "admin/adminoptions.h"
#include "employee/empoptions.h"
#include "customer/customeroptions.h"

#define PORT 8091
#define BUFFER_SIZE 10240

void *handle_client(void *socket_desc) {
    int new_socket = *(int *)socket_desc;
    free(socket_desc);
    char buffer[BUFFER_SIZE];
    bool logout = false;
    while ( logout == false) {
        menu_options:
        const char *menu = "Select an option:\n"
                        "1. Customer Login\n"
                        "2. Employee Login\n"
                        "3. Manager Login\n"
                        "4. Admin Login\n"
                        "5. Exit\n"
                        "Enter your choice: ";
        write(new_socket, menu, strlen(menu));
        bzero(buffer, BUFFER_SIZE);
        int read_size = read(new_socket, buffer, BUFFER_SIZE);
        const int option = atoi(buffer);
        if (read_size == 0) {
            perror("receive failed");
            printf("Try again\n");
            bzero(buffer, BUFFER_SIZE);
            goto menu_options;
        }
        if (read_size == -1) {
            perror("receive failed");
            bzero(buffer, BUFFER_SIZE);
            goto menu_options;
        }
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
                break;

            default:
                write(new_socket, "Invalid option. Please select again\n", 37);
                bzero(buffer, BUFFER_SIZE);
                goto menu_options;
        }
    }

    printf("Client disconnected\n");
    close(new_socket);
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);
    signal(SIGPIPE, SIG_IGN);

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
        close(server_fd);
        return 1;
    }

    listen(server_fd, 3);
    printf("Waiting for incoming connections...\n");

    while (1) {

        new_socket = accept(server_fd, (struct sockaddr *)&client, &client_len);

        if (new_socket < 0) {
            perror("Accept failed");
            close(server_fd); // Close the server socket on accept error
            return 1; // Optionally break the loop or return
        }

        printf("Connection accepted\n");

        pthread_t client_thread;
        int *new_sock = malloc(sizeof(int));

        if (new_sock == NULL) {
            perror("Could not allocate memory");
            close(new_socket);
            return 1;
        }

        *new_sock = new_socket;

        if (pthread_create(&client_thread, NULL, handle_client,new_sock) < 0) {
            perror("Could not create thread");
            close(new_socket); // **Close the socket if thread creation fails**
            free(new_sock); // **Free allocated memory**
            continue;
        }

        printf("Handler assigned\n");
    }
}