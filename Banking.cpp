#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>
#include <stdexcept>
using namespace std;

#define MIN_BALANCE 500

class InsufficientFunds {};

class Account
{
private:
    long accountNumber;
    string firstName;
    string lastName;
    float balance;
    static long NextAccountNumber;

public:
    Account() : accountNumber(0), balance(0.0f) {}
    Account(string fname, string lname, float balance);

    long getAccNo() const { return accountNumber; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    float getBalance() const { return balance; }

    void Deposit(float amount);
    void Withdraw(float amount);

    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();

    friend ofstream &operator<<(ofstream &ofs, Account &acc);
    friend ifstream &operator>>(ifstream &ifs, Account &acc);
    friend ostream &operator<<(ostream &os, Account &acc);
};

long Account::NextAccountNumber = 0;

class Bank
{
private:
    map<long, Account> accounts;
    void SaveToFile();

public:
    Bank();
    Account OpenAccount(string fname, string lname, float balance);
    Account BalanceEnquiry(long accountNumber);
    Account Deposit(long accountNumber, float amount);
    Account Withdraw(long accountNumber, float amount);
    void CloseAccount(long accountNumber);
    void ShowAllAccounts();
    ~Bank();
};

int main()
{
    Bank b;
    Account acc;

    int choice = 0;
    string fname, lname;
    long accountNumber = 0;
    float balance = 0.0f;
    float amount = 0.0f;

    cout << "*** Banking System ***" << endl;

    do
    {
        cout << "\n\tSelect one option below";
        cout << "\n\t1. Open an Account";
        cout << "\n\t2. Balance Enquiry";
        cout << "\n\t3. Deposit";
        cout << "\n\t4. Withdrawal";
        cout << "\n\t5. Close an Account";
        cout << "\n\t6. Show All Accounts";
        cout << "\n\t7. Quit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        try
        {
            switch (choice)
            {
            case 1:
                cout << "Enter First Name: ";
                cin >> fname;
                cout << "Enter Last Name: ";
                cin >> lname;
                cout << "Enter Initial Balance: ";
                cin >> balance;
                acc = b.OpenAccount(fname, lname, balance);
                cout << endl << "Congratulations! Account is created." << endl;
                cout << acc;
                break;

            case 2:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                acc = b.BalanceEnquiry(accountNumber);
                cout << endl << "Your Account Details" << endl;
                cout << acc;
                break;

            case 3:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                cout << "Enter Amount: ";
                cin >> amount;
                acc = b.Deposit(accountNumber, amount);
                cout << endl << "Amount is Deposited" << endl;
                cout << acc;
                break;

            case 4:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                cout << "Enter Amount: ";
                cin >> amount;
                acc = b.Withdraw(accountNumber, amount);
                cout << endl << "Amount Withdrawn" << endl;
                cout << acc;
                break;

            case 5:
                cout << "Enter Account Number: ";
                cin >> accountNumber;
                b.CloseAccount(accountNumber);
                cout << endl << "Account is Closed" << endl;
                break;

            case 6:
                b.ShowAllAccounts();
                break;

            case 7:
                break;

            default:
                cout << "\nPlease enter a correct choice." << endl;
            }
        }
        catch (const InsufficientFunds &)
        {
            cout << "Insufficient funds for this withdrawal." << endl;
        }
        catch (const exception &ex)
        {
            cout << ex.what() << endl;
        }
    } while (choice != 7);

    return 0;
}

Account::Account(string fname, string lname, float balance)
{
    NextAccountNumber++;
    accountNumber = NextAccountNumber;
    firstName = fname;
    lastName = lname;
    this->balance = balance;
}

void Account::Deposit(float amount)
{
    balance += amount;
}

void Account::Withdraw(float amount)
{
    if (balance - amount < MIN_BALANCE)
        throw InsufficientFunds();
    balance -= amount;
}

void Account::setLastAccountNumber(long accountNumber)
{
    NextAccountNumber = accountNumber;
}

long Account::getLastAccountNumber()
{
    return NextAccountNumber;
}

ofstream &operator<<(ofstream &ofs, Account &acc)
{
    ofs << acc.accountNumber << endl;
    ofs << acc.firstName << endl;
    ofs << acc.lastName << endl;
    ofs << acc.balance << endl;
    return ofs;
}

ifstream &operator>>(ifstream &ifs, Account &acc)
{
    ifs >> acc.accountNumber;
    ifs >> acc.firstName;
    ifs >> acc.lastName;
    ifs >> acc.balance;
    return ifs;
}

ostream &operator<<(ostream &os, Account &acc)
{
    os << "First Name: " << acc.getFirstName() << endl;
    os << "Last Name: " << acc.getLastName() << endl;
    os << "Account Number: " << acc.getAccNo() << endl;
    os << "Balance: " << acc.getBalance() << endl;
    return os;
}

Bank::Bank()
{
    ifstream infile("Bank.data");
    if (!infile)
        return;

    Account account;
    while (infile >> account)
    {
        accounts[account.getAccNo()] = account;
        Account::setLastAccountNumber(account.getAccNo());
    }

    infile.close();
}

void Bank::SaveToFile()
{
    ofstream outfile("Bank.data", ios::trunc);
    if (!outfile)
        throw runtime_error("Unable to open Bank.data for writing.");

    for (map<long, Account>::iterator itr = accounts.begin(); itr != accounts.end(); ++itr)
    {
        outfile << itr->second;
    }
    outfile.close();
}

Account Bank::OpenAccount(string fname, string lname, float balance)
{
    Account account(fname, lname, balance);
    accounts[account.getAccNo()] = account;
    SaveToFile();
    return account;
}

Account Bank::BalanceEnquiry(long accountNumber)
{
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    if (itr == accounts.end())
        throw runtime_error("Account does not exist.");
    return itr->second;
}

Account Bank::Deposit(long accountNumber, float amount)
{
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    if (itr == accounts.end())
        throw runtime_error("Account does not exist.");
    itr->second.Deposit(amount);
    accounts[accountNumber] = itr->second;
    SaveToFile();
    return itr->second;
}

Account Bank::Withdraw(long accountNumber, float amount)
{
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    if (itr == accounts.end())
        throw runtime_error("Account does not exist.");
    itr->second.Withdraw(amount);
    accounts[accountNumber] = itr->second;
    SaveToFile();
    return itr->second;
}

void Bank::CloseAccount(long accountNumber)
{
    map<long, Account>::iterator itr = accounts.find(accountNumber);
    if (itr == accounts.end())
        throw runtime_error("Account does not exist.");

    accounts.erase(itr);
    SaveToFile();
}

void Bank::ShowAllAccounts()
{
    if (accounts.empty())
    {
        cout << "No accounts found." << endl;
        return;
    }

    for (map<long, Account>::iterator itr = accounts.begin(); itr != accounts.end(); ++itr)
    {
        cout << "Account " << itr->first << endl << itr->second << endl;
    }
}

Bank::~Bank()
{
    try
    {
        SaveToFile();
    }
    catch (const exception &)
    {
    }
}