#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8091
#define BUFFER_SIZE 10240

void hide_input() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag &= ~ECHO; // Disable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}
void show_input() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag |= ECHO;  // Enable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}
int check_for_password(char *server_reply) {
    const char *password_prompts[] = {
        "Enter Password",
        "Enter Employee Password",
        "Enter Customer Password",
        "Enter New Password"
    };

    for (int i = 0; i < sizeof(password_prompts) / sizeof(password_prompts[0]); i++) {
        if (strstr(server_reply, password_prompts[i])) {
            return 1;
        }
    }
    return 0;
}

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
        if(check_for_password(server_reply)){
            hide_input();
            fflush(stdin);
            bzero(message, BUFFER_SIZE);
            fgets(message, BUFFER_SIZE, stdin);
            message[strcspn(message, "\n")] = '\0';
            show_input();
        }
        else {
            bzero(message, BUFFER_SIZE);
            fgets(message, BUFFER_SIZE, stdin);
            message[strcspn(message, "\n")] = '\0';
        }

        if(strcmp(message,"e")==0) {
            close(sock);
            return 0;
        }
        if (write(sock, message, strlen(message)) < 0) {
            return 1;
        }

        bzero(server_reply, BUFFER_SIZE);
        if (read(sock, server_reply, BUFFER_SIZE) < 0) {
            perror("Recv failed");
            break;
        }
        printf("%s", server_reply);
    }
    close(sock);
    return 0;
}