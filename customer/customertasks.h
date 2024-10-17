//
// Created by deepanshpandey on 10/9/24.
//

#ifndef CUSTOMERTASKS_H
#define CUSTOMERTASKS_H

#include "../db/db.h"
#include "../loans/loantasks.h"
#include "../db/ottasks.h"
#include "../transactions/transtasks.h"
#include <fcntl.h>
#include <string.h>

int getcustomerid(const char *email) {
    const char* file_path = "../db/customers.db";
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    Customer cust;
    while (read(fd, &cust, sizeof(cust)) > 0) {
        if(strcmp(email, cust.email) == 0) {
            close(fd);
            return cust.id;
        }
    }
    return 0;
}
int verify_customer(const char* email, const char* password) {
    const char* file_path = "../db/customers.db";
    Customer cust;
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }

    while (read(fd, &cust, sizeof(cust)) > 0) {
        if(strcmp(email, cust.email) == 0 && strcmp(password, cust.password) == 0 && cust.login_status==0) {
            cust.login_status=1;
            lseek(fd, -sizeof(cust), SEEK_CUR);
            write(fd, &cust, sizeof(cust));
            close(fd);
            return 1; // Record found
        }
    }
    return 0; // Record not found
}
int logoutcustomer(const char* email) {
    const char* file_path = "../db/customers.db";
    int fd = open(file_path, O_RDWR);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    Customer cust;
    while (read(fd, &cust, sizeof(cust)) > 0) {
        if(strcmp(email, cust.email) == 0) {
            lseek(fd, -sizeof(cust), SEEK_CUR);
            write(fd, &cust, sizeof(cust));
            close(fd);
            return 1; // Record found
        }
    }
    return 0; // Record not found
}
int change_customer_password(const char* email, const char* password) {
    //change customer password
    const char* file_path = "../db/customers.db";
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
double view_balance(const char* email) {
    //view account balance
    const char* file_path = "../db/customers.db";
    int fd=open(file_path, O_RDONLY);
    if(fd==-1) {
        perror("Failed to open file");
        return 0;
    }
    Customer cust;
    while(read(fd, &cust, sizeof(cust))>0) {
        if(strcmp(email, cust.email)==0) {
            //id already exists
            close(fd);
            return cust.balance;
        }
    }
    close(fd);
    return 0;
}
int deposit_money(const char* email, double add_amt) {
    //deposit money
    const char* file_path = "../db/customers.db";
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
            cust.balance+=add_amt;
            write(fd, &cust, sizeof(cust));
            deposit_transaction(cust.id, add_amt);
            close(fd);
            return 1;
        }
    }
    close(fd);
    return 0;
}
int withdraw_money(const char* email, double withdraw_amt) {
    //withdraw money
    const char* file_path = "../db/customers.db";
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
            if(cust.balance>=withdraw_amt) {
                cust.balance-=withdraw_amt;
                write(fd, &cust, sizeof(cust));
                withdrawal_transaction(cust.id, withdraw_amt);
                close(fd);
                return 1;
            }
            else {
                break;
            }
        }
    }
    close(fd);
    return 0;
}
int transfer_money(const char* from_email, const char* to_email, double transfer_amt) {
    //transfer funds
    const char* file_path = "../db/customers.db";
    Customer cust;
    int rem_success = 0;
    int add_success = 0;
    int event1 = 0, event2 = 0;


    int fd=open(file_path, O_RDWR);
    while(read(fd, &cust, sizeof(cust))>0) { // deduct from sender
        if(strcmp(from_email, cust.email)==0) {
            //id already exists
            lseek(fd, -sizeof(cust), SEEK_CUR);
            if(cust.balance>=transfer_amt) {
                cust.balance-=transfer_amt;
                rem_success=1;
                write(fd, &cust, sizeof(cust));
                event1=1;
            }
            else {
                return 0;
            }
        }
        if(event1==1) {
            break;
        }
    }
    close(fd);

    fd=open(file_path, O_RDWR);
    while(read(fd, &cust, sizeof(cust))>0) { // add to receiver
        if(strcmp(to_email, cust.email)==0) {
            //id already exists
            lseek(fd, -sizeof(cust), SEEK_CUR);
            cust.balance+=transfer_amt;
            add_success=1;
            write(fd, &cust, sizeof(cust));
            event2=1;
        }
        if(event2==1) {
            break;
        }
    }
    close(fd);

    if(rem_success==1 && add_success==1 && event1==1 && event2==1) {//all events successful
        write_transaction(getcustomerid(from_email), getcustomerid(to_email), transfer_amt);
        return 1;
    }

    if (rem_success == 1 && event1 == 0) {  //rollback if anything fails
        fd=open(file_path, O_RDWR);
        while(read(fd, &cust, sizeof(cust))>0) {
            if(strcmp(from_email, cust.email)==0) {
                //id already exists
                lseek(fd, -sizeof(cust), SEEK_CUR);
                cust.balance+=transfer_amt;
                write(fd, &cust, sizeof(cust));
                close(fd);
            }
        }
    }
    return 0;
}
int add_feedback(const char* email, const char* feedback) {
    //add feedback
    const char* file_path = "../db/feedbacks.db";
    int fd=open(file_path, O_WRONLY|O_CREAT|O_APPEND,0666);
    if(fd==-1) {
        perror("Failed to open file");
        return 0;
    }
    Feedback feed;
    feed.feedback_id = generate_admin_id();
    strcpy(feed.feedback, feedback);
    strcpy(feed.mail, email);
    DateTime dt;
    initializeDateTime(&dt);
    feed.date_time = dt;
    write(fd, &feed, sizeof(feed));
    close(fd);
    return 1;
}

#endif //CUSTOMERTASKS_H
