# Banking Management System in C

This is a simple banking management system that allows users to create an account, deposit money, withdraw money, check balance, and view transaction history. The system is implemented using C only with no external libraries at all.
This project is a part of the C programming course at the Faculty of Software Systems by Professor B. Thangaraju at IIIT Bangalore.
## Features
- 4 roles - Admin, Manager, Customer, and Employee
- Automatic ID generation
- Create an account
- Deposit money
- Withdraw money
- Check balance
- View transaction history
- Transaction tagging
- Password protected
- User-friendly interface
- Easy to use
- No external libraries
- Loan system
- Interest calculation
- User Session Limits
- Account locking/Unlocking
- etc.
## To-Do
- Improve the code
- Add Record Locking
- Add support for multiple Clients
## How to run
this project has been tested on Ubuntu 20.04 to 24.10 and should work on any Linux distribution.
run the following commands in a terminal
```
gcc server.c -o server && ./server
```
now in another terminal run the following commands
```
gcc client.c -o client && ./client 
```
### Note
- If you are using a different operating system, you may need to change the code a little bit to make it work.
- If you found any bugs or issues, please report them.
- An optional OCR (One Click Run) script has been added. This will compile and run the server and client files automatically. You can compile and run the script by running the following command
```
gcc ocr.c -o ocr && ./ocr
```
