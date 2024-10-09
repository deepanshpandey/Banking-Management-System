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
int lookup(int id) {
    const char* file_path = "../db/employees.txt";
    int fd=open(file_path, O_RDONLY);
    if(fd==-1) {
        perror("Failed to open file");
        return 0;
    }
    Employee emp;
    while(read(fd, &emp, sizeof(emp))>0) {
        if(strcmp(id, emp.id)==0) {
            //id already exists
            close(fd);
            return 1;
        }
    }
    close(fd);
    return 0;
}
int add_employee(int id, const char *name, const char *email, const char* password, int is_manager) {
    const char* file_path = "../db/employees.txt";
    int fd = open(file_path, O_WRONLY | O_APPEND);  // Open file in append mode
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    if(lookup(id)==1) {
        //already exists
        return 0;
    }else {
        //add new employee
        Employee employee={id, name, email, password, is_manager};
        write(fd, &employee, sizeof(employee));
    }
    close(fd);
    return 1;
}
int modify_employee(int id, const char *name, const char *email, const char* password, int is_manager) {
    const char *file_path = "../db/employees.txt";
    int fd = open(file_path, O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    Employee emp={id, name, email, password, is_manager};
    while(read(fd, &emp, sizeof(emp))>0) {
        if(strcmp(id, emp.id)==0) {
            //id already exists
            lseek(fd, -1*sizeof(emp), SEEK_CUR);
            write(fd, &emp, sizeof(emp));
            close(fd);
            return 1;
        }
    }
        return 0;
}
int modify_customer(int id, const char *name, const char *email, const char *phone, const char *password, double balance, int account_active) {
    const char *file_path = "../db/customers.txt";
    int fd = open(file_path, O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    Customer cust={id, name, email, phone, password, balance, account_active};
    while(read(fd, &cust, sizeof(cust))>0) {
        if(strcmp(id, cust.id)==0) {
            //id already exists
            lseek(fd, -1*sizeof(cust), SEEK_CUR);
            write(fd, &cust, sizeof(cust));
            close(fd);
            return 1;
        }
    }
        return 0;
}
int change_admin_password(const char* email, const char* password) {
    //change admin password
    const char* file_path = "../db/admins.txt";
    int fd=open(file_path, O_RDWR);
    if(fd==-1) {
        perror("Failed to open file");
        return 0;
    }
    Admin admin;
    while(read(fd, &admin, sizeof(admin))>0) {
        if(strcmp(email, admin.email)==0) {
            //id already exists
            lseek(fd, -sizeof(admin), SEEK_CUR);
            strcpy(admin.password, password);
            write(fd, &admin, sizeof(admin));
            close(fd);
            return 1;
        }
    }
    close(fd);
    return 0;
}
#endif // ADMINTASKS_H