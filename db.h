//
// Created by deepanshpandey on 10/8/24.
//

// db.h
// db.h

#ifndef DB_H
#define DB_H

// Define a struct for Customer
typedef struct {
    int id;
    char name[50];
    char email[50];
    char phone[15];
    char password[50];
    double balance;
    int account_active;
    int login_status;
} Customer;

// Define a struct for Employee
typedef struct {
    int id;
    char name[50];
    char email[50];
    char password[50];
    int is_manager;
    int login_status;
} Employee;

// Define a struct for Admin
typedef struct {
    char name[50];
    char email[50];
    char password[50];
    int login_status;
} Admin;

// Define a struct for Transaction
typedef struct {
    int transaction_id;
    int account_id;
    double amount;
    char type[15]; // "deposit", "withdrawal", "transfer", "failed"
    char date[20];
} Transaction;

// Define a struct for LoanApplication
typedef struct {
    int loan_id;
    int customer_id;
    double amount;
    float interest_rate;
    int dur_year; // in years only
    int dur_month;// in months only
    double remaining_amount;
    char approved_date[20];
    char status[20]; // "pending", "approved", "rejected"
    int assigned_employee_id;
} LoanApplication;

// Define a struct for Feedback
typedef struct {
    int feedback_id;
    int customer_id;
    char feedback[255];
    char date[20];
} Feedback;
// Define a struct for other data
typedef struct {
    int highest_customer_id;
    int highest_employee_id;
    int highest_admin_id;
    int highest_transaction_id;
    int highest_loan_id;
    int highest_feedback_id;
} OTData;
#endif // DB_H