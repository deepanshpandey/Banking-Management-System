//
// Created by deepanshpandey on 10/9/24.
//

#ifndef CUSTOMERTASKS_H
#define CUSTOMERTASKS_H

#include "../db.h"
#include <fcntl.h>

int verify_customer(const char* email, const char* password) {
    const char* file_path = "../db/customers.txt";
    Customer cust;
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }

    while (read(fd, &cust, sizeof(cust)) > 0) {
        if(strcmp(email, cust.email) == 0 && strcmp(password, cust.password) == 0) {
            close(fd);
            return 1; // Record found
        }
    }
    return 0; // Record not found
}
int change_customer_password(const char* email, const char* password) {
    //change customer password
    const char* file_path = "../db/customers.txt";
    int fd=open(file_path, O_RDWR);
    if(fd==-1) {
        perror("Failed to open file");
        return 0;
    }
    Customer cust;
    while(read(fd, &cust, sizeof(cust))>0) {
        if(strcmp(email, cust.email)==0) {
            //id already exists
            lseek(fd, -sizeof(cust), SEEK_CUR);
            strcpy(cust.password, password);
            write(fd, &cust, sizeof(cust));
            close(fd);
            return 1;
        }
    }
    close(fd);
    return 0;
}
#endif //CUSTOMERTASKS_H
