//
// Created by deepanshpandey on 10/9/24.
//

#ifndef EMPTASKS_H
#define EMPTASKS_H

#include "../db/db.h"
#include "../db/ottasks.h"
#include "../customer/customertasks.h"
#include <string.h>
#include <fcntl.h>

int verify_employee(const char *email, const char *password) {
    const char* file_path = "../db/employees.db";
    Employee emp;
    int fd = open(file_path, O_RDONLY,0777);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }

    while (read(fd, &emp, sizeof(emp)) > 0) {
        if(strcmp(email, emp.email) == 0 && strcmp(password, emp.password) == 0 && emp.login_status==0) {
            emp.login_status=1;
            lseek(fd, -sizeof(emp), SEEK_CUR);
            write(fd, &emp, sizeof(emp));
            close(fd);
            return 1; // Record found
        }
    }
    return 0; // Record not found
}
int verify_manager(const char *email, const char *password) {
    const char* file_path = "../db/employees.db";
    Employee emp;
    int fd = open(file_path, O_RDONLY,0777);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }

    while (read(fd, &emp, sizeof(emp)) > 0) {
        if(strcmp(email, emp.email) == 0 && strcmp(password, emp.password) == 0 && emp.is_manager==1 && emp.login_status==0) {
            emp.login_status=1;
            lseek(fd, -sizeof(emp), SEEK_CUR);
            write(fd, &emp, sizeof(emp));
            close(fd);
            return 1; // Record found
        }
    }
    return 0; // Record not found
}
int logoutemployee(const char *email) {
    const char* file_path = "../db/employees.db";
    int fd = open(file_path, O_RDWR,0777);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    Employee emp;
    while (read(fd, &emp, sizeof(emp)) > 0) {
        if(strcmp(email, emp.email) == 0) {
            emp.login_status=0;
            lseek(fd, -sizeof(emp), SEEK_CUR);
            write(fd, &emp, sizeof(emp));
            close(fd);
            return 1; // Record found
        }
    }
    return 0; // Record not found
}
int customer_lookup(const char *email) {
    //check if customer exists
    const char* file_path = "../db/customers.db";
    int fd=open(file_path, O_RDONLY,0777);
    if(fd==-1) {
        perror("Failed to open file");
        return 0;
    }
    Customer cust;
    while(read(fd, &cust, sizeof(cust))>0) {
        if(strcmp(email, cust.email)==0) {
            //id already exists
            close(fd);
            return 1;
        }
    }
    close(fd);
    return 0;
}
int getemployeeid(const char *email) {
    //get employee id
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
            return emp.id;
        }
    }
    close(fd);
    return 0;
}
int change_customer_status(const char* email,int status) {
    //change customer status
    const char* file_path = "../db/customers.db";
    int fd=open(file_path, O_RDWR,0777);
    if(fd==-1) {
        perror("Failed to open file");
        return 0;
    }
    int id = getcustomerid(email);
    Customer cust;
    while(read(fd, &cust, sizeof(cust))>0) {
        if(strcmp(id, cust.id)==0) {
            //id already exists
            lseek(fd, -sizeof(cust), SEEK_CUR);
            cust.account_active=status;
            write(fd, &cust, sizeof(cust));
            close(fd);
            return 1;
        }
    }
    close(fd);
    return 0;
}
int change_emp_password(const char* email, const char* password) {
    //change employee password
    const char* file_path = "../db/employees.db";
    int fd=open(file_path, O_RDWR,0777);
    if(fd==-1) {
        perror("Failed to open file");
        return 0;
    }
    Employee emp;
    while(read(fd, &emp, sizeof(emp))>0) {
        if(strcmp(email, emp.email)==0) {
            //id already exists
            lseek(fd, -sizeof(emp), SEEK_CUR);
            strcpy(emp.password, password);
            write(fd, &emp, sizeof(emp));
            close(fd);
            return 1;
        }
    }
    close(fd);
    return 0;
}
int add_customer(const char *name, const char *email, const char *phone, const char *password, double balance, int account_active) {
    const char* file_path = "../db/customers.db";
    int fd = open(file_path, O_RDWR | O_CREAT | O_APPEND, 0666);  // Open file in append mode
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    //add new customer
    int login_status=0;
    int id= generatecustomerid();
    Customer customer={id, name, email, phone, password, balance, account_active,login_status};
    write(fd, &customer, sizeof(customer));

    close(fd);
    return id;
}
#endif //EMPTASKS_H
