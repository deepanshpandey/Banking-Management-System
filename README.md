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
- Multi-user support (via multi-threading)
- User Session Limits
- Account locking/Unlocking
- etc.
## To-Do
- Improve the code
- Add Record Locking
## How to run
this project has been tested on Ubuntu 20.04 to 24.10 and should work on any Linux distribution.
run the following commands in a terminal in the project directory.
- To setup Database files and directories, run the following command.
```
gcc adminCreator.c -o adminCreator && ./adminCreator
```
- To compile and run the server, run the following commands
```
gcc server.c -o server && ./server
```
- To compile and run the Client, Now in another terminal run the following commands
```
gcc client.c -o client && ./client 
```
- Client supports multithreading and can be run multiple times to simulate multiple users.
- To clear login data, run the adminCreator.c Program and follow the on-screen instructions.

### Note
- If you are using a Operating System other than Ubuntu or Linux (say Windows or MacOS) , you may need to change the code a little bit to make it work.
- If you found any bugs or issues, please report them.
- An optional OCR (One Click Run) script has been added. This will compile and run the server and client files automatically, but you will need to run client again if you wish to have multiple clients. You can compile and run the script by running the following command
```
gcc ocr.c -o ocr && ./ocr
```
- The default username and password are `admin` and `admin` respectively.
- to terminate a client instance type `exit` and press enter.
- to create a new admin account or clear the databases, you can use the adminCreator.c Program.
- To Run the adminCreator.c Program, use the following commands
```
gcc adminCreator.c -o ac && ./ac
```
- This program is made using JetBrains CLion and uses Cmake.
- The adminCreator.c is a simple program that can be used to create a new admin account or clear the databases and logins.
