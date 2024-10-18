//
// Created by deepanshpandey on 10/16/24.
//

#ifndef TRANSTASKS_H
#define TRANSTASKS_H

#include <fcntl.h>
#include <string.h>
#include "../db/db.h"
#include "../datetime_ot/ottasks.h"
#include "../datetime_ot/datetimetasks.h"

void view_transactions(int account_id, int new_socket) {
    const char* file_path = "../db/transactions.db";
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }
    Transaction transaction;
    char buffer[1024];
    while (read(fd, &transaction, sizeof(transaction)) > 0) {
        if(transaction.account_id == account_id) {
            sprintf(buffer, "Transaction ID: %d\nAccount ID: %d\nTo Account ID: %d\nAmount: %f\nType: %s\nDate: %d/%d/%d %d:%d:%d\n\n", transaction.transaction_id, transaction.account_id, transaction.to_account_id, transaction.amount, transaction.type, transaction.date.day, transaction.date.month, transaction.date.year, transaction.date.hour, transaction.date.minute, transaction.date.second);
            write(new_socket, buffer, sizeof(buffer));
        }
    }
    close(fd);
}
int write_transaction(int account_id, int to_account_id, double amount) {
    const char* file_path = "../db/transactions.db";
    int fd = open(file_path,  O_APPEND | O_RDWR| O_CREAT , 0666);
    if (fd == -1) {
        perror("Failed to open file");
        return 0;
    }
    Transaction transaction;
    transaction.transaction_id = generate_transaction_id();
    transaction.account_id = account_id;
    transaction.to_account_id = to_account_id;
    transaction.amount = amount;
    strcpy(transaction.type, "Transfer");
    DateTime dt;
    initializeDateTime(&dt);
    transaction.date = dt;
    write(fd, &transaction, sizeof(transaction));
    close(fd);
    return 1;
}
void loan_transaction(int loan_id, int account_id, double amount) {
    const char* file_path = "../db/transactions.db";
    int fd = open(file_path,  O_APPEND | O_RDWR| O_CREAT , 0666);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }
    Transaction transaction;
    transaction.transaction_id = generate_transaction_id();
    transaction.account_id = loan_id;
    transaction.to_account_id = account_id;
    strcpy(transaction.type, "Loan");
    transaction.amount = amount;
    DateTime dt;
    initializeDateTime(&dt);
    transaction.date = dt;
    write(fd, &transaction, sizeof(transaction));
    close(fd);
}
void deposit_transaction(int account_id, double amount) {
    const char* file_path = "../db/transactions.db";
    int fd = open(file_path, O_APPEND | O_RDWR| O_CREAT , 0666);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }
    Transaction transaction;
    transaction.transaction_id = generate_transaction_id();
    transaction.account_id = account_id;
    transaction.amount = amount;
    DateTime dt;
    initializeDateTime(&dt);
    transaction.date = dt;
    transaction.to_account_id = account_id;
    strcpy(transaction.type, "Deposit");
}
void withdrawal_transaction(int account_id, double amount) {
    const char* file_path = "../db/transactions.db";
    int fd = open(file_path, O_APPEND | O_RDWR| O_CREAT , 0666);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }
    Transaction transaction;
    transaction.transaction_id = generate_transaction_id();
    transaction.account_id = account_id;
    transaction.amount = amount;
    DateTime dt;
    initializeDateTime(&dt);
    transaction.date = dt;
    transaction.to_account_id = 0;
    strcpy(transaction.type, "Withdrawal");
    close(fd);
}
void failed_transaction(int account_id, int to_account_id, double amount) {
    const char* file_path = "../db/transactions.db";
    int fd = open(file_path, O_APPEND | O_RDWR| O_CREAT , 0666);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }
    Transaction transaction;
    transaction.transaction_id = generate_transaction_id();
    transaction.account_id = account_id;
    transaction.to_account_id = to_account_id;
    transaction.amount = amount;
    DateTime dt;
    initializeDateTime(&dt);
    transaction.date = dt;
    strcpy(transaction.type, "Failed");
    close(fd);
}
#endif //TRANSTASKS_H
