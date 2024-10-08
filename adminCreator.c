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



    printf("Enter Name: ");
    scanf(" %s", admin.name);  // Read a line of text with spaces

    printf("Enter Email: ");
    scanf(" %s", admin.email);  // Read a line of text with spaces

    printf("Enter Password: ");
    scanf(" %s", admin.password);  // Read a line of text with spaces

    // Write the admin structure to the file

    if (write(fd, &admin, sizeof(Admin))<0) {
        printf("Error writing admin data to file.\n");
    } else {
        printf("Admin added successfully!\n");
    }

    close(fd);
}

int main() {
    add_admin();
    return 0;
}
