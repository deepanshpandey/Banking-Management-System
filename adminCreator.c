#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "db.h"

void add_admin() {
    Admin admin;
    const int fd = open("db/admins.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);

    if (fd == -1) {
        perror("Failed to open file db/admins.txt");
        return;
    }



    printf("Enter Name: ");
    fgets(admin.name, sizeof(admin.name), stdin);
    admin.name[strcspn(admin.name, "\n")] = '\0';

    printf("Enter Email: ");
    fgets(admin.email, sizeof(admin.email), stdin);
    admin.email[strcspn(admin.email, "\n")] = '\0';

    printf("Enter Password: ");
    fgets(admin.password, sizeof(admin.password), stdin);
    admin.password[strcspn(admin.password, "\n")] = '\0';

    // Write the admin structure to the file

    if (write(fd, &admin, sizeof(Admin))<0) {
        printf("Error writing admin data to file.\n");
    } else {
        printf("%s\n", admin.name);
        printf("%s\n", admin.email);
        printf("%s\n", admin.password);
        printf("Admin added successfully!\n");
    }

    close(fd);
}

int main() {
    add_admin();
    return 0;
}
