//
// Created by deepanshpandey on 10/9/24.
//
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "../db.h"
#include "admintasks.h"

int verify_admin(const char *email, const char *password) {
    const char* file_path = "db/admins.dat";
    Admin admin;
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return -1;
    }

    while (read(fd, &admin, sizeof(admin)) > 0) {
        if (strcmp(admin.email, email) == 0 && strcmp(admin.password, password) == 0) {
            close(fd);
            return 1; // Record found
        }
    }

    close(fd);
    return 0; // Record not found
}