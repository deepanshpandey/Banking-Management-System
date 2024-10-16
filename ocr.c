#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For usleep(), fork(), execlp()
#include <sys/types.h>  // For pid_t
#include <signal.h>  // For kill()
#include <sys/wait.h>  // For wait()

int main() {
    // Define source files and their output binaries
    const char *server_source = "server.c";
    const char *server_output = "./server";
    const char *client_source = "client.c";
    const char *client_output = "./client";

    // Compile the server program
    char compile_server_command[100];
    sprintf(compile_server_command, "gcc %s -o %s", server_source, server_output);
    system(compile_server_command);

    // Fork to run the server program
    pid_t server_pid = fork();
    if (server_pid == 0) {
        // Child process: execute the server
        execlp(server_output, server_output, NULL);
        // If execlp fails
        perror("execlp failed");
        exit(1);
    }

    // Wait for a moment to allow the server to start
    usleep(1200 * 1000);  // usleep takes microseconds, so multiply by 1000

    // Compile the client program
    char compile_client_command[100];
    sprintf(compile_client_command, "gcc %s -o %s", client_source, client_output);
    system(compile_client_command);

    // Run the client program
    system(client_output);

    // Wait for the client to finish
    wait(NULL);  // Wait for the child process (client) to finish

    // Once client is closed, terminate the server
    kill(server_pid, SIGTERM);  // Send the terminate signal to the server process

    // Optionally, wait for the server process to terminate
    waitpid(server_pid, NULL, 0);

    return 0;
}
