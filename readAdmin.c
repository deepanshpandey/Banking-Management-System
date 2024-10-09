#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "db.h"

void read_admins() {
    const char* file_path = "db/admins.txt";
    Admin admin;
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }

    while (read(fd, &admin, sizeof(admin)) > 0) {
        printf("Name: %s\n", admin.name);
        printf("Email: %s\n", admin.email);
        printf("Password: %s\n", admin.password);
        printf("-------------------------\n");
    }

    close(fd);
}

int main() {
    read_admins();
    return 0;
}