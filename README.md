# Banking System

A simple console-based banking management system implemented in C++.

## Features
- Open a new bank account
- Check account balance
- Deposit money
- Withdraw money
- Close an account
- View all accounts
- Store account data in a file for persistence

## How to Run

### Prerequisites
- A C++ compiler such as g++
- Windows PowerShell or any terminal

### Compile
```bash
g++ Banking.cpp -o Banking.exe
```

### Run
```bash
./Banking.exe
```

## Example Usage
1. Select option 1 to open an account
2. Enter first name, last name, and initial balance
3. Use other options to view balance, deposit, withdraw, or close the account

## File Used
- Bank.data: stores account information between program runs

## Notes
- Minimum balance is set to 500
- Withdrawal attempts below the minimum balance will show an insufficient funds message
