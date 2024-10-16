//
// Created by deepanshpandey on 10/14/24.
//

#ifndef ADMINOPTIONS_H
#define ADMINOPTIONS_H

#include <stdbool.h>
#include "../employee/emptasks.h"
#include "admintasks.h"
#define BUFFER_SIZE 10240

void admin_menu(int new_socket) {
    char buffer[BUFFER_SIZE];
    bool logout = false;
    const char *attempt = "enter credentials";
        char email[50], password[50];
        write(new_socket, "Enter Email:", 12);
        read(new_socket, email, sizeof(email)-1);
        email[strcspn(email, "\n")] = '\0';
        write(new_socket, "Enter Password:", 15);
        read(new_socket, password, sizeof(password)-1);
        password[strcspn(password, "\n")] = '\0';
        if (verify_admin(email, password) == 1) {
            write(new_socket, "Admin Login Successful\n", 23);
            const char *admin_menu = "Select an option:\n"
                "1.Add new bank employee/manager\n"
                "2.Modify bank employee/manager details\n"
                "3.Modify bank customer details\n"
                "4.Change password\n"
                "5.Logout\n";

            write(new_socket, admin_menu, strlen(admin_menu));
            bzero(buffer, BUFFER_SIZE);
            read(new_socket, buffer, BUFFER_SIZE);
            int admin_option = atoi(buffer);
            do {
                switch (admin_option) {
                    case 1: {
                        int id;
                        char name[50], email[50], password[50];
                        write(new_socket, "Enter Employee ID:", 20);
                        bzero(buffer, BUFFER_SIZE);
                        read(new_socket,buffer,sizeof(buffer));
                        id=atoi(buffer);
                        write(new_socket, "Enter Employee Name:", 21);
                        read(new_socket, name, sizeof(name)-1);
                        name[strcspn(name, "\n")] = '\0';
                        write(new_socket, "Enter Employee Email:", 22);
                        read(new_socket, email, sizeof(email)-1);
                        email[strcspn(email, "\n")] = '\0';
                        write(new_socket, "Enter Employee Password:", 25);
                        read(new_socket, password, sizeof(password)-1);
                        password[strcspn(password, "\n")] = '\0';
                        write(new_socket, "Enter Employee Type(1 for manager, 0 for employee):", 52);
                        bzero(buffer, BUFFER_SIZE);
                        read(new_socket, buffer, sizeof(buffer));
                        int is_manager = atoi(buffer);
                        if(add_employee(id, name, email, password,is_manager)==1) {
                            write(new_socket, "Employee added successfully\n", 29);
                        }else {
                            write(new_socket, "Employee already exists\n", 26);
                        }
                        break;
                    }

                    case 2:{
                        int id;
                        write(new_socket, "Enter Employee ID:", 19);
                        read(new_socket, id, sizeof(id));

                        if(lookup(id)==1) {
                            //already exists
                            char name[50], email[50], password[50];
                            int is_manager;
                            write(new_socket, "Enter Employee Name:", 21);
                            read(new_socket, name, sizeof(name)-1);
                            name[strcspn(name, "\n")] = '\0';
                            write(new_socket, "Enter Employee Email:", 22);
                            read(new_socket, email, sizeof(email)-1);
                            email[strcspn(email, "\n")] = '\0';
                            write(new_socket, "Enter Employee Password:", 25);
                            read(new_socket, password, sizeof(password)-1);
                            password[strcspn(password, "\n")] = '\0';
                            write(new_socket, "Enter Employee Type(1 for manager, 0 for employee):", 52);
                            read(new_socket, is_manager, sizeof(is_manager));
                            if(modify_employee(id, name, email, password,is_manager)==1) {
                                write(new_socket, "Employee modified successfully\n", 32);
                            }else {
                                write(new_socket, "Employee modification failed\n", 30);
                            }
                        }else {
                            //id does not exist
                            write(new_socket, "Employee does not exist\n", 25);
                        }
                        break;
                    }

                    case 3: {
                        int id;
                        write(new_socket, "Enter Customer ID:", 19);
                        read(new_socket, id, sizeof(id));
                        if(customer_lookup(id)==1) {
                            //already exists
                            char name[50], email[50], phone[15], password[50];
                            double balance;
                            char amount[20];
                            int account_active=1;
                            write(new_socket, "Enter Customer Name:", 21);
                            read(new_socket, name, sizeof(name)-1);
                            name[strcspn(name, "\n")] = '\0';
                            write(new_socket, "Enter Customer Email:", 22);
                            read(new_socket, email, sizeof(email)-1);
                            email[strcspn(email, "\n")] = '\0';
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
                            if(modify_customer(id, name, email, phone, password, balance, account_active)==1) {
                                write(new_socket, "Customer modified successfully\n", 32);
                            }else {
                                write(new_socket, "Customer modification failed\n", 30);
                            }
                        } else {
                            //id does not exist
                            write(new_socket, "Customer does not exist\n", 25);
                        }
                        break;
                    }

                    case 4: {
                        //change password
                        char password[50];
                        write(new_socket, "Enter New Password:", 19);
                        read(new_socket, password, sizeof(password)-1);
                        password[strcspn(password, "\n")] = '\0';
                        if(change_admin_password(email, password)==1) {
                            write(new_socket, "Password changed successfully\n", 31);
                        }
                        break;
                    }

                    case 5: {
                        write(new_socket, "Logout\n", 7);
                        logout = true;
                        break;
                    }

                    default: {
                        write(new_socket, "Invalid option. Please select again\n", 37);
                        write(new_socket, admin_menu, strlen(admin_menu));
                        break;
                    }

                }
            }
            while(logout == false);
        } else {
            write(new_socket, "Invalid Admin Credentials\n", 26);
            bzero(new_socket, BUFFER_SIZE);
            write(new_socket,attempt,strlen(attempt));
        }
}
#endif //ADMINOPTIONS_H
