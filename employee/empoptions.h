//
// Created by deepanshpandey on 10/14/24.
//

#ifndef EMPOPTIONS_H
#define EMPOPTIONS_H

#include <stdbool.h>
#include "emptasks.h"
#include "../customer/customertasks.h"
#define BUFFER_SIZE 10240

void emp_menu(int new_socket) {
    char buffer[BUFFER_SIZE];
    bool logout = false;
    emp_retry:
    char password[50], email[50], empemail[50];
    write(new_socket, "Enter Email: ", 13);
    read(new_socket, email, sizeof(email)-1);
    email[strcspn(email, "\n")] = '\0';
    strcpy(empemail,email);
    write(new_socket, "Enter Password: ", 16);
    read(new_socket, password, sizeof(password)-1);
    password[strcspn(password, "\n")] = '\0';
    if(verify_employee(email, password)==1) {
        emp_menu:
        write(new_socket, "Employee Login Successful\n", 27);
        const char *employee_menu = "Select an option:\n"
                                    "1. Add New Customer\n"
                                    "2. Modify Customer Details\n"
                                    "3. View Assigned Loan Applications\n"
                                    "4. View Customer Transactions\n"
                                    "5. View Assigned Loan Applications\n"
                                    "6. Change Password\n"
                                    "7. Logout\n";
        write(new_socket, employee_menu, strlen(employee_menu));
        bzero(buffer, BUFFER_SIZE);
        read(new_socket, buffer, sizeof(buffer));
        int employee_option=atoi(buffer);
        switch (employee_option) {
            case 1: {
                //add new customer
                char name[50], email[50], phone[15], password[50];
                double balance;
                char amount[20];
                int account_active=1;
                write(new_socket, "Enter Customer Email:", 22);
                read(new_socket, email, sizeof(email)-1);
                email[strcspn(email, "\n")] = '\0';
                if(lookup(getcustomerid(email))==1) {
                    //already exists
                    write(new_socket, "Customer already exists\n", 26);
                }else {
                    write(new_socket, "Enter Customer Name:", 21);
                    read(new_socket, name, sizeof(name)-1);
                    name[strcspn(name, "\n")] = '\0';
                    write(new_socket, "Enter Customer Phone:", 22);
                    read(new_socket, phone, sizeof(phone)-1);
                    phone[strcspn(phone, "\n")] = '\0';
                    write(new_socket, "Enter Customer Password:", 25);
                    read(new_socket, password, sizeof(password)-1);
                    password[strcspn(password, "\n")] = '\0';
                    write(new_socket, "Enter Customer Balance:", 25);
                    read(new_socket, amount, sizeof(amount)-1);
                    amount[strcspn(amount, "\n")] = '\0';
                    balance=strtod(amount,NULL);
                    int id = add_customer(name, email, phone, password, balance, account_active);
                    if(id) {
                        bzero(buffer, BUFFER_SIZE);
                        sprintf(buffer, "Customer added successfully with ID %d\n", id);
                        write(new_socket,buffer, sizeof(buffer));
                    }else {
                        write(new_socket, "Customer addition failed\n", 27);
                    }
                }
                break;
            }
            case 2: {
                //modify customer details
                char  email[50];
                write(new_socket, "Enter Customer Email:", 22);
                read(new_socket, email, sizeof(email)-1);
                email[strcspn(email, "\n")] = '\0';
                if(customer_lookup(email)==1) {
                    //already exists
                    char name[50], phone[15], password[50];
                    double balance;
                    char amount[20];
                    int account_active=1;
                    write(new_socket, "Enter Customer Name:", 21);
                    read(new_socket, name, sizeof(name)-1);
                    name[strcspn(name, "\n")] = '\0';
                    write(new_socket, "Enter Customer Phone:", 22);
                    read(new_socket, phone, sizeof(phone)-1);
                    phone[strcspn(phone, "\n")] = '\0';
                    write(new_socket, "Enter Customer Password:", 25);
                    read(new_socket, password, sizeof(password)-1);
                    password[strcspn(password, "\n")] = '\0';
                    write(new_socket, "Enter Customer Balance:", 25);
                    read(new_socket, amount, sizeof(amount)-1);
                    amount[strcspn(amount, "\n")] = '\0';
                    balance=strtod(amount,NULL);
                    if(modify_customer(name, email, phone, password, balance, account_active)==1) {
                        write(new_socket, "Customer modified successfully\n", 32);
                    }else {
                        write(new_socket, "Customer modification failed\n", 30);
                    }
                } else {
                    write(new_socket, "Customer does not exist\n", 25);
                }
                break;
            }
            case 3: {
                //view assigned loan applications
                view_assigned_loans(email,new_socket);
                break;
            }
            case 4: {
                //view customer transactions
                char cemail[50];
                write(new_socket, "Enter Customer Email:", 22);
                read(new_socket, cemail, sizeof(cemail)-1);
                cemail[strcspn(cemail, "\n")] = '\0';
                view_transactions(getcustomerid(cemail),new_socket);
                break;
            }
            case 5: {
                //view assigned loan applications
                view_assigned_loans(empemail,new_socket);
                break;
            }
            case 6: {
                //change password
                char password[50];
                write(new_socket, "Enter New Password:", 19);
                read(new_socket, password, sizeof(password)-1);
                password[strcspn(password, "\n")] = '\0';
                if(change_emp_password(empemail, password)==1) {
                    write(new_socket, "Password changed successfully\n", 31);
                }else {
                    write(new_socket, "Password change failed\n", 25);
                }
                break;
            }
            case 7: {
                //logout
                if(logoutemployee(empemail)==1) {
                    write(new_socket, "Logout Successful\n", 18);
                    logout = true;
                }
                else {
                     write(new_socket, "Logout Failed\n", 15);
                }
                break;
            }
            default: {
                write(new_socket, "Invalid option. Please select again\n", 37);
                goto emp_menu;
            }
        }
        if(logout==false) {
            goto emp_menu;
        }
    }
    else {
        write(new_socket, "Login Failed. Try Again?\n(Type 1 for yes or 0 for no) :", 54);
        bzero(buffer, BUFFER_SIZE);
        read(new_socket, buffer, sizeof(buffer));
        if(atoi(buffer)==1) {
            goto emp_retry;
        }
    }

}

void manager_menu(int new_socket) {
    char buffer[BUFFER_SIZE];
    bool logout = false;

    man_retry:
    char password[50], email[50];
    write(new_socket, "Enter Email: ", 13);
    read(new_socket, email, sizeof(email)-1);
    email[strcspn(email, "\n")] = '\0';
    write(new_socket, "Enter Password: ", 16);
    read(new_socket, password, sizeof(password)-1);
    password[strcspn(password, "\n")] = '\0';
    if (verify_manager(email, password) == 1) {
        write(new_socket, "Manager Login Successful\n", 27);
        man_menu:
        const char *employee_menu = "Select an option:\n"
                                    "1. Change Customer Status\n"
                                    "2. Assign Loan Application to an Employee\n"
                                    "3. Review Customer Feedback\n"
                                    "4. Change Password\n"
                                    "5. Logout\n";
        write(new_socket, employee_menu, strlen(employee_menu));
        bzero(buffer, BUFFER_SIZE);
        read(new_socket, buffer, sizeof(buffer));
        int employee_option=atoi(buffer);
        switch(employee_option) {
            case 1: {
                //change customer status
                int id;
                char email[50];
                write(new_socket, "Enter Customer Email:", 19);
                read(new_socket, email, sizeof(email)-1);
                buffer[strcspn(email, "\n")] = '\0';
                if(customer_lookup(email)==1) {
                    //already exists
                    int status = 0;
                    bzero(buffer, BUFFER_SIZE);
                    write(new_socket, "Enter Customer Status (1 for activation, 0 for deactivation):", 23);
                    read(new_socket, buffer, sizeof(buffer));
                    status = atoi(buffer);
                    if(change_customer_status(email, status)==1) {
                        write(new_socket, "Customer status changed successfully\n", 38);
                    }else {
                        write(new_socket, "Customer status change failed\n", 32);
                    }
                }else {
                    //id does not exist
                    write(new_socket, "Customer does not exist\n", 25);
                }
                break;
            }
            case 2: {
                //assign loan application to an employee
                write(new_socket, "Enter Employee Email:", 21);
                read(new_socket, email, sizeof(email)-1);
                email[strcspn(email, "\n")] = '\0';
                assign_loan_to_employee(email,new_socket);
                break;
            }
            case 3: {
                //review customer feedback
                break;
            }
            case 4: {
                //change password
                char password[50];
                write(new_socket, "Enter New Password:", 19);
                read(new_socket, password, sizeof(password)-1);
                password[strcspn(password, "\n")] = '\0';
                if(change_emp_password(email, password)==1) {
                    write(new_socket, "Password changed successfully\n", 31);
                }else {
                    write(new_socket, "Password change failed\n", 25);
                }
                break;
            }
            case 5: {
                if(logoutemployee(email)==1) {
                    write(new_socket, "Logout Successful\n", 18);
                    logout = true;
                }
                else {
                    write(new_socket, "Logout Failed\n", 15);
                }
                break;
            }
            default: {
                write(new_socket, "Invalid option. Please select again\n", 37);
                goto man_menu;
            }
        }
        if(logout == false) {
            goto man_menu;
        }
    }
    else {
        write(new_socket, "Login Failed. Try Again?\n(Type 1 for yes or 0 for no) :", 54);
        bzero(buffer, BUFFER_SIZE);
        read(new_socket, buffer, sizeof(buffer));
        if(atoi(buffer)==1) {
            goto man_retry;
        }
    }
}

#endif //EMPOPTIONS_H
