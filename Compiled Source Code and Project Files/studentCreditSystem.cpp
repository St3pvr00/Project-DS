/* Features:

1. The system can do C.R.U.D. The user can top-up to their accounts.
    C-create: Account and Credit cash in
    R-Read: User info
    U-Update: User cradentials, Tansaction history
    D-Deelete: Cancel orders and delete receeipts of completed orders

2. There will be multiple user roles in the system such as student, scholar, athlete. At least 5 users must be pre-made in the system with each varying privileges and discounts.

3. There will be a transaction history per user to be displayed in the account.

4. The system is able to simulate 3 separate stalls with multiple items and prices each.

5. The system is implemented in a console-based output with a menu-style display. */

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <queue>
#include <chrono>
#include <ctime>
#include <cstdio>
using namespace std;

struct User {
    string name;
    string academicStatus;
    int idNumber;
    string pin;
    int credit = 0;
    //dagdagan ng pl, dl, scholar, varshity, normal student value
};


//Formatzzz
void centerText(const string& text);
void horizontalLine();
void creditHeaderName();
void rightLine(const string& text);

//First Part
void mainMenu();//oks na
    void createAccount(); //oks na
        void openUserFile(const User& user);//oks na
    bool accountLogin(); //oks na

//Second Part
void userDashboard(User loginUser);//oks na
    void transactionOptions(User loggedInUser);//oks na
        void cashIn(User& loggedInUser);//oks na
        void orderStores(User loggedInUser);//oks na
            void belgianWaffle(User& loggedInUser); //oks na
            void nescafe(User& loggedInUser);//oks na  
            void vmes(User& loggedInUser);//oks na     
    void transactionHistory();//oks na
    void accountInformation(User loggedInUser);//oks na
        void infoChange(User loggedInUser);//oks na
        
const int consoleWidth = 80;

vector<User> users;
queue<string> purchaseHistory;
int main()
{
    creditHeaderName();
    mainMenu();
    return 0;
}


void centerText(const string& text) {
    int padding = (consoleWidth - text.length()) / 2;
    cout << string(padding, ' ') << text << endl;
}

void horizontalLine() {
    cout << ' ' << string(consoleWidth - 2, '_') << ' ' << endl;
}

void rightLine(const string& text) {
    int padding = consoleWidth - text.length();
    cout << string(padding, ' ') << text << endl;
}

void creditHeaderName() {
    horizontalLine();
    centerText("Student Credit System");
    centerText("Canteen C-Cash-Cash");
    horizontalLine();
}

void createAccount() {
    User newStudent;
    // Prompts user to enter information
    cout << "Enter your Name: ";
    getline(cin >> ws, newStudent.name);

    cout << "Enter your ID number: ";
    cin >> newStudent.idNumber;

    cout << "Enter your PIN: ";
    cin >> newStudent.pin;

    newStudent.academicStatus = "Regular Student";

    ifstream inFile("user_info.txt");
    //checks if user input struct is exisiting in the user_info.txt
    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            istringstream ss(line);
            string token;
            
            User existingUser;
            if (getline(ss, token, ',')) {
                existingUser.name = token;
            }
            if (getline(ss, token, ',')) {
                existingUser.idNumber = stoi(token);
            }

            if (newStudent.name == existingUser.name) {
                cout << "Username or ID Number entered is already taken! Please choose a different one." << endl;
                inFile.close();
                creditHeaderName();
                mainMenu();
                return;
            }
            else if(newStudent.idNumber == existingUser.idNumber) {
                cout << "ID Number entered is already taken! This is distinct for every student in the university." << endl;
                inFile.close();
                creditHeaderName();
                mainMenu();
                return;
            }
        }
        inFile.close();
    } else {
        cout << "Error opening the file. Unable to check for duplicate accounts." << endl;
        creditHeaderName();
        mainMenu();
        return;
    }

    users.push_back(newStudent);
    //stores the user input struct as a new student to be appended to the user_info.txt
    ofstream outFile("user_info.txt", ios::app); 

    if (outFile.is_open()) {
        outFile << newStudent.name << "," << newStudent.idNumber << "," << newStudent.pin << "," << newStudent.credit << "," << newStudent.academicStatus << endl;
        outFile.close();

        cout << "User account created successfully!" << endl;
        creditHeaderName();
        mainMenu();
    } else {
        cout << "Error opening the file. Unable to save user information." << endl;
    }
}

void openUserFile(const User& user) {
    // Construct the filename based on the username of the logged in user
    string filename = user.name + ".txt";
    //creates a blank txt file named after the user to be used as individual transaction history 
    ofstream userFile(filename, ios::app);

    if (userFile.is_open()) {
        cout << "User file opened successfully.\n";
        userFile.close();
    } else {
        cout << "Error opening the user file for user: " << user.name << endl;
    }
}

bool accountLogin() {
    // Prompts user to enter information but this time it compares it to existing users in user_info.txt to log in the user
    User loginUser;
    cout << "Enter your Name: ";
    getline(cin >> ws, loginUser.name);

    cout << "Enter your ID number: ";
    cin >> loginUser.idNumber;

    cout << "Enter your PIN: ";
    cin >> loginUser.pin;

    ifstream inFile("user_info.txt");

    if (inFile.is_open()) {
        users.clear();

        string line;
        while (getline(inFile, line)) {
            istringstream ss(line);
            string token;

            User loadedUser;
            if (getline(ss, token, ',')) {
                loadedUser.name = token;
            }
            if (getline(ss, token, ',')) {
                loadedUser.idNumber = stoi(token);
            }
            if (getline(ss, token, ',')) {
                loadedUser.pin = token;
            }
            if (getline(ss, token, ',')) { 
                loadedUser.credit = stoi(token);
            }
            if (getline(ss, token, ',')) { 
                loadedUser.academicStatus = token;
            }

            users.push_back(loadedUser);
        }

        inFile.close();

        for (const auto& user : users) {
            if (loginUser.name == user.name && loginUser.idNumber == user.idNumber && loginUser.pin == user.pin) {
                cout << "Login successful. Welcome, " << user.name << "!" << endl;
                loginUser.academicStatus = user.academicStatus;  
                openUserFile(user);
                creditHeaderName();
                userDashboard(user); 
                return true;
            }
        }

        cout << "Login failed. Invalid credentials." << endl;
        creditHeaderName();
        mainMenu();
    } else {
        cout << "Error opening the file. Unable to perform login." << endl;
    }

    return false;
}

void transactionHistory(User loggedInUser) {
    string filename = loggedInUser.name + ".txt";
    ifstream userFile(filename);

    if (userFile.is_open()) {
        cout << "Transaction History for " << loggedInUser.name << ":" << endl;
        cout << "---------------------------------------------" << endl;

        string line;
        while (getline(userFile, line)) {
            cout << line << endl;
        }

        userFile.close();
    } else {
        cout << "Error opening the user file for user: " << loggedInUser.name << endl;
    }
    cout << "Press any key to go back to the main menu..." << endl;
    cin.ignore(); 
    cin.get();
    creditHeaderName();
    userDashboard(loggedInUser);
}

void infoChange(User loggedInUser) { 
    string newName;
    string newPin;
    centerText("Enter New Name: ");
    getline(cin >> ws, newName);
    centerText("Enter New Pin: ");
    cin >> newPin;

    // Find the index of the loggedInUser in the vector
    int index = -1;
    for (int i = 0; i < users.size(); ++i) {
        if (users[i].name == loggedInUser.name && users[i].idNumber == loggedInUser.idNumber) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        string oldFilename = loggedInUser.name + ".txt";
        // Update the loggedInUser data in the vector
        users[index].name = newName;
        users[index].pin = newPin;
        users[index].credit = loggedInUser.credit;
        // Rewrite the entire file with updated user information
        string newFilename = newName + ".txt";
        if (rename(oldFilename.c_str(), newFilename.c_str()) == 0) {
            cout << "User's .txt file renamed successfully!" << endl;
        } else {
            cout << "Error renaming user's .txt file." << endl;
        }
        ofstream outFile("user_info.txt");
        if (outFile.is_open()) {
            for (const auto& user : users) {
                
                outFile << user.name << "," << user.idNumber << "," << user.pin << "," << user.credit << "," << user.academicStatus << endl;
            }
            outFile.close();
            centerText("Account Change Complete");
            creditHeaderName();
            accountInformation(users[index]);
        } else {
            centerText("Error updating user information. Please try again later.");
            accountInformation(users[index]);
        }
    } else {
        centerText("User not found. Unable to update information.");
        accountInformation(loggedInUser);
    }
}

void accountInformation(User loggedInUser) {
    int choice;
    do {
        centerText("Name: " + loggedInUser.name);
        centerText("Student ID: " + to_string(loggedInUser.idNumber));
        centerText("Credit: " + to_string(loggedInUser.credit));
        centerText("Academic Standing: " + loggedInUser.academicStatus);
        centerText("1: Change account information");
        centerText("2: Go Back to Main Menu");
        cin >> choice;

        switch (choice) {
            case 1:
                creditHeaderName();
                cout << "Change Information" << endl;
                cout << "Note* Changing Information will not migrate Transaction History" << endl;
                infoChange(loggedInUser);
                break;
            case 2:
                cout << "Going back to the main menu..." << endl;
                creditHeaderName();
                userDashboard(loggedInUser);
                break;
            default:
                cout << "Invalid choice. Please choose 1 to go back to the main menu." << endl;
                break;
        }
    } while (choice != 1);
}

void cashIn(User& loggedInUser) {
    int cash;
    string choice;
    centerText("Input the amount of money you want to add to your account credits: ");
    cin >> cash;
    centerText("Do you want to proceed? (Y/n)");
    cin >> choice;

    if (choice == "Y" || choice == "y") {
        loggedInUser.credit += cash;
        
        // Update user_info.txt with the new credit value
        ofstream outFile("user_info.txt");
        if (outFile.is_open()) {
            for (const auto& user : users) {
                if (user.name == loggedInUser.name && user.idNumber == loggedInUser.idNumber) {
                    outFile << user.name << "," << user.idNumber << "," << user.pin << "," << loggedInUser.credit << "," << user.academicStatus << endl;
                } else {
                    outFile << user.name << "," << user.idNumber << "," << user.pin << "," << user.credit << "," << user.academicStatus << endl;
                }
            }
            outFile.close();
            centerText("Transaction complete!");
            creditHeaderName();
            transactionOptions(loggedInUser);
        } else {
            centerText("Error updating user information. Please try again later.");
            transactionOptions(loggedInUser);
        }

    } else {
        centerText("Cancelling transaction...");
        creditHeaderName();
        transactionOptions(loggedInUser);
    }
}

void belgianWaffle(User& loggedInUser) {
    int choice;
    int plain = 40;
    int chocolate = 40;
    int cheeseSpread = 55;
    int hazelnut = 65;
    int frenchButterMaple = 60;
    int cost = 0;
    string waffleType;
    do {
        centerText("Available Menu:");
        centerText("1: Plain (Php 40)");
        centerText("2: Chocolate (Php 55)");
        centerText("3: Cheese Spread (Php 55)");
        centerText("4: Hazelnut (Php 65)");
        centerText("5: French Butter Maple (Php 60)");
        centerText("6: Go Back");
        
        cin >> choice;

        switch (choice) {
            case 1:
                waffleType = "Plain";
                creditHeaderName();
                cout << waffleType << " Waffle has been added to your order" << endl;
                cost = plain;
                break;
            case 2:
                waffleType = "Chocolate";
                creditHeaderName();
                cout << waffleType << " Waffle has been added to your order " << endl;
                cost = chocolate;
                break;
            case 3:
                waffleType = "Cheese Spread";
                creditHeaderName();
                cout << waffleType << " Waffle has been added to your order" << endl;
                cost = cheeseSpread;
                break;
            case 4:
                waffleType = "Hazelnut";
                creditHeaderName();
                cout << waffleType << " Waffle has been added to your order" << endl;
                cost = hazelnut;
                break;
            case 5:
                waffleType = "French Butter Maple";
                creditHeaderName();
                cout << waffleType << " Waffle has been added to your order" << endl;
                cost = frenchButterMaple;
                break;
            case 6:
                creditHeaderName();
                transactionOptions(loggedInUser);
                break;   
            default:
                cout << "Invalid input, order cannot be added." << endl;
                break;
        }

        string choice2;
        cout << "You Want to Proceed With Order? (Y/n): "<<endl;
        cin >> choice2;
        if (choice2 == "y" || choice2 == "Y"){
        // Deduct the cost from user's credit
        if (loggedInUser.credit >= cost) {
        // Apply discount first
        double discountRate = 0.0;
        if (loggedInUser.academicStatus == "Dean's Lister") {
            discountRate = 0.2;
        } else if (loggedInUser.academicStatus == "President's Lister") {
            discountRate = 0.3;
        } else if (loggedInUser.academicStatus == "Athlete") {
            discountRate = 0.1;
        } else if (loggedInUser.academicStatus == "Scholar") {
            discountRate = 0.2;
        }

    		// Calculate discounted cost
        double discountedCost = cost - (cost * discountRate);

        cout << "Price: Php " << cost << ", Discount: " << (discountRate * 100) << "%, Discounted Cost: Php " << discountedCost << endl;
        
        auto currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());

        // Convert time to string
        string purchaseTime = ctime(&currentTime);
        // Display time and date
        cout << "Time of Purchase: " << purchaseTime;

        // Update user's credit
        loggedInUser.credit -= discountedCost;

            // Update the user information in the vector
            for (auto& user : users) {
                
                if (user.name == loggedInUser.name && user.idNumber == loggedInUser.idNumber) {
                user.credit = loggedInUser.credit;
                user.academicStatus = loggedInUser.academicStatus;  // Add this line
                break;
                }
            }
            // Rewrite the entire file with updated user information
            ofstream outFile("user_info.txt");
            if (outFile.is_open()) {
                for (const auto& user : users) {
                    outFile << user.name << "," << user.idNumber << "," << user.pin << "," << user.credit << "," << user.academicStatus << endl;
                }
                outFile.close();
                centerText("Transaction complete!");
            } else {
                centerText("Error updating user information. Please try again later.");
                transactionOptions(loggedInUser);
            }
                     string filename = loggedInUser.name + ".txt";
                    ofstream userFile(filename, ios::app);
                    if (userFile.is_open()) {
                        userFile << "------------------------------------------------------------------------------------------------------------"<<endl;
                        userFile << "Belgian Waffle: " << waffleType << ", Price: Php " << cost << ", Discount: " << (discountRate * 100) << "%, Discounted Cost: Php " << discountedCost << endl;
                        userFile << "Time of Purchase: " << purchaseTime;
                        userFile.close();
                    } else {
                        cout << "Error opening the user file for user: " << loggedInUser.name << endl;
                    }

            // Prompt user to cash out
            cout << "Press Y to Proceed and n to order another waffle (Y/n): ";
            string choice;  
            cin >> choice;

            if (choice != "Y" && choice != "y") {
                cout << "Order Another:" << endl;
            } else {
                cout << "Thank you for your purchase!" << endl;
                creditHeaderName();
                transactionOptions(loggedInUser);
            }
        } else {
            cout << "....Insufficient credit. Please add more credits to your account." << endl;
        }    
        } else{
            cout << "Cancelling Order"<<endl;
            cost = 0;
        }

    } while (true);
}

void nescafe(User& loggedInUser) {
    int choice;
    int plainCone = 15;
    int cup = 25;
    int cupOneTopping = 30;
    int cupTwoToppings = 35;
    int cupThreeToppings = 40;
    int cost = 0;
    string iceCream;

    do {
        centerText("Available Ice Cream Menu:");
        centerText("1: Plain cone (Php 15)");
        centerText("2: Plain cup (Php 25)");
        centerText("3: Cup with one topping (Php 30)");
        centerText("4: Cup with two toppings (Php 35)");
        centerText("5: Cup with three toppings (Php 40)");
        centerText("6: Go Back");
        
        cin >> choice;

        switch (choice) {
            case 1:
                iceCream = "Plain Cone";
                creditHeaderName();
                cout << "Plain cone has been added to your list" << endl;
                cost = plainCone;
                break;
            case 2:
                iceCream = "Plain Cup";
                creditHeaderName();
                cout << "Plain cup has been added to your list " << endl;
                cost = cup;
                break;
            case 3:
                iceCream = "Cup with one topping";
                creditHeaderName();
                cout << "Cup with one topping has been added to your list" << endl;
                cost = cupOneTopping;
                break;
            case 4:
                iceCream = "Cup with two toppings";
                creditHeaderName();
                cout << "Cup with two toppings has been added to your list" << endl;
                cost = cupTwoToppings;
                break;
            case 5:
                iceCream = "Cup with three toppings";
                creditHeaderName();
                cout << "Cup with three toppings has been added to your list" << endl;
                cost = cupThreeToppings;
                break;
            case 6:
                creditHeaderName();
                transactionOptions(loggedInUser);
                break;   
            default:
                cout << "Invalid input, order cannot be added." << endl;
                break;
        }
        
        string choice2;
        cout << "You Want to Proceed With Order? (Y/n): "<<endl;
        cin >> choice2;
        if (choice2 == "y" || choice2 == "Y"){

        if (loggedInUser.credit >= cost) {
            // Apply discount first
            double discountRate = 0.0; // initialize discountRate
            if (loggedInUser.academicStatus == "Dean's Lister") {
                discountRate = 0.2;
            } else if (loggedInUser.academicStatus == "President's Lister") {
                discountRate = 0.3;
            } else if (loggedInUser.academicStatus == "Athlete") {
                discountRate = 0.1;
            } else if (loggedInUser.academicStatus == "Scholar") {
                discountRate = 0.2;
            }

            // Calculate discounted cost
            double discountedCost = cost - (cost * discountRate);

            cout << "Price: Php " << cost << ", Discount: " << (discountRate * 100) << "%, Discounted Cost: Php " << discountedCost << endl;

            auto currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());

            // Convert time to string
            string purchaseTime = ctime(&currentTime);
            // Display time and date
            cout << "Time of Purchase: " << purchaseTime;

            // Update user's credit
            loggedInUser.credit -= discountedCost;

            // Update user information in the vector
            for (auto& user : users) {
                if (user.name == loggedInUser.name && user.idNumber == loggedInUser.idNumber) {
                    user.credit = loggedInUser.credit;
                    user.academicStatus = loggedInUser.academicStatus;
                    break;
                }
            }

            // Rewrite the entire file with updated user information
            ofstream outFile("user_info.txt");
            if (outFile.is_open()) {
                for (const auto& user : users) {
                    outFile << user.name << "," << user.idNumber << "," << user.pin << "," << user.credit << "," << user.academicStatus << endl;
                }
                outFile.close();
                centerText("Transaction complete!");
            } else {
                centerText("Error updating user information. Please try again later.");
                transactionOptions(loggedInUser);
            }

            string filename = loggedInUser.name + ".txt";
            ofstream userFile(filename, ios::app);
            if (userFile.is_open()) {
                userFile << "------------------------------------------------------------------------------------------------------------" << endl;
                userFile << "Nescafe: " << iceCream << ", Price: Php " << cost << ", Discount: " << (discountRate * 100) << "%, Discounted Cost: Php " << discountedCost << endl;
                userFile << "Time of Purchase: " << purchaseTime;
                userFile.close();
            } else {
                cout << "Error opening the user file for user: " << loggedInUser.name << endl;
            }

            // Prompt the user to cash out
            cout << "Press Y to Proceed and n to order another desert (Y/n): "; //changed waffle to desert
            string choice;  
            cin >> choice;

            if (choice != "Y" && choice != "y") {
                cout << "Order Another:" << endl;
            } else {
                cout << "Thank you for your purchase!" << endl;
                creditHeaderName();
                transactionOptions(loggedInUser);
            }
        } else {
            cout << "....Insufficient credit. Please add more credits to your account." << endl;
        }
        } else{
            cout << "Cancelling Order"<<endl;
            cost = 0;
        }
    } while (true);
}

void vmes(User& loggedInUser) {
    int choice;
    int siomai = 45;
    int egg = 40;
    int longganisa = 60;
    int calamares = 70;
    int friedChicken = 90;
    int cost = 0;
    string foodType;

    do {
        centerText("Available Dishes with Rice:");
        centerText("1: Siomai Rice (Php 45)");
        centerText("2: Egg with Rice (Php 40)");
        centerText("3: Longganisa with Rice (Php 60)");
        centerText("4: Calamares with Rice (Php 70)");
        centerText("5: Fried Chicken with Rice (Php 90)");
        centerText("6: Go Back");
        cin >> choice;
        
        switch (choice) {
            case 1:
                foodType = "Siomai rice";
                creditHeaderName();
                cout << "Siomai rice has been added to your list" << endl;
                cost = siomai;
                break;
            case 2:
                foodType = "Egg with rice";
                creditHeaderName();
                cout << "Egg with rice has been added to your list " << endl;
                cost = egg;
                break;
            case 3:
                foodType = "Longganisa with rice";
                creditHeaderName();
                cout << "Longganisa with rice has been added to your list" << endl;
                cost = longganisa;
                break;
            case 4:
                foodType = "Calamares with rice";
                creditHeaderName();
                cout << "Calamares with rice has been added to your list" << endl;
                cost = calamares;
                break;
            case 5:
                foodType = "Fried chicken with rice";
                creditHeaderName();
                cout << "Fried chicken with rice has been added to your list" << endl;
                cost = friedChicken;
                break;
            case 6:
                creditHeaderName();
                transactionOptions(loggedInUser);
                break;
            default:
                cout << "Invalid input, order cannot be added." << endl;
                break;
        }
        string choice2;
        cout << "You Want to Proceed With Order? (Y/n): "<<endl;
        cin >> choice2;
        if (choice2 == "y" || choice2 == "Y"){


        if (loggedInUser.credit >= cost) {
            // Apply discount first
            double discountRate = 0.0; // initialize discountRate
            if (loggedInUser.academicStatus == "Dean's Lister") {
                discountRate = 0.2;
            } else if (loggedInUser.academicStatus == "President's Lister") {
                discountRate = 0.3;
            } else if (loggedInUser.academicStatus == "Athlete") {
                discountRate = 0.1;
            } else if (loggedInUser.academicStatus == "Scholar") {
                discountRate = 0.2;
            }

            // Calculate discounted cost
            double discountedCost = cost - (cost * discountRate);

            cout << "Price: Php " << cost << ", Discount: " << (discountRate * 100) << "%, Discounted Cost: Php " << discountedCost << endl;

            auto currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());

            // Convert time to string
            string purchaseTime = ctime(&currentTime);
            // Display time and date
            cout << "Time of Purchase: " << purchaseTime;

            // Update user's credit
            loggedInUser.credit -= discountedCost;

            // Update user information in the vector
            for (auto& user : users) {
                if (user.name == loggedInUser.name && user.idNumber == loggedInUser.idNumber) {
                    user.credit = loggedInUser.credit;
                    user.academicStatus = loggedInUser.academicStatus;
                    break;
                }
            }

            // Rewrite the entire file with updated user information
            ofstream outFile("user_info.txt");
            if (outFile.is_open()) {
                for (const auto& user : users) {
                    outFile << user.name << "," << user.idNumber << "," << user.pin << "," << user.credit << "," << user.academicStatus << endl;
                }
                outFile.close();
                centerText("Transaction complete!");
            } else {
                centerText("Error updating user information. Please try again later.");
                transactionOptions(loggedInUser);
            }

            string filename = loggedInUser.name + ".txt";
            ofstream userFile(filename, ios::app);
            if (userFile.is_open()) {
                userFile << "------------------------------------------------------------------------------------------------------------" << endl;
                userFile << "VMES: " << foodType << ", Price: Php " << cost << ", Discount: " << (discountRate * 100) << "%, Discounted Cost: Php " << discountedCost << endl;
                userFile << "Time of Purchase: " << purchaseTime;
                userFile.close();
            } else {
                cout << "Error opening the user file for user: " << loggedInUser.name << endl;
            }

            // Prompt user to cash out
            cout << "Press Y to Proceed and n to order another dish (Y/n): ";
            string choice;
            cin >> choice;

            if (choice != "Y" && choice != "y") {
                cout << "Order Another:" << endl;
            } else {
                cout << "Thank you for your purchase!" << endl;
                creditHeaderName();
                transactionOptions(loggedInUser);
            }
        } else {
            cout << "....Insufficient credit. Please add more credits to your account." << endl;
        }
        } else{
            cout << "Cancelling Order"<<endl;
            cost = 0;
        }
    } while (true);
}

void orderStores(User loggedInUser) {
    int choice;
    centerText("Choose among the Three Stores Below");
    centerText("1. Belgian Waffle");
    centerText("2. Nescafe");
    centerText("3. VMES");
    centerText("4. Go Back");
    cout << "Enter your choice (1-4): ";
    cin >> choice;

    switch (choice) {
        case 1:
            creditHeaderName();
            cout << "Belgian Waffle" << endl;
            belgianWaffle(loggedInUser);
            break;
        case 2:
            creditHeaderName();
            cout << "Nescafe" << endl;
            nescafe(loggedInUser);
            break;
        case 3:
            creditHeaderName();
            cout << "VMES" << endl;
            vmes(loggedInUser);
            break;
        case 4:
            creditHeaderName();
            cout << "Go Back" << endl;
            transactionOptions(loggedInUser);
            break;
        default:
            cout << "Invalid choice. Please choose from 1 to 3." << endl;
            break;
    }
}

void transactionOptions(User loggedInUser) {
    int choice;
    centerText("Transaction Menu: ");
    centerText("1. Order ");
    centerText("2. Cash In ");
    centerText("3. Go Back ");
    centerText("Enter your choice (1-3): ");
    cin >> choice;

    switch (choice) {
        case 1:
            creditHeaderName();
            cout << "Order" << endl;
            orderStores(loggedInUser);
            break;
        case 2:
            creditHeaderName();
            cout << "Cash In" << endl;
            cashIn(loggedInUser);
            break;
        case 3:
            creditHeaderName();
            cout << "Go Back" << endl;
            userDashboard(loggedInUser);
            break;
        default:
            cout << "....Invalid choice. Please choose from 1 to 3." << endl;
            break;
    }
}

void userDashboard(User loggedInUser) {
    char choice;
    centerText("User Menu: ");
    centerText("1. Transaction ");
    centerText("2. Transaction History ");
    centerText("3. My account ");
    centerText("4. Logout ");
    centerText("Enter your choice (1-4): ");
    cout << "Credits: " << loggedInUser.credit << endl;
    cin >> choice;
    do{
    switch (choice) {
        case '1':
            creditHeaderName();
            cout << "Transactions" << endl;
            transactionOptions(loggedInUser);
            break;
        case '2':
        creditHeaderName();
            cout << "Transaction History" << endl;
            transactionHistory(loggedInUser);
            break;
        case '3':
            creditHeaderName();
            cout << "My Account: " << endl;
            accountInformation(loggedInUser);
            break;
        case '4':
            cout << "Logout" << endl;
            cout << "Logging out your account..." << endl;
            creditHeaderName();
            mainMenu();
            break;
        default:
            cout << "Invalid choice. Please choose between 1 and 4." << endl;
            creditHeaderName();
            userDashboard(loggedInUser);
            break;
        }
    } while(true);
}

void mainMenu() {
    char choice;
    centerText("Main Menu:");
    centerText("1. Create Account ");
    centerText("2. Account Log In");
    centerText("3. Exit ");
    centerText("Enter your choice (1-3): ");
    do{
    cin >> choice;

    switch (choice) {
        case '1':
            creditHeaderName();
            cout << "Create Account" << endl;
            createAccount();
            mainMenu();
            break;
        case '2':
            creditHeaderName();
            cout << "Account Log In" << endl;
            if (accountLogin()) {
                
            }
            else {
                mainMenu();
            }
            break;
        case '3':
            creditHeaderName();
            cout << "Exiting Program..." << endl;
            exit(0);
            break;
            
        default:
            cout << "Invalid choice. Please choose between 1 and 3." << endl;
    }
    
    }while(true);
}
