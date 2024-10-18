#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "db/db.h"
void set_otdata() {
    const char* fd="db/otdata.db";
    int fd1 = open(fd, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd1 == -1) {
        perror("Failed to open file db/otdata.db");
        return;
    }
    OTData otdata;
    otdata.highest_admin_id=0;
    otdata.highest_employee_id=0;
    otdata.highest_customer_id=0;
    otdata.highest_feedback_id=0;
    otdata.highest_loan_id=0;
    otdata.highest_transaction_id=0;
    if (write(fd1, &otdata, sizeof(OTData))<0) {
        printf("Error writing otdata to file.\n");
    } else {
        printf("OTData set to 0 successfully!\n");
    }
}
void clear_db() {
    const char *db_files[] = {"db/admins.db", "db/employees.db", "db/customers.db"};
    size_t num_files = sizeof(db_files) / sizeof(db_files[0]);

    for (size_t i = 0; i < num_files; ++i) {
        int fd = open(db_files[i], O_WRONLY | O_TRUNC);
        if (fd == -1) {
            printf("Failed to clear database: %s\n", db_files[i]);
        } else {
            printf("Cleared %s\n", db_files[i]);
            close(fd);
        }
    }
    set_otdata();
}
void add_admin() {
    Admin admin;
    printf("Do you wish to Clear all databases? (y/n): ");
    char choice;
    scanf("%c", &choice);
    if (choice == 'y') {
        clear_db();
    }

    const int fd = open("db/admins.db", O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd == -1) {
        perror("Failed to open file db/admins.db");
        return;
    }
    printf("\nEnter Admin Details\n");
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
        printf("Name: %s\n", admin.name);
        printf("Email: %s\t", admin.email);
        printf("Password: %s\n", admin.password);
        printf("Admin added successfully!\n");
    }
    close(fd);
}

int main() {
    add_admin();
    return 0;
}
