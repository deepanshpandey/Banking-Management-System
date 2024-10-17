//
// Created by deepanshpandey on 10/16/24.
//

#ifndef LOANOPTIONS_H
#define LOANOPTIONS_H

#include "loantasks.h"
#define BUFFER_SIZE 10240

void loan_menu(int new_socket, const char* email) {
    char buffer[BUFFER_SIZE];
    viewapplied:
                    write(new_socket, "\nSelect an option:\n"
                                      "1. View Applied Loans\n"
                                      "2. Apply for a Loan\n"
                                      "3. Go Back\n"
                                      "Enter Your Choice: ", 71);
    read(new_socket, buffer, sizeof(buffer)-1);
    int loan_option = atoi(buffer);
    switch (loan_option) {
        case 1: {
            //view applied loans
            view_applied_loans(email, new_socket);
            break;
        }
        case 2: {
            double loan_amt;
            int dur_year, dur_month;
            write(new_socket, "Enter Loan Amount: ", 20);
            read(new_socket, buffer, sizeof(buffer)-1);
            loan_amt = atof(buffer);
            write(new_socket, "Enter Loan Duration (years): ", 30);
            read(new_socket, buffer, sizeof(buffer)-1);
            dur_year = atoi(buffer);
            write(new_socket, "Enter Loan Duration (months): ", 32);
            read(new_socket, buffer, sizeof(buffer)-1);
            dur_month = atoi(buffer);
            if(apply_for_loan(email, loan_amt, dur_year, dur_month)==1) {
                write(new_socket, "Loan applied successfully\n", 27);
            }
        }
        case 3: {
            break;
        }
        default: {
            write(new_socket, "Invalid option. Please select again\n", 37);
            goto viewapplied;
        }
    }
}

void emploanmenu(int new_socket, const char* email) {
    char buffer[BUFFER_SIZE];
    viewapplied:
        write(new_socket, "\nSelect an option:\n"
                          "1. View Assigned Application & Exit\n"
                          "2. Approve/Reject/Modify a Loan Application\n"
                          "3. Go Back\n"
                          "Enter Your Choice: ", 89);
    read(new_socket, buffer, sizeof(buffer)-1);
    int loan_option = atoi(buffer);
    switch (loan_option) {
        case 1:
               //view assigned applications
               view_assigned_loans(new_socket, email);
               break;
        case 2:
               //approve/reject/modify application
               view_assigned_loans(new_socket, email);
               modify_loan(new_socket, email);
               break;
        case 3:
               break;
        default:
               write(new_socket, "Invalid option. Please select again\n", 37);
               goto viewapplied;
    }
}

#endif //LOANOPTIONS_H
