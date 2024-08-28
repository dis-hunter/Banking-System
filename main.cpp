#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

// Function prototypes
void mainmenu();
void submenu();
void usermenu();
void adminmenu();
bool fileExists(const string& filename);
bool usernameExists(const string& username);
bool adminExists(const string& username);
double accountBalance(const string& username);
void updateAccount(const string& username, const string& password, double newBalance);
void withdrawCash(const string& username);
void depositCash(const string& username);
void resetAdminPassword(const string& adminUsername);
void resetUserPassword(const string& username);
void handleMainMenuOptions(int option);
void handleSubMenu(int option);
void adminDeposit(const string& username);
void handleUserMenuOption(int option, const string& username);
void handleAdminMenuOption(int option, const string& username);
void userlogin();
void adminlogin();
void clientregistration();

// Check if a file exists
bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

// Submenu
void submenu() {
    cout << "1. Normal User Login\n";
    cout << "2. Administrative User Login\n";
}

// Main menu
void mainmenu() {
    cout << "1. Login\n";
    cout << "2. Exit\n\n";
}

// Normal user menu
void usermenu() {
    cout << "1. Withdraw Cash\n";
    cout << "2. Check Balance\n";
    cout << "3. Reset Your Password\n";
    cout << "4. Log out\n";
}

// Administrator menu
void adminmenu() {
    cout << "1. Deposit Money for Client\n";
    cout << "2. Register New Client\n";
    cout << "3. Reset Your Password\n";
    cout << "4. Log out\n";
}

// Validate if a username exists
bool usernameExists(const string& username) {
    ifstream file("UserDB.txt");
    string line;
    while (getline(file, line)) {
        if (line.find(username + " ") == 0) {
            return true;
        }
    }
    return false;
}
//validate if admin exists
bool adminExists(const string& username) {
    ifstream file("admin.txt");
    string line;
    while (getline(file, line)) {
        if (line.find(username + " ") == 0) {
            return true;
        }
    }
    return false;
}

// Function to retrieve account balance
double accountBalance(const string& username) {
    ifstream file("UserDB.txt");
    string line;
    while (getline(file, line)) {
        if (line.find(username + " ") == 0) {
            size_t pos = line.find_last_of(" ");
            return stod(line.substr(pos + 1));
        }
    }
    return 0.0;
}

// Function to update account balance for a given user
void updateAccount(const string& username, const string& password, double newBalance) {
    ifstream file("UserDB.txt");
    ofstream temp("Temp.txt");

    string line;
    while (getline(file, line)) {
        if (line.find(username + " ") == 0) {
            temp << username << " " << password << " " << newBalance << endl;
        }
        else {
            temp << line << endl;
        }
    }

    file.close();
    temp.close();

    remove("UserDB.txt");
    rename("Temp.txt", "UserDB.txt");
}

// Function to withdraw cash
void withdrawCash(const string& username) {
    ifstream inputFile("UserDB.txt");
    ofstream tempFile("temp.txt");
    string line;
    bool userFound = false;
    cout << "Enter the amount you want to withdraw: ";
    double amount;
    cin >> amount;
    while (getline(inputFile, line)) {
        if (line.find(username + " ") == 0) {
            userFound = true;
            size_t pos = line.find_last_of(" ");
            double balance = stod(line.substr(pos + 1));

            if (balance >= amount) {
                balance -= amount;
                tempFile << line.substr(0, pos) << " " << balance << endl;
                cout << "Withdrawal successful. New balance: " << balance << endl;
            }
            else {
                cout << "Insufficient balance. Withdrawal canceled." << endl;
                tempFile << line << endl;
            }
        }
        else {
            tempFile << line << endl;
        }
    }

    if (!userFound) {
        cout << "User not found. Withdrawal canceled." << endl;
    }

    inputFile.close();
    tempFile.close();

    remove("UserDB.txt");
    rename("temp.txt", "UserDB.txt");
}

// Function to deposit money into a customer's account
void adminDeposit() {
    string customerAccount;
    double depositAmount;

    // Display a list of accounts to choose from
    ifstream inputFile("UserDB.txt");
    string line;
    cout << "List of customer accounts:\n";
    while (getline(inputFile, line)) {
        istringstream iss(line);
        string username, password;
        double balance;
        if (iss >> username >> password >> balance) {
            cout << username << endl;
        }
    }
    inputFile.close();

    // Prompt for the customer's account and deposit amount
    cout << "Enter the customer's account: ";
    cin >> customerAccount;
    cout << "Enter the amount to deposit: ";
    cin >> depositAmount;

    // Update the customer's account balance
    ifstream inputFile2("UserDB.txt");
    ofstream tempFile("Temp.txt");
    while (getline(inputFile2, line)) {
        istringstream iss(line);
        string username, password;
        double balance;
        if (iss >> username >> password >> balance) {
            if (username == customerAccount) {
                balance += depositAmount;
            }
            tempFile << username << " " << password << " " << balance << endl;
        }
    }
    inputFile2.close();
    tempFile.close();

    // Replace the original file with the updated file
    remove("UserDB.txt");
    rename("Temp.txt", "UserDB.txt");

    cout << "Deposit of $" << depositAmount << " into account " << customerAccount << " successful!\n";
}


// Function to reset user password
void resetUserPassword(const string& username) {
    string newPassword;

    cout << "Enter new password: ";
    cin >> newPassword;

    ifstream inputFile("UserDB.txt");
    ofstream tempFile("Temp.txt");
    string line;

    while (getline(inputFile, line)) {
        istringstream iss(line);
        string storedUsername, storedPassword;
        double balance;

        if (iss >> storedUsername >> storedPassword >> balance) {
            if (storedUsername == username) {
                tempFile << storedUsername << " " << newPassword << " " << balance << endl;
                cout << "Password reset successfully!\n";
            } else {
                tempFile << line << endl;
            }
        }
    }

    inputFile.close();
    tempFile.close();

    remove("UserDB.txt");
    rename("Temp.txt", "UserDB.txt");
}

// Function to reset admin password
void resetAdminPassword(const string& adminUsername) {
    string newPassword;

    cout << "Enter new password: ";
    cin >> newPassword;

    ifstream inputFile("admin.txt");
    ofstream tempFile("Temp.txt");
    string line;

    while (getline(inputFile, line)) {
        istringstream iss(line);
        string storedUsername, storedPassword;

        if (iss >> storedUsername >> storedPassword) {
            if (storedUsername == adminUsername) {
                tempFile << storedUsername << " " << newPassword << endl;
                cout << "Password reset successfully!\n";
            } else {
                tempFile << line << endl;
            }
        }
    }

    inputFile.close();
    tempFile.close();

    remove("admin.txt");
    rename("Temp.txt", "admin.txt");
}

// Rest of the code...

// Function to handle main menu options
void handleMainMenuOptions(int option) {
    while (option != 1 && option != 2) {
        cout << "Invalid option. Please try again: ";
        cin >> option;
    }

    switch (option) {
    case 1:
        submenu();
        break;
    case 2:
        cout << "Exiting...\n";
        break;
    }
}

// Function to handle submenu
void handleSubMenu(int option) {
    while (option != 1 && option != 2) {
        cout << "Invalid option. Please try again: ";
        cin >> option;
    }

    switch (option) {
    case 1:
        userlogin();
        break;
    case 2:
        adminlogin();
        break;
    }
}

// Function to handle user menu options
void handleUserMenuOption(int option, const string& username) {
    switch (option) {
    case 1:
        withdrawCash(username);
        break;
    case 2:
        cout << "Account Balance: " << accountBalance(username) << endl;
        break;
    case 3:
        resetUserPassword(username);
        break;
    case 4:
        cout << "Logging out...\n";
        mainmenu();
        break;
    default:
        cout << "Invalid option. Please try again.\n";
        break;
    }
}

// Function to handle the administrator menu options
void handleAdminMenuOption(int option, const string& username) {
    switch (option) {
    case 1:
        adminDeposit();
        break;
    case 2:
        clientregistration();
        break;
    case 3:
        resetAdminPassword(username);
        break;
    case 4:
        cout << "Logging out...\n";
        mainmenu();
        break;
    default:
        cout << "Invalid option. Please try again.\n";
        break;
    }
}

// Function to log in as a user
// Function to log in as a user
// Function to log in as a user
void userlogin() {
    string username, password;
    bool loggedin = false;
    int attempts = 3;

    while (!loggedin && attempts > 0) {
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        if (usernameExists(username)) {
            ifstream file("UserDB.txt");
            string line;

            while (getline(file, line)) {
                istringstream iss(line);
                string storedUsername, storedPassword;
                double balance;

                if (iss >> storedUsername >> storedPassword >> balance) {
                    if (storedUsername == username && storedPassword == password) {
                        loggedin = true;
                        cout << "Successfully logged in\n";
                        break;
                    }
                }
            }

            file.close();
        }
        else {
            cout << "Invalid username\n";
            attempts--;
            cout << attempts << " attempts remaining\n";
        }

        if (!loggedin && attempts == 0) {
            cout << "No more attempts remaining\n";
        }

        if (loggedin) {
            usermenu();
            int userOption;
            cout << "Enter an option to continue: ";
            cin >> userOption;
            handleUserMenuOption(userOption, username);
        }
    }
}



// Function to log in as an administrator
void adminlogin() {
    string username, password;
    int attempts = 3;
    bool loggedin = false;
    while (!loggedin && attempts > 0) {
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;
        if (adminExists(username)) {
            ifstream file;
            file.open("admin.txt");
            string line;
            while (getline(file, line)) {
                istringstream iss(line);
                string adminStoredUsername, adminStoredPassword;
                double balance;
                if (iss >> adminStoredUsername >> adminStoredPassword ) {
                    if (username == adminStoredUsername && password == adminStoredPassword) {
                        loggedin = true;
                        cout << "Successfully logged in\n";
                    }
                    else {
                        cout << "Incorrect password please try again\n";
                        attempts--;
                        cout << attempts << " attempts remaining\n";
                    }

                }
            }file.close();

        }
        else {
            cout << "Invalid username please try again\n";
            attempts--;
            cout << attempts << " attempts remaining\n";
        }
    }if (!loggedin && attempts == 0) {
        cout << "Log in failed\nNo more attempts remaining\n";
    }
    if (loggedin) {
        adminmenu();
        int adminOption;
        cout << "Enter an option to continue: ";
        cin >> adminOption;
        handleAdminMenuOption(adminOption, username);
    }
}



// Function to register a new client by an administrator
void clientregistration() {
    string username, password;
    bool registered = false;

    while (!registered) {
        cout << "Enter username: ";
        cin >> username;

        if (usernameExists(username)) {
            cout << "Username already exists\n";
        }
        else {
            cout << "Enter password: ";
            cin >> password;

            double balance;
            cout << "Enter initial balance: ";
            cin >> balance;

            ofstream file("UserDB.txt", ios::app);
            file << username << " " << password << " " << balance << endl;
            file.close();

            cout << "Client registered successfully\n";
            registered = true;
        }
    }
}

int main() {
    cout << "***** WELCOME TO E-CASH SERVICES *****\n";
    int mainOption;

    do {
        cout << "Choose an option to proceed\n\n";
        mainmenu();
        cin >> mainOption;

        if (mainOption == 1) {
            int subOption;
            cout << "Enter an option to proceed\n\n";
            submenu();
            cin >> subOption;

            if (subOption == 1) {
                userlogin();
            }
            else if (subOption == 2) {
                adminlogin();
            }
            else {
                cout << "Invalid option. Please try again.\n";
            }
        }
    } while (mainOption != 2);

    return 0;
}