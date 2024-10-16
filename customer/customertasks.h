//
// Created by deepanshpandey on 10/9/24.
//

#ifndef CUSTOMERTASKS_H
#define CUSTOMERTASKS_H

#include "../db.h"
#include "../loans/loantasks.h"
#include <fcntl.h>
#include <string.h>

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
double view_balance(const char* email) {
    //view account balance
    const char* file_path = "../db/customers.txt";
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
            cust.balance+=add_amt;
            write(fd, &cust, sizeof(cust));
            close(fd);
            return 1;
        }
    }
    close(fd);
    return 0;
}
int withdraw_money(const char* email, double withdraw_amt) {
    //withdraw money
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
            if(cust.balance>=withdraw_amt) {
                cust.balance-=withdraw_amt;
                write(fd, &cust, sizeof(cust));
                close(fd);
                return 1;
            }else {
                close(fd);
                return 0;
            }
        }
    }
    close(fd);
    return 0;
}
int transfer_money(const char* from_email, const char* to_email, double transfer_amt) {
    //transfer funds
    const char* file_path = "../db/customers.txt";
    int fd=open(file_path, O_RDWR);
    if(fd==-1) {
        perror("Failed to open file");
        return 0;
    }
    Customer cust;
    int rem_success = 0, add_success = 0;
    while(read(fd, &cust, sizeof(cust))>0) {
        if(strcmp(from_email, cust.email)==0) {
            //id already exists
            lseek(fd, -sizeof(cust), SEEK_CUR);
            if(cust.balance>=transfer_amt) {
                cust.balance-=transfer_amt;
                rem_success=1;
                write(fd, &cust, sizeof(cust));
                close(fd);
                fd=open(file_path, O_RDWR);
                while(read(fd, &cust, sizeof(cust))>0) {
                    if(strcmp(to_email, cust.email)==0) {
                        //id already exists
                        lseek(fd, -sizeof(cust), SEEK_CUR);
                        cust.balance+=transfer_amt;
                        add_success=1;
                        write(fd, &cust, sizeof(cust));
                        close(fd);
                        return 1;
                    }
                }
                if (rem_success == 0) {
                    //rollback if anything fails
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
                if (add_success == 0) {
                    fd=open(file_path, O_RDWR);
                    while(read(fd, &cust, sizeof(cust))>0) {
                        if(strcmp(to_email, cust.email)==0) {
                            //id already exists
                            lseek(fd, -sizeof(cust), SEEK_CUR);
                            cust.balance-=transfer_amt;
                            write(fd, &cust, sizeof(cust));
                            close(fd);
                            return 1;
                        }
                    }
                    close(fd);
                }
            }else {
                close(fd);
            }
        }
    }
    close(fd);
    return 0;
}
int apply_for_loan(const char* email, double loan_amt, int dur_year, int dur_month) {
    //apply for a loan
    const char* file_path = "../db/customers.txt";
    int fd=open(file_path, O_RDWR);
    if(fd==-1) {
        perror("Failed to open file");
        return 0;
    }
    Customer cust;
    int to_id;
    while(read(fd, &cust, sizeof(cust))>0) {
        if(strcmp(email, cust.email)==0) {
            //id already exists
            lseek(fd, -sizeof(cust), SEEK_CUR);
            to_id=cust.id;
            break;
        }
    }
    close(fd);
    const char* file_path2 = "../db/loanapplications.txt";
    int fd2=open(file_path2, O_RDWR | O_CREAT | O_APPEND, 0666);
    if(fd2==-1) {
        perror("Failed to open file");
        return 0;
    }
    LoanApplication loan;
    while(read(fd2, &loan, sizeof(loan))>0) {
        if(loan.customer_id==to_id) {
            //id already exists
            loan.loan_id = generate_loan_id();
            loan.amount = loan_amt;
            strcpy(loan.status, "pending");
            loan.assigned_employee_id = -1;
            loan.dur_year = dur_year;
            loan.dur_month = dur_month;
            loan.remaining_amount = -1;
            loan.interest_rate = 0;
            write(fd2, &loan, sizeof(loan));
            close(fd2);
            return 0;
        }
    }
    close(fd2);
    return 0;
}
#endif //CUSTOMERTASKS_H
