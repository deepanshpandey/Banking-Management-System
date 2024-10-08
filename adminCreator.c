#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "db.h"

void add_admin() {
    Admin admin;
    const int fd = open("db/admins.dat", O_WRONLY | O_CREAT | O_APPEND, 0666);

    if (fd == -1) {
        perror("Failed to open file db/admins.dat");
        return;
    }

    // Input Admin details
    printf("Enter Admin ID: ");
    if (scanf("%d", &admin.id) != 1) {
        printf("Invalid input for Admin ID.\n");
        close(fd);
        return;
    }

    printf("Enter Name: ");
    scanf(" %[^\n]%*c", admin.name);  // Read a line of text with spaces

    printf("Enter Email: ");
    scanf(" %[^\n]%*c", admin.email);  // Read a line of text with spaces

    printf("Enter Password: ");
    scanf(" %[^\n]%*c", admin.password);  // Read a line of text with spaces

    // Write the admin structure to the file
    ssize_t written = write(fd, &admin, sizeof(Admin));
    if (written == sizeof(Admin)) {
        printf("Admin added successfully!\n");
    } else {
        printf("Error writing admin data to file.\n");
    }

    close(fd);
}

int main() {
    add_admin();
    return 0;
}
