//
// Created by deepanshpandey on 10/14/24.
//

#ifndef CUSTOMEROPTIONS_H
#define CUSTOMEROPTIONS_H


#include <stdbool.h>
#include "customertasks.h"
#define BUFFER_SIZE 10240

void customer_menu(int new_socket) {
    char buffer[BUFFER_SIZE];
    bool logout = false;
    do {
        char password[50], email[50];
        write(new_socket, "Enter Email:", 12);
        read(new_socket, email, sizeof(email)-1);
        email[strcspn(email, "\n")] = '\0';
        write(new_socket, "Enter Password:", 15);
        read(new_socket, password, sizeof(password)-1);
        password[strcspn(password, "\n")] = '\0';
        if(verify_customer(email, password)==1) {
            cust_menu:
            write(new_socket, "Customer Login Successful\n", 27);
            const char *customer_menu = "Select an option:\n"
                                        "1. View Account Balance\n"
                                        "2. Deposit Money\n"
                                        "3. Withdraw Money\n"
                                        "4. Transfer Funds\n"
                                        "5. Apply For a Loan\n"
                                        "6. Change Password\n"
                                        "7. Add FeedBack\n"
                                        "8. View Transaction History\n"
                                        "9. Logout\n"
                                        "Enter Your Choice: ";
            write(new_socket, customer_menu, strlen(customer_menu));
            int customer_option=atoi(buffer);
            switch (customer_option) {
                case 1: {
                    //view account balance
                    double balance = view_balance(email);
                    sprintf(buffer, "%f", balance);
                    write(new_socket, "Your account balance is: ", 26);
                    write(new_socket, buffer, strlen(buffer));
                    break;
                }
                case 2:{
                    //deposit money
                    double add_amt;
                    write(new_socket, "Enter Amount to Deposit: ", 26);
                    read(new_socket, buffer, sizeof(buffer)-1);
                    add_amt = atof(buffer);
                    if(deposit_money(email, add_amt)==1) {
                        write(new_socket, "Amount deposited successfully\n", 31);
                    }
                    break;
                }
                case 3: {
                    //withdraw money
                    double withdraw_amt;
                    write(new_socket, "Enter Amount to Withdraw: ", 27);
                    read(new_socket, buffer, sizeof(buffer)-1);
                    withdraw_amt = atof(buffer);
                    if(withdraw_money(email, withdraw_amt)==1) {
                        write(new_socket, "Amount withdrawn successfully\n", 31);
                    }
                    break;
                }
                case 4: {
                    //transfer funds
                    char to_email[50];
                    double transfer_amt;
                    write(new_socket, "Enter Email to Transfer Funds: ", 31);
                    read(new_socket, to_email, sizeof(to_email)-1);
                    to_email[strcspn(to_email, "\n")] = '\0';
                    write(new_socket, "Enter Amount to Transfer: ", 27);
                    read(new_socket, buffer, sizeof(buffer)-1);
                    transfer_amt = atof(buffer);
                    if(transfer_money(email, to_email, transfer_amt)==1) {
                        write(new_socket, "Amount transferred successfully\n", 33);
                    }
                    break;
                }
                case 5: {
                    //apply for a loan
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
                    break;
                }
                case 6: {
                    //change password
                    char password[50];
                    write(new_socket, "Enter New Password: ", 21);
                    read(new_socket, password, sizeof(password)-1);
                    password[strcspn(password, "\n")] = '\0';
                    if(change_customer_password(email, password)==1) {
                        write(new_socket, "Password changed successfully\n", 31);
                    }else {
                        write(new_socket, "Password change failed\n", 25);
                    }
                    break;
                }
                case 7: {
                    //add feedback
                    break;
                }
                case 8: {
                    //view transaction history
                    break;
                }
                case 9: {
                    //logout
                    write(new_socket, "Logout\n", 7);
                    logout = true;
                    break;
                }
                default:{
                    write(new_socket, "Invalid option. Please select again\n", 37);
                    goto cust_menu;
                }
            }
        }
    }while (logout == false);
}

#endif //CUSTOMEROPTIONS_H
