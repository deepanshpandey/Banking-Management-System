//
// Created by deepanshpandey on 10/16/24.
//

#ifndef LOANTASKS_H
#define LOANTASKS_H

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "../db/db.h"
#include "../db/ottasks.h""
#include "../employee/emptasks.h"
#include "../transactions/transtasks.h"

#define BUFFER_SIZE 10240

// int getemployeeid(const char *email) {
//     //get employee id
//     const char* file_path = "../db/employees.db";
//     int fd=open(file_path, O_RDONLY,0777);
//     if(fd==-1) {
//         perror("Failed to open file");
//         return 0;
//     }
//     Employee emp;
//     while(read(fd, &emp, sizeof(emp))>0) {
//         if(strcmp(email, emp.email)==0) {
//             //id already exists
//             close(fd);
//             return emp.id;
//         }
//     }
//     close(fd);
//     return 0;
// }
void view_applied_loans(const char* email, int new_socket){
    const char* file_path = "../db/customers.db";
    int fd=open(file_path, O_RDWR);
    if(fd==-1) {
        perror("Failed to open file");
        return;
    }
    Customer cust;
    int cid;
    while(read(fd, &cust, sizeof(cust))>0) {
        if(strcmp(email, cust.email)==0) {
            //id already exists
            lseek(fd, -sizeof(cust), SEEK_CUR);
            cid=cust.id;
            break;
        }
    }
    close(fd);
    char buffer[BUFFER_SIZE];
    file_path = "../db/loans.db";
    fd = open(file_path, O_RDWR | O_CREAT | O_APPEND, 0666);  // Open file in append mode
    if (fd == -1) {
        perror("Failed to open file");
    }
    LoanApplication loan;
    while(read(fd, &loan, sizeof(loan))>0) {
        if(loan.customer_id==cid) {
            bzero(buffer, BUFFER_SIZE);
            sprintf(buffer, "Loan ID: %d\nAmount: %f\nStatus: %s\nAssigned Employee ID: %d\nDuration: %d years %d months\nRemaining Amount: %f\nInterest Rate: %f\n", loan.loan_id, loan.amount, loan.status, loan.assigned_employee_id, loan.month_year.years, loan.month_year.months, loan.remaining_amount, loan.interest_rate);
            write(new_socket, buffer, BUFFER_SIZE);
            bzero(buffer, BUFFER_SIZE);
            sprintf(buffer, "Application Date: %d-%d-%d %d:%d:%d\n", loan.application_date.day, loan.application_date.month, loan.application_date.year, loan.application_date.hour, loan.application_date.minute, loan.application_date.second);
            write(new_socket, buffer, BUFFER_SIZE);
            bzero(buffer, BUFFER_SIZE);
            if(strcmp(loan.status, "Approved")==0) {
                sprintf(buffer, "Application Approved Date: %d-%d-%d %d:%d:%d\n", loan.approved_date.day, loan.approved_date.month, loan.approved_date.year, loan.approved_date.hour, loan.approved_date.minute, loan.approved_date.second);
                write(new_socket, buffer, BUFFER_SIZE);
            }
        }
    }
    close(fd);
}
int apply_for_loan(const char* email, double loan_amt, int dur_year, int dur_month) {
    //apply for a loan
    const char* file_path = "../db/customers.db";
    int fd=open(file_path, O_RDWR);
    if(fd==-1) {
        perror("Failed to open file");
        return -1;
    }
    Customer cust;
    int to_id = -1;
    while(read(fd, &cust, sizeof(cust))>0) {
        if(strcmp(email, cust.email)==0) {
            //id already exists
            lseek(fd, -sizeof(cust), SEEK_CUR);
            to_id=cust.id;
            break;
        }
    }
    close(fd);
    const char* file_path2 = "../db/loans.db";
    int fd2=open(file_path2, O_RDWR | O_CREAT | O_APPEND, 0666);  //open in append mode
    if(fd2==-1) {
        perror("Failed to open file");
        return -1;
    }
    int id=generateloanid();
    LoanApplication loan;
    loan.loan_id=id;
    loan.customer_id=to_id;
    strcpy(loan.email,email);
    loan.amount=loan_amt;
    strcpy(loan.status,"Pending");
    loan.assigned_employee_id=-1;
    loan.month_year.years=dur_year;
    loan.month_year.months=dur_month;
    loan.remaining_amount=loan_amt;
    loan.interest_rate=0.0;
    DateTime dt;
    initializeDateTime(&dt);
    loan.application_date=dt;
    write(fd2, &loan, sizeof(loan));
    close(fd2);
    return 1;
}
int assign_loan_to_employee(const char* email,int loan_id) { //pass loan id and email of employee
   LoanApplication loan;
    const char* file_path = "../db/loans.db";
    int fd = open(file_path, O_RDWR);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    while(read(fd, &loan, sizeof(loan))>0) {
        if(loan.loan_id==loan_id) {
            //id already exists
            lseek(fd, -sizeof(loan), SEEK_CUR);
            loan.assigned_employee_id=getemployeeid(email);
            write(fd, &loan, sizeof(loan));
            close(fd);
            return 1;
        }
    }
}
int view_assigned_loans(const char* email, int new_socket) {
    int eid=getemployeeid(email);
    const char* file_path = "../db/loans.db";
    int fd=open(file_path, O_RDWR);
    if(fd==-1) {
        perror("Failed to open file");
        return 0;
    }
    char buffer[BUFFER_SIZE];
    LoanApplication loan;
    while(read(fd, &loan, sizeof(loan))>0) {
        if(loan.assigned_employee_id==eid) {
            //id already exists
            bzero(buffer, BUFFER_SIZE);
            sprintf(buffer, "Loan ID: %d\nAmount: %f\nStatus: %s\nAssigned Employee ID: %d\nDuration: %d years %d months\nRemaining Amount: %f\nInterest Rate: %f\nApproval Status: %s\n", loan.loan_id, loan.amount, loan.status, loan.assigned_employee_id, loan.month_year.years, loan.month_year.months, loan.remaining_amount, loan.interest_rate, loan.status);
            write(new_socket, buffer, BUFFER_SIZE);
            bzero(buffer, BUFFER_SIZE);
            sprintf(buffer, "Application Date: %d-%d-%d %d:%d:%d\n", loan.application_date.day, loan.application_date.month, loan.application_date.year, loan.application_date.hour, loan.application_date.minute, loan.application_date.second);
            write(new_socket, buffer, BUFFER_SIZE);
            bzero(buffer, BUFFER_SIZE);
            if(strcmp(loan.status, "Approved")==0) {
                sprintf(buffer, "Application Approved Date: %d-%d-%d %d:%d:%d\n", loan.approved_date.day, loan.approved_date.month, loan.approved_date.year, loan.approved_date.hour, loan.approved_date.minute, loan.approved_date.second);
                write(new_socket, buffer, BUFFER_SIZE);
            }
            close(fd);
            return 1;
        }
    }
    close(fd);
    return 0;
}
int modify_loan(int new_socket, const char* email) {
    char buffer[BUFFER_SIZE];
    write(new_socket, "Enter Loan ID to Modify: ", 26);
    read(new_socket, buffer, sizeof(buffer)-1);
    int loan_id = atoi(buffer);
    const char* file_path = "../db/loans.db";
    int fd=open(file_path, O_RDWR);
    if(fd==-1) {
        perror("Failed to open file");
        return 0;
    }
    LoanApplication loan;//below will show assigned applications
    while(read(fd, &loan, sizeof(loan))>0) {
        if(loan.loan_id==loan_id) {
            //id already exists
            bzero(buffer, BUFFER_SIZE);
            sprintf(buffer, "\nLoan ID: %d\nAmount: %f\nStatus: %s\nAssigned Employee ID: %d\nDuration: %d years %d months\nRemaining Amount: %f\nInterest Rate: %f\n", loan.loan_id, loan.amount, loan.status, loan.assigned_employee_id, loan.month_year.years, loan.month_year.months, loan.remaining_amount, loan.interest_rate);
            write(new_socket, buffer, BUFFER_SIZE);
            bzero(buffer, BUFFER_SIZE);
            sprintf(buffer, "Application Date: %d-%d-%d %d:%d:%d\n", loan.application_date.day, loan.application_date.month, loan.application_date.year, loan.application_date.hour, loan.application_date.minute, loan.application_date.second);
            write(new_socket, buffer, BUFFER_SIZE);
            bzero(buffer, BUFFER_SIZE);
            if(strcmp(loan.status, "Approved")==0) {
                sprintf(buffer, "Application Approved Date: %d-%d-%d %d:%d:%d\n", loan.approved_date.day, loan.approved_date.month, loan.approved_date.year, loan.approved_date.hour, loan.approved_date.minute, loan.approved_date.second);
                write(new_socket, buffer, BUFFER_SIZE);
            }
            break;
        }
    }
    loan_options:
    const char* modify_options = "Select an option:\n"
                                 "1. Approve Loan\n"
                                 "2. Reject Loan\n"
                                 "3. View Applicant's Details\n"
                                 "4. Modify Loan's remaining amount or repayment time\n"
                                 "Enter Your Choice: ";
    write(new_socket, modify_options, strlen(modify_options));
    bzero(buffer, BUFFER_SIZE);
    read(new_socket, buffer, BUFFER_SIZE);
    int modify_option = atoi(buffer);
    switch (modify_option) {
        case 1: {
            if(strcmp(loan.status, "Approved")==0) {
                write(new_socket, "Loan already approved\n", 23);
            }
            else {
                write(new_socket, "Enter Interest Rate: ", 22);
                bzero(buffer, BUFFER_SIZE);
                read(new_socket, buffer, BUFFER_SIZE);
                int interest_rate = atoi(buffer);
                loan.interest_rate=interest_rate;
                write(new_socket, "\nConfirm Approval of Loan? (1 for Yes, 0 for No): ", 52);
                bzero(buffer, BUFFER_SIZE);
                read(new_socket, buffer, BUFFER_SIZE);
                int confirm = atoi(buffer);
                if(confirm==1) {
                    strcpy(loan.status, "Approved");
                    DateTime dt;
                    initializeDateTime(&dt);
                    loan.approved_date=dt;
                    loan.remaining_amount=loan.amount;
                    const char* file_path1= "../db/customers.db";
                    int fd1=open(file_path1, O_RDWR);
                    if(fd1==-1) {
                        perror("Failed to open file");
                        return 0;
                    }
                    Customer cust;
                    while(read(fd1, &cust, sizeof(cust))>0) {
                        if(cust.id==loan.customer_id) {
                            //id already exists
                            lseek(fd1, -sizeof(cust), SEEK_CUR);
                            cust.balance = cust.balance + loan.amount;
                            loan_transaction(loan.loan_id,cust.id, loan.amount);
                            write(fd1, &cust, sizeof(cust));
                            break;
                        }
                    }
                    close(fd1);
                    write(fd, &loan, sizeof(loan));
                    write(new_socket, "\nLoan Approved\n", 14);
                }
            }
            break;
        }
        case 2: {
            strcpy(loan.status, "Rejected");
            write(new_socket, "Loan Rejected\n", 14);
            break;
        }
        case 3: {
            Customer cust;
            const char* file_path1= "../db/customers.db";
            int fd1=open(file_path1, O_RDONLY);\
            if(fd1==-1) {
                perror("Failed to open file");
                return 0;
            }
            while(read(fd1, &cust, sizeof(cust))>0) {
                if(cust.id==loan.customer_id) {
                    //id already exists
                    bzero(buffer, BUFFER_SIZE);
                    sprintf(buffer, "Applicant's Balance: %f\n", cust.balance);
                    break;
                }
            }
            close(fd);
            write(new_socket, buffer, BUFFER_SIZE);
            view_transactions(loan.customer_id, new_socket);
            goto loan_options; // after viewing applicant's details, show loan options again
            break;
            case 4:
                bzero(buffer, BUFFER_SIZE);
            sprintf(buffer, "Current Remaining Amount: %f\n", loan.remaining_amount);
            write(new_socket, buffer, BUFFER_SIZE);
            break;
            default:{
                write(new_socket, "Invalid option. Please select again\n", 37);
                goto loan_options;
            }
        }

    }
    close(fd);
}

#endif //LOANTASKS_H
