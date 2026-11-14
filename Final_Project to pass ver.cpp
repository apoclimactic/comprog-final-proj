//J.U.L.S. Banking System
// by Julian Gabriel Cuyugan, Juan Carlo Tumbaga, Lebron Aquino, Simoune Rein Quitay

#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cstdlib>
using namespace std;

// global account data
bool hasAccount = false;
string firstName, middleName, lastName, address;
int birthMonth, birthDay, birthYear;
char gender;
int accountType;            // 1 = savings and 2 = current
long accountNumber;
int pin;
double initialDeposit, balance;
// function prototypes
void displayHeader();
void displayMenu();
void openAccount();
void checkPIN();
void balanceInquiry();
void deposit();
void withdraw();
void viewAccountInfo();
void exitScreen();

int main() {
    // system("color 0A");   // optional terminal color
    srand(time(0)); // seed random generator for account number 
    cout << fixed << setprecision(2);   // format currency to 2 decimal places
    

    int choice;
    do {
        displayHeader();
        displayMenu();
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: openAccount(); break;
            case 2: case 3: case 4: case 5: // block access if no account exists, otherwise verify PIN first.
                if (!hasAccount) {
                    cout << "No account found. Please open one first.\n";
                } else {
                    checkPIN();
                    if (choice == 2) balanceInquiry();
                    else if (choice == 3) deposit();
                    else if (choice == 4) withdraw();
                    else viewAccountInfo();
                }
                break;
            case 6: exitScreen(); break;
            default: cout << "Invalid choice.\n";
        }
        cout << "\n";
    } while (choice != 6);
    return 0;
}

void displayHeader() {
    cout << "==============================\n";
    cout << "    J.U.L.S. BANKING SYSTEM\n";
    cout << "==============================\n";
}

void displayMenu() {
    cout << "[1] Open a New Account\n"
         << "[2] Balance Inquiry\n"
         << "[3] Deposit\n"
         << "[4] Withdraw\n"
         << "[5] View Account Information\n"
         << "[6] Exit Program\n";
}

void checkPIN() {
    int entered;
    do {
        cout << "Enter PIN: ";
        cin >> entered;
        if (entered != pin) cout << "Wrong PIN. Try again.\n";
    } while (entered != pin);
}

void openAccount() {
    displayHeader();
    if (hasAccount) {
        cout << "An account already exists.\n";
        return;
    }

    const double SA_MIN_INITIAL = 5000, CA_MIN_INITIAL = 10000;

    cout << "First name: ";   // first getline clears leftover newline from menu cin, allowing for proper input of first name with spaces. (mainly used for people with second names)
    getline(cin, firstName);
    getline(cin, firstName);
    cout << "Middle name: ";  
    getline(cin, middleName);
    cout << "Last name: ";   
    getline(cin, lastName);

    // address
    cout << "Address: ";
    getline(cin, address);
   

    // birthday and age check
    time_t now = time(0); // get current system date for age calculation.
    tm *t = localtime(&now);
    int curYear = t->tm_year + 1900;
    int curMonth = t->tm_mon + 1;
    int curDay = t->tm_mday;
    int age;
    int maxDay;
    bool invalidDate;
    


    do {
    cout << "Birth month (1-12): ";
    cin >> birthMonth;
    cout << "Birth day: ";
    cin >> birthDay;
    cout << "Birth year: ";
    cin >> birthYear;

    // invalid date check, by checking if the amount of days in the month is valid for the given year (leap year check for February). also checks if the month and year indicate user is over 18 years old.
    invalidDate = false;

    if (birthMonth < 1 || birthMonth > 12 || birthYear < 1900 || birthYear > curYear) {
        invalidDate = true;
    } else {
        switch (birthMonth) {
            case 4: case 6: case 9: case 11:
                maxDay = 30;
                break;
            case 2:
                if ((birthYear % 4 == 0 && birthYear % 100 != 0) || birthYear % 400 == 0)
                    maxDay = 29;
                else
                    maxDay = 28;
                break;
            default:
                maxDay = 31;
        }
        if (birthDay < 1 || birthDay > maxDay) {
            invalidDate = true;
        }
    }
    // rejects invalid date or age less than 18
    if (invalidDate) {
        cout << "Invalid birthday. Try again.\n";
    } else {
        age = curYear - birthYear;
        // subtract 1 year if birthday has not occurred yet this year.
        if (curMonth < birthMonth || 
            (curMonth == birthMonth && curDay < birthDay)) {
            age--;
        }

        if (age < 18) {
            cout << "Your age is not yet allowed to open an account.\n";
        }
    }
} while (invalidDate || age < 18);

    // gender male or female only
    do {
        cout << "Gender (M/F): ";
        cin >> gender;
        if (gender == 'm') gender = 'M';
        if (gender == 'f') gender = 'F';
        if (gender != 'M' && gender != 'F')
            cout << "Invalid. Enter M or F only.\n";
    } while (gender != 'M' && gender != 'F');

    // account type
    do {
        cout << "Account type [1] Savings  [2] Current: ";
        cin >> accountType;
        if (accountType != 1 && accountType != 2)
            cout << "Invalid choice.\n";
    } while (accountType != 1 && accountType != 2);

    // initial deposit
    double minimum = (accountType == 1) ? SA_MIN_INITIAL : CA_MIN_INITIAL;
    do {
        cout << "Initial deposit (minimum " << minimum << "): ";
        cin >> initialDeposit;
        if (initialDeposit < minimum)
            cout << "Initial deposit is not sufficient.\n";
    } while (initialDeposit < minimum);

    // pin maker
    do {
        cout << "Set 6-digit PIN (must not begin with 0): ";
        cin >> pin;
        if (pin < 100000 || pin > 999999)
            cout << "PIN must be exactly 6 digits.\n";
    } while (pin < 100000 || pin > 999999);

    // account number generation (8 digits)
    accountNumber = (long)(rand() % 9000 + 1000) * 10000 + rand() % 10000; // generate random 8-digit account number (10000000 to 99999999).
    balance = initialDeposit;
    hasAccount = true;

    cout << "\nAccount created! Your account number is " << accountNumber << "\n";
}

// balance inquiry function
void balanceInquiry() {
    displayHeader();
    cout << "Current balance: " << balance << "\n";
}

// deposit function, minimum deposit depends on account type
void deposit() {
    displayHeader();
    double minimum = (accountType == 1) ? 300 : 500;
    double amount;
    char confirm;

    do {
        cout << "Enter deposit amount (minimum " << minimum << ", or 0 to cancel): ";
        cin >> amount;
        if (amount == 0) {
            cout << "Transaction cancelled.\n";
            return;
        }
        if (amount < minimum)
            cout << "The entered amount is not valid.\n";
    } while (amount < minimum);

    cout << "Confirm deposit of " << amount << "? (Y/N): ";
    cin >> confirm;
    if (confirm == 'Y' || confirm == 'y') {
        balance += amount;
        cout << "Deposit successful! New balance: " << balance << "\n";
    } else {
        cout << "Transaction cancelled.\n";
    }
}

// withdraw function, minimum withdrawal depends on account type
void withdraw() {
    displayHeader();
    double minimum = (accountType == 1) ? 300 : 500;
    double amount;
    char confirm;

    do {
        cout << "Current balance: " << balance << "\n";
        cout << "Enter withdrawal amount (minimum " << minimum << ", or 0 to cancel): ";
        cin >> amount;
        if (amount == 0) {
            cout << "Transaction cancelled.\n";
            return;
        }
        if (amount < minimum || amount > balance)
            cout << "The entered amount is not valid.\n";
    } while (amount < minimum || amount > balance);

    cout << "Confirm withdrawal of " << amount << "? (Y/N): ";
    cin >> confirm;
    if (confirm == 'Y' || confirm == 'y') {
        balance -= amount;
        cout << "Withdrawal successful! New balance: " << balance << "\n";
    } else {
        cout << "Transaction cancelled.\n";
    }
}

// view account information function
void viewAccountInfo() {
    displayHeader();
    cout << "Account Number : " << accountNumber << "\n";
    cout << "Full Name      : " << firstName << " " << middleName << " " << lastName << "\n";
    cout << "Address        : " << address << "\n";
    cout << "Birthday       : " << birthMonth << "/" << birthDay << "/" << birthYear << "\n";
    cout << "Gender         : " << gender << "\n";
    cout << "Account Type   : ";
    if (accountType == 1)
        cout << "Savings Account\n";
    else
        cout << "Current Account\n";
    cout << "Initial Deposit: " << initialDeposit << "\n";
    cout << "Current Balance: " << balance << "\n";
}

// exit screen function
void exitScreen() {
    displayHeader();
    cout << "Thank you for using J.U.L.S. Bank!\n";
    cout << "Members: Simoune Rein Quitay, Juan Carlo Tumbaga,\n";
    cout << "         Lebron Aquino, Julian Gabriel Cuyugan\n";
}
