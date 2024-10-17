#ifndef ADMINTASKS_H
#define ADMINTASKS_H
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "../customer/customertasks.h"
#include "../employee/emptasks.h"
#include "../db/ottasks.h"
#include "../db/db.h"

int verify_admin(const char *email, const char *password) {
    const char* file_path = "../db/admins.db";
    Admin admin;
    int fd = open(file_path, O_RDONLY,0777);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }

    while (read(fd, &admin, sizeof(admin)) > 0) {
        if(strcmp(email, admin.email) == 0 && strcmp(password, admin.password) == 0 && admin.login_status==0) {
            admin.login_status=1;
            lseek(fd, -sizeof(admin), SEEK_CUR);
            write(fd, &admin, sizeof(admin));
            close(fd);
            return 1; // Record found and no other sessions
        }
    }
    return 0; // Record not found
}
int logoutadmin(const char *email) {
    const char* file_path = "../db/admins.db";
    int fd = open(file_path, O_RDWR,0777);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    Admin admin;
    while (read(fd, &admin, sizeof(admin)) > 0) {
        if(strcmp(email, admin.email) == 0) {
            admin.login_status=0;
            lseek(fd, -sizeof(admin), SEEK_CUR);
            write(fd, &admin, sizeof(admin));
            close(fd);
            return 1; // Record found and set active login to zero
        }
    }
    return 0; // Record not found
}
int lookup(char *email) {
    const char* file_path = "../db/employees.db";
    int fd=open(file_path, O_RDONLY,0777);
    if(fd==-1) {
        perror("Failed to open file");
        return 0;
    }
    Employee emp;
    while(read(fd, &emp, sizeof(emp))>0) {
        if(strcmp(email, emp.email)==0) {
            //id already exists
            close(fd);
            return 1;
        }
    }
    close(fd);
    return 0;
}
int add_employee(const char *name, const char *email, const char* password, int is_manager) {
    const char* file_path = "../db/employees.db";
    int fd = open(file_path, O_RDWR | O_CREAT | O_APPEND, 0666);  // Open file in append mode
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    if(lookup(email)==1) {
        //already exists
        return 0;
    }else {
        //add new employee
        int id=generateemployeeid();
        Employee employee={id, name, email, password, is_manager};
        write(fd, &employee, sizeof(employee));
    }
    close(fd);
    return 1;
}
int modify_employee(const char *name, const char *email, const char* password, int is_manager) {
    const char *file_path = "../db/employees.db";
    int fd = open(file_path, O_CREAT| O_RDWR | O_APPEND, 0777);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    int id = getemployeeid(email);
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
int modify_customer(const char *name, const char *email, const char *phone, const char *password, double balance, int account_active) {
    const char *file_path = "../db/customers.db";
    int fd = open(file_path, O_RDWR | O_CREAT | O_APPEND, 0666);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    int id = getcustomerid(email);
    Customer cust={id, name, email, phone, password, balance, account_active,0};
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
    const char* file_path = "../db/admins.db";
    int fd=open(file_path, O_RDWR,0777);
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