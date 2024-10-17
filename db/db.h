//
// Created by deepanshpandey on 10/8/24.
//

// db.h
// db.h

#ifndef DB_H
#define DB_H

// Define a struct for Customer
typedef struct {
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
} DateTime;


typedef struct {
    int months;
    int years;
} MonthYear;;

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
    int to_account_id;
    double amount;
    char type[20]; // "deposit", "withdrawal", "transfer", "failed", "loan"
    DateTime date;
} Transaction;

// Define a struct for LoanApplication
typedef struct {
    int loan_id;
    int customer_id;
    char email[50]; // customer email
    double amount;
    float interest_rate;
    MonthYear month_year; // in years & months only
    double remaining_amount;
    DateTime application_date;
    DateTime approved_date;
    char status[20]; // "pending", "approved", "rejected"
    int assigned_employee_id;
} LoanApplication;

// Define a struct for Feedback
typedef struct {
    int feedback_id;
    char mail[50];
    char feedback[1000];
    DateTime date_time;
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