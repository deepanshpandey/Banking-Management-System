//
// Created by deepanshpandey on 10/17/24.
//

#ifndef OTTASKS_H
#define OTTASKS_H

#include <fcntl.h>

#include "../db/db.h"

int generateemployeeid() {
    const char* file_path = "../db/otdata.db";
    int fd = open(file_path, O_RDWR);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    OTData otdata = {0};
    read(fd, &otdata, sizeof(otdata));
    int id=otdata.highest_employee_id;
    otdata.highest_employee_id = otdata.highest_employee_id + 1;
    id = id+1;
    lseek(fd, 0, SEEK_SET);
    write(fd, &otdata, sizeof(otdata));
    close(fd);
    return id;
}
int generatecustomerid() {
    const char* file_path = "../db/otdata.db";
    int fd = open(file_path, O_RDWR);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    OTData otdata = {0};
    read(fd, &otdata, sizeof(otdata));
    int id=otdata.highest_customer_id;
    otdata.highest_customer_id = otdata.highest_customer_id + 1;
    id = id+1;
    lseek(fd, 0, SEEK_SET);
    write(fd, &otdata, sizeof(otdata));
    close(fd);
    return id;
}
int generateloanid() {
    const char* file_path = "../db/otdata.db";
    int fd = open(file_path, O_RDWR);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    OTData otdata = {0};
    read(fd, &otdata, sizeof(otdata));
    int id=otdata.highest_loan_id;
    otdata.highest_loan_id = otdata.highest_loan_id + 1;
    id = id+1;
    lseek(fd, 0, SEEK_SET);
    write(fd, &otdata, sizeof(otdata));
    close(fd);
    return id;
}
int generate_feedback_id() {
    const char* file_path = "../db/otdata.db";
    int fd = open(file_path, O_RDWR);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    OTData otdata = {0};
    read(fd, &otdata, sizeof(otdata));
    int id=otdata.highest_feedback_id;
    otdata.highest_feedback_id = otdata.highest_feedback_id + 1;
    id = id+1;
    lseek(fd, 0, SEEK_SET);
    write(fd, &otdata, sizeof(otdata));
    close(fd);
    return id;
}
int generate_admin_id() {
    const char* file_path = "../db/otdata.db";
    int fd = open(file_path, O_RDWR);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    OTData otdata = {0};
    read(fd, &otdata, sizeof(otdata));
    int id=otdata.highest_admin_id;
    otdata.highest_admin_id = otdata.highest_admin_id + 1;
    id = id+1;
    lseek(fd, 0, SEEK_SET);
    write(fd, &otdata, sizeof(otdata));
    close(fd);
    return id;
}
int generate_transaction_id() {
    const char* file_path = "../db/otdata.db";
    int fd = open(file_path, O_RDWR);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    OTData otdata = {0};
    read(fd, &otdata, sizeof(otdata));
    int id=otdata.highest_transaction_id;
    otdata.highest_transaction_id = otdata.highest_transaction_id + 1;
    id = id+1;
    lseek(fd, 0, SEEK_SET);
    write(fd, &otdata, sizeof(otdata));
    close(fd);
    return id;
}

#endif //OTTASKS_H
