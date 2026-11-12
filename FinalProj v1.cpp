#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cstdlib>
using namespace std;

// ---- Global account data (one account) ----
bool hasAccount = false;
string firstName, middleName, lastName, address;
int birthMonth, birthDay, birthYear;
char gender;
int accountType;            // 1 = Savings, 2 = Current
long accountNumber;
int pin;
double initialDeposit, balance;

// ---- Prototypes ----
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
    // system("color 0A");   // optional terminal color (Windows only)
    srand(time(0));
    cout << fixed << setprecision(2);   // show money as 5000.00

    int choice;
    do {
        displayHeader();
        displayMenu();
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: openAccount(); break;
            case 2: case 3: case 4: case 5:
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
    cout << "        J.U.L.S. BANK\n";
    cout << "    Joint User Ledger System\n";
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

    cout << "First name: ";   cin >> firstName;
    cout << "Middle name: ";  cin >> middleName;
    cout << "Last name: ";    cin >> lastName;

    // Address needs spaces, so we use getline.
    // cin >> leaves a '\n' in the buffer, so the first getline swallows it.
    // (Alternative: cin.ignore(1000, '\n'); then one getline. Ask if allowed.)
    cout << "Address: ";
    getline(cin, address);
    getline(cin, address);

    // Birthday and age check
    time_t now = time(0);
    tm *t = localtime(&now);
    int curYear = t->tm_year + 1900;
    int curMonth = t->tm_mon + 1;
    int curDay = t->tm_mday;
    int age;

bool invalidDate;

do {
    cout << "Birth month (1-12): ";
    cin >> birthMonth;
    cout << "Birth day (1-31): ";
    cin >> birthDay;
    cout << "Birth year: ";
    cin >> birthYear;

    invalidDate = birthMonth < 1 || birthMonth > 12 ||
                  birthDay < 1 || birthDay > 31 ||
                  birthYear > curYear || birthYear < 1900;

    if (invalidDate) {
        cout << "Invalid birthday. Try again.\n";
    } else {
        age = curYear - birthYear;

        if (curMonth < birthMonth ||
            (curMonth == birthMonth && curDay < birthDay)) {
            age--;
        }

        if (age < 18) {
            cout << "Your age is not yet allowed to open an account.\n";
        }
    }
} while (invalidDate || age < 18);

    // Gender (M or F only)
    do {
        cout << "Gender (M/F): ";
        cin >> gender;
        if (gender == 'm') gender = 'M';
        if (gender == 'f') gender = 'F';
        if (gender != 'M' && gender != 'F')
            cout << "Invalid. Enter M or F only.\n";
    } while (gender != 'M' && gender != 'F');

    // Account type
    do {
        cout << "Account type [1] Savings  [2] Current: ";
        cin >> accountType;
        if (accountType != 1 && accountType != 2)
            cout << "Invalid choice.\n";
    } while (accountType != 1 && accountType != 2);

    // Initial deposit
    double minimum = (accountType == 1) ? SA_MIN_INITIAL : CA_MIN_INITIAL;
    do {
        cout << "Initial deposit (minimum " << minimum << "): ";
        cin >> initialDeposit;
        if (initialDeposit < minimum)
            cout << "Initial deposit is not sufficient.\n";
    } while (initialDeposit < minimum);

    // PIN (6 digits)
    do {
        cout << "Set 6-digit PIN: ";
        cin >> pin;
        if (pin < 100000 || pin > 999999)
            cout << "PIN must be exactly 6 digits.\n";
    } while (pin < 100000 || pin > 999999);

    // Finish
    accountNumber = (long)(rand() % 9000 + 1000) * 10000 + rand() % 10000;
    balance = initialDeposit;
    hasAccount = true;

    cout << "\nAccount created! Your account number is " << accountNumber << "\n";
}

void balanceInquiry() {
    displayHeader();
    cout << "Current balance: " << balance << "\n";
}

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

void viewAccountInfo() {
    displayHeader();
    cout << "Account Number : " << accountNumber << "\n";
    cout << "Full Name      : " << firstName << " " << middleName << " " << lastName << "\n";
    cout << "Address        : " << address << "\n";
    cout << "Birthday       : " << birthMonth << "/" << birthDay << "/" << birthYear << "\n";
    cout << "Gender         : " << gender << "\n";
    cout << "Account Type   : " << ((accountType == 1) ? "Savings Account" : "Current Account") << "\n";
    cout << "Initial Deposit: " << initialDeposit << "\n";
    cout << "Current Balance: " << balance << "\n";
}

void exitScreen() {
    displayHeader();
    cout << "Thank you for using J.U.L.S. Bank!\n";
    cout << "Members: Simoune Rein Quitay, Juan Carlo Tumbaga,\n";
    cout << "         Lebron Aquino, Julian Gabriel Cuyugan\n";
}