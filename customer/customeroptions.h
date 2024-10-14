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
            write(new_socket, customer_menu, strlen(customer_menu));
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
                    logout = true;
                    write(new_socket,customer_menu,strlen(customer_menu));
                    break;
                }
                default:{
                    write(new_socket, "Invalid option. Please select again\n", 37);
                    write(new_socket, customer_menu, strlen(customer_menu));
                    break;
                }
            }
        }
    }while (logout == false);
}

#endif //CUSTOMEROPTIONS_H
