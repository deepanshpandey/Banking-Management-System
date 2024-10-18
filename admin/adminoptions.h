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
    admin_retry:
        char email[50], password[50];
        bzero(buffer, BUFFER_SIZE);
        write(new_socket, "\nEnter Email: ", 15);
        read(new_socket, email, sizeof(email)-1);
        email[strcspn(email, "\n")] = '\0';
        write(new_socket, "\nEnter Password: ", 18);
        read(new_socket, password, sizeof(password)-1);
        password[strcspn(password, "\n")] = '\0';
        if (verify_admin(email, password) == 1) {
            bool logout = false;

            write(new_socket, "\nAdmin Login Successful\n", 25);
            const char *admin_menu = "Select an option:\n"
                "1.Add new bank employee/manager\n"
                "2.Modify bank employee/manager details\n"
                "3.Deactivate an Employee\n"
                "4.Modify bank customer details\n"
                "5.Change password\n"
                "6.Logout\n"
                "Enter Your Choice: ";

            write(new_socket, admin_menu, strlen(admin_menu));
            bzero(buffer, BUFFER_SIZE);
            read(new_socket, buffer, BUFFER_SIZE);
            int admin_option = atoi(buffer);
            do {
                switch (admin_option) {
                    case 1: {
                        char name[50], email[50], password[50];
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
                        if(add_employee(name, email, password,is_manager)==1) {
                            write(new_socket, "Employee added successfully\n", 29);
                        }else {
                            write(new_socket, "Employee already exists\n", 26);
                        }
                        break;

                        case 2:{
                            int id;
                            write(new_socket, "Enter Employee Email:", 22);
                            read(new_socket, email, sizeof(email)-1);
                            email[strcspn(email, "\n")] = '\0';
                            if(getemployeeid(email)) {
                                //already exists
                                char name[50], email[50], password[50];
                                int is_manager;
                                write(new_socket, "Enter Employee Name:", 21);
                                read(new_socket, name, sizeof(name)-1);
                                name[strcspn(name, "\n")] = '\0';
                                write(new_socket, "Enter Employee Password:", 25);
                                read(new_socket, password, sizeof(password)-1);
                                password[strcspn(password, "\n")] = '\0';
                                write(new_socket, "Enter Employee Type(1 for manager, 0 for employee):", 52);
                                read(new_socket, is_manager, sizeof(is_manager));
                                if(modify_employee(name, email, password,is_manager)==1) {
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
                            //deactivate/activate employee
                            char empmail[50];
                            int active_status=0;
                            write(new_socket,"Enter Employee Email:",22);
                            read(new_socket,empmail,sizeof(empmail)-1);
                            empmail[strcspn(empmail, "\n")] = '\0';
                            write(new_socket,"Enter 1 to deactivate or 0 to activate:",40);\
                            bzero(buffer,BUFFER_SIZE);
                            read(new_socket,buffer,sizeof(buffer));
                            active_status=atoi(buffer);
                            if(activate_employee(empmail,active_status)==1) {
                                write(new_socket,"Employee status changed successfully\n",38);
                            }
                            break;
                        }

                        case 4: {
                            char email[50];
                            write(new_socket, "Enter Customer Email:", 22);
                            read(new_socket, email, sizeof(email)-1);
                            email[strcspn(email, "\n")] = '\0';
                            if(getcustomerid(email)) {
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
                                //id does not exist
                                write(new_socket, "Customer does not exist\n", 25);
                            }
                            break;
                        }

                        case 5: {
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

                        case 6: {
                            if (logoutadmin(email) == 1) {
                                write(new_socket, "Logout Successful\n", 19);
                            } else {
                                write(new_socket, "Logout Failed\n", 15);
                            }
                            logout = true;
                            break;
                        }

                        default: {
                            write(new_socket, "\nInvalid option. Please select again.\n", 39);
                            goto admin_options;
                        }
                    }
                }
            }
            while(logout == false);
        }
        else {
            write(new_socket, "Login Failed. Try Again?\nType 0 for yes or 1 for no :", 52);
            bzero(buffer, BUFFER_SIZE);
            read(new_socket, buffer, sizeof(buffer));
            if(atoi(buffer)==1) {
                goto admin_retry;
            }
        }
}
#endif //ADMINOPTIONS_H
