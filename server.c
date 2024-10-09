#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "db.h"
#include "admin/admintasks.h"
#include "employee/emptasks.h"
#include "customer/customertasks.h"
#define PORT 8091
#define BUFFER_SIZE 102400

void *handle_client(void *socket_desc) {
    int new_socket = *(int *)socket_desc;
    char buffer[BUFFER_SIZE];
    int read_size;

    const char *menu = "Select an option:\n"
                        "1. Customer Login\n"
                        "2. Employee Login\n"
                        "3. Manager Login\n"
                        "4. Admin Login\n";
    send(new_socket, menu, strlen(menu), 0);

    while ((read_size = read(new_socket, buffer, BUFFER_SIZE)) > 0) {
        int option = atoi(buffer);

        switch (option) {
            case 1: {
                char password[50], email[50];
                write(new_socket, "Enter Email:", 12);
                read(new_socket, email, sizeof(email)-1);
                email[strcspn(email, "\n")] = '\0';
                write(new_socket, "Enter Password:", 15);
                read(new_socket, password, sizeof(password)-1);
                password[strcspn(password, "\n")] = '\0';
                if(verify_customer(email, password)==1) {
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
                                                "9. Logout\n";
                    send(new_socket, customer_menu, strlen(customer_menu), 0);
                    int customer_option=atoi(buffer);
                    switch (customer_option) {
                        case 1: {
                            //view account balance
                            break;
                        }
                        case 2:{
                            //deposit money
                            break;
                        }
                        case 3: {
                            //withdraw money
                            break;
                        }
                        case 4: {
                            //transfer funds
                            break;
                        }
                        case 5: {
                            //apply for a loan
                            break;
                        }
                        case 6: {
                            //change password
                            char password[50];
                            write(new_socket, "Enter New Password:", 19);
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
                            write(new_socket,customer_menu,strlen(customer_menu));
                            break;
                        }
                        default:{
                            write(new_socket, "Invalid option. Please select again\n", 37);
                            write(new_socket, customer_menu, strlen(customer_menu));
                            break;
                        }
                    }
                }else {
                    write(new_socket, "Invalid Customer Credentials\n", 30);
                    write(new_socket, menu, strlen(menu));
                }
            }
            case 2: {
                char password[50], email[50];
                write(new_socket, "Enter Email:", 12);
                read(new_socket, email, sizeof(email)-1);
                email[strcspn(email, "\n")] = '\0';
                write(new_socket, "Enter Password:", 15);
                read(new_socket, password, sizeof(password)-1);
                password[strcspn(password, "\n")] = '\0';
                if(verify_employee(email, password)==1) {
                    write(new_socket, "Employee Login Successful\n", 27);
                    const char *employee_menu = "Select an option:\n"
                                                "1. Add New Customer\n"
                                                "2. Modify Customer Details\n"
                                                "3. View Assigned Loan Applications\n"
                                                "4. View Customer Transactions\n"
                                                "5. View Assigned Loan Applications\n"
                                                "6. Change Password\n"
                                                "7. Logout\n";
                    send(new_socket, employee_menu, strlen(employee_menu), 0);
                    int employee_option=atoi(buffer);
                    switch (employee_option) {
                        case 1: {
                            //add new customer
                            int id;
                            char name[50], email[50], phone[15], password[50];
                            double balance;
                            char amount[20];
                            int account_active=1;
                            write(new_socket, "Enter Customer ID:", 19);
                            read(new_socket, id, sizeof(id));
                            if(lookup(id)==1) {
                                //already exists
                                write(new_socket, "Customer already exists\n", 26);
                            }else {
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
                                if(add_customer(id, name, email, phone, password, balance, account_active)==1) {
                                    write(new_socket, "Customer added successfully\n", 29);
                                }else {
                                    write(new_socket, "Customer addition failed\n", 27);
                                }
                            }
                            break;
                        }
                        case 2: {
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
                                write(new_socket, "Customer does not exist\n", 25);
                            }
                            break;
                        }
                        case 3: {
                            break;
                        }
                        case 4: {
                            break;
                        }
                        case 5: {
                            break;
                        }
                        case 6: {
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
                        case 7: {
                            //logout
                            write(new_socket, "Logout\n", 7);
                            write(new_socket,employee_menu,strlen(employee_menu));
                            break;
                        }
                        default: {
                            write(new_socket, "Invalid option. Please select again\n", 37);
                            write(new_socket, employee_menu, strlen(employee_menu));
                            break;
                        }
                    }
                }else {
                    write(new_socket, "Invalid Employee Credentials\n", 30);
                }
                break;
            }
            case 3: {
                char password[50], email[50];
                write(new_socket, "Enter Email:", 12);
                read(new_socket, email, sizeof(email)-1);
                email[strcspn(email, "\n")] = '\0';
                write(new_socket, "Enter Password:", 15);
                read(new_socket, password, sizeof(password)-1);
                password[strcspn(password, "\n")] = '\0';
                if (verify_employee(email, password) == 1) {
                    write(new_socket, "Manager Login Successful\n", 27);
                    const char *employee_menu = "Select an option:\n"
                                                "1. Change Customer Status\n"
                                                "2. Assign Loan Application to an Employee\n"
                                                "3. Review Customer Feeedback\n"
                                                "4. Change Password\n"
                                                "5. Logout\n";
                    send(new_socket, employee_menu, strlen(employee_menu), 0);
                    int employee_option=atoi(buffer);
                    switch(employee_option) {
                        case 1: {
                            //change customer status
                            int id;
                            write(new_socket, "Enter Customer ID:", 19);
                            read(new_socket, id, sizeof(id));
                            if(customer_lookup(id)==1) {
                                //already exists
                                char status[20];
                                write(new_socket, "Enter Customer Status (1 for activation, 0 for deactivation):", 23);
                                read(new_socket, status, sizeof(status)-1);
                                status[strcspn(status, "\n")] = '\0';
                                if(change_customer_status(id, status)==1) {
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
                            write(new_socket, "Logout\n", 7);
                            write(new_socket,menu,strlen(menu));
                            break;
                        }
                        default: {
                            write(new_socket, "Invalid option. Please select again\n", 37);
                            write(new_socket, employee_menu, strlen(employee_menu));
                            break;
                        }
                    }
                }else {
                    write(new_socket, "Invalid Employee Credentials\n", 30);
                }
            }
            case 4: {
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
                        "1.Add new bank employee\n"
                        "2.Add new bank manager\n"
                        "3.Modify bank employee details\n"
                        "4.Modify bank manager details\n"
                        "5.Modify bank customer details\n"
                        "6.Change password\n"
                        "7.Logout\n";
                    write(new_socket, admin_menu, strlen(admin_menu));
                    int admin_option=atoi(buffer);
                    switch (admin_option) {
                        case 1:case 2: {
                            int id;
                            char name[50], email[50], password[50];
                            int is_manager=(admin_option==1)?0:1;;
                            write(new_socket, "Enter Employee ID:", 19);
                            read(new_socket,id,sizeof(id));
                            write(new_socket, "Enter Employee Name:", 21);
                            read(new_socket, name, sizeof(name)-1);
                            name[strcspn(name, "\n")] = '\0';
                            write(new_socket, "Enter Employee Email:", 22);
                            read(new_socket, email, sizeof(email)-1);
                            email[strcspn(email, "\n")] = '\0';
                            write(new_socket, "Enter Employee Password:", 25);
                            read(new_socket, password, sizeof(password)-1);
                            password[strcspn(password, "\n")] = '\0';
                            if(add_employee(id, name, email, password,is_manager)==1) {
                                write(new_socket, "Employee added successfully\n", 29);
                            }else {
                                write(new_socket, "Employee already exists\n", 26);
                            }
                            break;
                        }
                        case 3: case 4:{
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
                        case 5: {
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
                        case 6: {
                            //change password
                            char password[50];
                            write(new_socket, "Enter New Password:", 19);
                            read(new_socket, password, sizeof(password)-1);
                            password[strcspn(password, "\n")] = '\0';
                            if(change_admin_password(email, password)==1) {
                                write(new_socket, "Password changed successfully\n", 31);
                                break;
                            }
                        }
                        case 7: {
                            write(new_socket, "Logout\n", 7);
                            write(new_socket,menu,strlen(menu));
                            break;
                        }
                        default: {
                            write(new_socket, "Invalid option. Please select again\n", 37);
                            write(new_socket, admin_menu, strlen(admin_menu));
                            break;
                        }
                    }
                } else {
                    write(new_socket, "Invalid Admin Credentials\n", 26);
                    write(new_socket, menu, strlen(menu));
                }
                break;
            }
            default:
                write(new_socket, "Invalid option. Please select again\n", 37);
                bzero(new_socket, BUFFER_SIZE);
                write(new_socket, menu, strlen(menu));
                break;
        }
    }

    if (read_size == 0) {
        printf("Client disconnected\n");
    } else if (read_size == -1) {
        perror("recv failed");
    }

    close(new_socket);
    free(socket_desc);
    return NULL;
}

int main() {
    int server_fd, new_socket, *new_sock;
    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Could not create socket");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        return 1;
    }

    listen(server_fd, 3);

    printf("Waiting for incoming connections...\n");
    while ((new_socket = accept(server_fd, (struct sockaddr *)&client, &client_len))) {
        printf("Connection accepted\n");

        pthread_t client_thread;
        new_sock = malloc(1);
        *new_sock = new_socket;

        if (pthread_create(&client_thread, NULL, handle_client, (void *)new_sock) < 0) {
            perror("Could not create thread");
            return 1;
        }

        printf("Handler assigned\n");
    }

    if (new_socket < 0) {
        perror("Accept failed");
        return 1;
    }

    close(server_fd);
    return 0;
}