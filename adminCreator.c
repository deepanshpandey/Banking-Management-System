#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "db/db.h"
void create_db() {
    const char *db_files[] = {"db/admins.db", "db/otdata.db", "db/employees.db", "db/customers.db", "db/feedbacks.db", "db/loans.db", "db/transactions.db"};
    size_t num_files = sizeof(db_files) / sizeof(db_files[0]);

    for (size_t i = 0; i < num_files; ++i) {
        int fd = open(db_files[i], O_CREAT | O_RDWR | O_APPEND, 0666);
        if (fd == -1) {
            printf("Failed to create database: %s\n", db_files[i]);
        } else {
            printf("Created %s\n", db_files[i]);
            close(fd);
        }
    }
}
void set_otdata() {
    const char* file_path="db/otdata.db";
    int fd1 = open(file_path, O_CREAT | O_RDWR | O_APPEND, 0777);
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
    close(fd1);
}
void clear_db() {
    const char *db_files[] = {"db/admins.db", "db/employees.db", "db/customers.db", "db/feedbacks.db", "db/loans.db", "db/transactions.db"};
    size_t num_files = sizeof(db_files) / sizeof(db_files[0]);

    for (size_t i = 0; i < num_files; ++i) {
        int fd = open(db_files[i], O_CREAT | O_RDWR | O_TRUNC, 0666);
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
    const int fd = open("db/admins.db", O_CREAT | O_RDWR | O_APPEND, 0666);
    if (fd == -1) {
        perror("Failed to open file db/admins.db");
        return;
    }
    printf("\nEnter Admin Details\n");

    getchar();

    printf("Enter Name:");
    fgets(admin.name, sizeof(admin.name), stdin);
    admin.name[strcspn(admin.name, "\n")] = '\0';

    printf("Enter Email:");
    fgets(admin.email, sizeof(admin.email), stdin);
    admin.email[strcspn(admin.email, "\n")] = '\0';

    printf("Enter Password:");
    fgets(admin.password, sizeof(admin.password), stdin);
    admin.password[strcspn(admin.password, "\n")] = '\0';

    admin.login_status = 0;
    // Write the admin structure to the file

    if (write(fd, &admin, sizeof(Admin))<0) {
        printf("Error writing admin data to file.\n");
    } else {
        printf("\nName: %s\n", admin.name);
        printf("Email: %s\t", admin.email);
        printf("Password: %s\n", admin.password);
        printf("Admin added successfully!\n");
    }
    close(fd);
}
void clear_logins(int choice) {
   if (choice==1) {
       const char *db_files[] = {"db/admins.db", "db/employees.db", "db/customers.db"};
       size_t num_files = sizeof(db_files) / sizeof(db_files[0]);

       for (size_t i = 0; i < num_files; ++i) {
           int fd = open(db_files[i], O_CREAT | O_RDWR , 0666);

           if (fd == -1) {
             printf("Failed to open file: %s\n", db_files[i]);
           }
           else {
               if(i==0) {
                   Admin admin;
                   while (read(fd, &admin, sizeof(Admin)) > 0) {
                       admin.login_status = 0;
                       lseek(fd, -sizeof(Admin), SEEK_CUR);
                       write(fd, &admin, sizeof(Admin));
                   }
                   printf("Cleared logins for %s\n", db_files[i]);
                   close(fd);
               }
               if (i==1) {
                   Employee emp;
                       while (read(fd, &emp, sizeof(Employee)) > 0) {
                          emp.login_status = 0;
                          lseek(fd, -sizeof(Employee), SEEK_CUR);
                          write(fd, &emp, sizeof(Employee));
                       }
                   printf("Cleared logins for %s\n", db_files[i]);
                     close(fd);
               }
               if (i==2) {
                   Customer cust;
                       while (read(fd, &cust, sizeof(Customer)) > 0) {
                          cust.login_status = 0;
                          lseek(fd, -sizeof(Customer), SEEK_CUR);
                          write(fd, &cust, sizeof(Customer));
                       }
                   printf("Cleared logins for %s\n", db_files[i]);
                     close(fd);
               }
          }
       }
   }
    else {
        printf("\nWhich database to clear logins\n1. Admins\n2. Employees\n3. Customers\n");
        int i;
        scanf("%d", &i);
        i--;
        const char *db_files[] = {"db/admins.db", "db/employees.db", "db/customers.db"};
        int fd = open(db_files[i], O_CREAT | O_RDWR , 0666);
        if(i==0) {
            Admin admin;
            while (read(fd, &admin, sizeof(Admin)) > 0) {
                admin.login_status = 0;
                lseek(fd, -sizeof(Admin), SEEK_CUR);
                write(fd, &admin, sizeof(Admin));
            }
            printf("Cleared logins for %s\n", db_files[i]);
            close(fd);
        }
        if (i==1) {
            Employee emp;
            while (read(fd, &emp, sizeof(Employee)) > 0) {
                emp.login_status = 0;
                lseek(fd, -sizeof(Employee), SEEK_CUR);
                write(fd, &emp, sizeof(Employee));
            }
            printf("Cleared logins for %s\n", db_files[i]);
            close(fd);
        }
        if (i==2) {
            Customer cust;
            while (read(fd, &cust, sizeof(Customer)) > 0) {
                cust.login_status = 0;
                lseek(fd, -sizeof(Customer), SEEK_CUR);
                write(fd, &cust, sizeof(Customer));
            }
            printf("Cleared logins for %s\n", db_files[i]);
            close(fd);
        }
        close(fd);
    }
}


int main() {
    char c,c1,c2;

    printf("\nDo you wish to create all databases? (y/n): ");
    scanf(" %c", &c);
    if (c == 'y') {
        create_db();
    }
    printf("\nDo you wish to Clear all databases? (y/n): ");
    scanf(" %c", &c1);
    if (c1 == 'y') {
        clear_db();
    }
    printf("\nDo you wish to Create a New Admin? (y/n): ");
    scanf(" %c", &c2);
    if (c2 == 'y') {
        add_admin();;
    }
    printf("\nDo you wish to clear all Logins or select specific databse? \n'y' for all / 'n' to select specific: ");
    scanf(" %c", &c2);
    if (c2 == 'y') {
        clear_logins(1);
    }
    else {
        clear_logins(0);
    }
    return 0;
}
