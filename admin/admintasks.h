#ifndef ADMINTASKS_H
#define ADMINTASKS_H
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "../db.h"

int verify_admin(const char *email, const char *password) {
    const char* file_path = "../db/admins.txt";
    Admin admin;
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }

    while (read(fd, &admin, sizeof(admin)) > 0) {
        if(strcmp(email, admin.email) == 0 && strcmp(password, admin.password) == 0) {
            close(fd);
            return 1; // Record found
        }
    }
    return 0; // Record not found
}

#endif // ADMINTASKS_H