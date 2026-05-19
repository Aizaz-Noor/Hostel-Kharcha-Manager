#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <limits>
#include <fstream>
#include <windows.h>
#include "../include/MemberHash.h"
#include "../include/TransactionTimeline.h"
#include "../include/DebtHeap.h"

MemberHash* globalMemberHash = nullptr;

using namespace std;

// Configure Windows terminal to support UTF-8 and ANSI colors
void setupConsole() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

// ANSI color codes (prefixed with C_ to avoid Windows header name conflicts)
const string C_RST = "\033[0m";
const string C_BD  = "\033[1m";
const string C_DIM = "\033[2m";
const string C_GRN = "\033[92m";   // bright green
const string C_YLW = "\033[93m";   // bright yellow
const string C_RED = "\033[91m";   // bright red
const string C_CYN = "\033[96m";   // bright cyan
const string C_BLU = "\033[94m";   // bright blue
const string C_MGT = "\033[95m";   // bright magenta
const string C_WHT = "\033[97m";   // white
const string C_GRY = "\033[90m";   // dark grey

// UI box panel width in characters
const int W = 54;

// Print box layout line dividers
void divTop()  { cout << C_CYN << "  +" << string(W, '-') << "+" << C_RST << "\n"; }
void divMid()  { cout << C_CYN << "  |" << string(W, '-') << "|" << C_RST << "\n"; }
void divBot()  { cout << C_CYN << "  +" << string(W, '-') << "+" << C_RST << "\n"; }
void divLine() { cout << C_GRY << "  " << string(W + 2, '-') << C_RST << "\n"; }

// Print a string centered inside the UI panel
// visLen specifies the length of text without ANSI color codes
void centerRow(const string& text, int visLen, const string& colorPrefix = "") {
    int pad   = W - visLen;
    int left  = pad / 2;
    int right = pad - left;
    cout << C_CYN << "  |" << C_RST
         << string(left, ' ')
         << colorPrefix << text << C_RST
         << string(right, ' ')
         << C_CYN << "|" << C_RST << "\n";
}

// Print a formatted menu option row
void menuRow(const string& num, const string& label, const string& tag,
             const string& numColor, const string& tagColor) {
    // Visible layout inside the | borders:
    //  "  [ N ]  <label padded to 30>  [tag padded to 10]  "  = 52 visible chars = W-2
    string visNum = "[ " + num + " ]";   // 5 chars
    int labelW    = 31;
    int tagW      = 12;

    string tagStr = tag.empty() ? string(tagW, ' ') : ("[" + tag + "]");
    int tagPad    = tagW - (int)tagStr.size();
    if (tagPad < 0) tagPad = 0;

    int labelPad  = labelW - (int)label.size();
    if (labelPad < 1) labelPad = 1;

    // visible width of content = 2 + 5 + 2 + label (31) + tagStr (12) + 2
    // = 9 + 31 + 12 + 2 = 54 = W
    cout << C_CYN << "  |" << C_RST << "  "
         << numColor << C_BD << visNum << C_RST << "  "
         << C_WHT << label << C_RST
         << string(labelPad, ' ')
         << tagColor << C_DIM << tagStr << C_RST
         << string(tagPad, ' ') << "  "
         << C_CYN << "|" << C_RST << "\n";
}

// General UI helper functions
void clearScreen() { system("cls"); }

void flushCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pause() {
    cout << "\n" << C_DIM << C_CYN << "  Press Enter to continue..." << C_RST;
    string dummy;
    getline(cin, dummy);
}

// Calculate the hash map bucket index for an ID
int computeBucket(const string& id) {
    int h = 0;
    for (char c : id) h += (int)c;
    return h % 100;
}

// Display terminal splash screen with system overview
void printSplash() {
    clearScreen();
    cout << "\n";
    cout << C_CYN << "      __ __    __ __    __  ___ \n";
    cout          << "     / // /   / //_/   /  |/  / \n";
    cout          << "    / _  /   / ,<     / /|_/ /  \n";
    cout          << "   /_//_/   /_/|_|   /_/  /_/   \n" << C_RST;
    cout << "\n";
    cout << C_BD << C_WHT << "  H O S T E L   K H A R C H A   M A N A G E R\n" << C_RST;
    cout << "\n";
    cout << C_GRY;
    cout << "   System Features:\n";
    cout << "     [1] Fast Member Registration    ->  Manage profiles & deposits\n";
    cout << "     [2] Detailed Ledger             ->  Chronological expense tracking\n";
    cout << "     [3] Safe Revert System          ->  Undo mistakes instantly\n";
    cout << "     [4] Debt Priority Analytics     ->  Real-time financial tracking\n";
    cout << C_RST << "\n";
    cout << C_DIM << C_GRY << "   Team: Abdul Basit Shahid  |  Aizaz Noor  |  Muhammad Abdullah\n" << C_RST;
    cout << "\n";
    divLine();
    cout << "\n" << C_DIM << C_CYN << "   Press Enter to start..." << C_RST;
    cin.get();
}

// Print header block above menus showing current counts
void printHeader(int members, int txns) {
    string title    = "HOSTEL KHARCHA MANAGER";
    string subtitle = "Members: " + to_string(members)
                    + "   |   Transactions: " + to_string(txns);
    cout << "\n";
    divTop();
    centerRow(title,    (int)title.size(),    C_BD + C_WHT);
    centerRow(subtitle, (int)subtitle.size(), C_DIM + C_GRY);
    divBot();
}

// Display the primary main menu options
void printMenu(int members, int txns) {
    clearScreen();
    printHeader(members, txns);
    cout << "\n";
    divTop();
    centerRow("MAIN MENU", 9, C_BD + C_CYN);
    divMid();
    menuRow("1", "Add New Roommate",          "Profile",   C_GRN, C_GRN);
    menuRow("2", "Remove a Roommate",         "Delete",    C_RED, C_RED);
    menuRow("3", "Log a Shared Expense",      "Ledger",    C_YLW, C_YLW);
    menuRow("4", "Undo Last Transaction",     "Revert",    C_RED, C_RED);
    menuRow("5", "View Transaction Timeline", "History",   C_CYN, C_CYN);
    menuRow("6", "View Roommate Balances",    "Finances",  C_BLU, C_BLU);
    menuRow("7", "Who Owes The Most?",        "Analytics", C_MGT, C_MGT);
    menuRow("8", "Settle Debt / Deposit",     "Clear",     C_GRN, C_GRN);
    menuRow("9", "Exit & Save Data",          "",          C_GRY, C_GRY);
    divBot();
    cout << "\n  " << C_BD << "Your choice: " << C_RST;
}

// Print a sub-header block when navigating to a feature section
void sectionHeader(const string& title, const string& subtitle, const string& color) {
    cout << "\n";
    divTop();
    centerRow(title,   (int)title.size(),   C_BD + color);
    centerRow(subtitle, (int)subtitle.size(), C_DIM + C_GRY);
    divBot();
    cout << "\n";
}

// Split an expense using either equal or custom weighted sharing
void splitExpense(MemberHash& profiles, const string& payerId, double amount, BalanceChange*& outChanges, int& outNumChanges) {
    int count = 0;
    MemberNode** members = profiles.getAllMembers(count);
    if (count == 0) {
        delete[] members;
        outChanges = nullptr;
        outNumChanges = 0;
        return;
    }
    
    outNumChanges = count;
    outChanges = new BalanceChange[count];
    int changeIdx = 0;

    cout << "\n  How to split this expense?\n";
    cout << "  1. Equal Split\n";
    cout << "  2. Custom Weighted Distribution\n";
    cout << "  Choice: ";
    int splitChoice;
    cin >> splitChoice;

    if (cin.fail() || (splitChoice != 1 && splitChoice != 2)) {
        flushCin();
        cout << C_RED << "  ! Invalid choice. Falling back to Equal Split.\n" << C_RST;
        splitChoice = 1;
    } else {
        flushCin();
    }

    if (splitChoice == 1) {
        double share = amount / count;

        if (payerId == "FUND") {
            for (int i = 0; i < count; i++) {
                profiles.updateBalance(members[i]->id, -share);
                outChanges[changeIdx++] = {members[i]->id, -share};
            }
            cout << C_GRY
                 << "\n  Per-person share : Rs. " << fixed << setprecision(2) << share
                 << "\n  Paid by Common Fund (No individual credited)"
                 << "\n" << C_RST;
        } else {
            // Payer gets back (amount - their own share)
            profiles.updateBalance(payerId, amount - share);
            outChanges[changeIdx++] = {payerId, amount - share};

            // Everyone else is debited their share
            for (int i = 0; i < count; i++) {
                MemberNode* m = members[i];
                if (m->id != payerId) {
                    profiles.updateBalance(m->id, -share);
                    outChanges[changeIdx++] = {m->id, -share};
                }
            }

            cout << C_GRY
                 << "\n  Per-person share : Rs. " << fixed << setprecision(2) << share
                 << "\n  Payer credited   : Rs. " << fixed << setprecision(2) << (amount - share)
                 << "\n" << C_RST;
        }
    } else {
        double totalWeighted = 0;
        double* weights = new double[count];
        
        cout << "\n  Enter weight or amount for each member:\n";
        for (int i = 0; i < count; i++) {
            MemberNode* m = members[i];
            cout << "  " << m->name << " (" << m->id << "): ";
            cin >> weights[i];
            if (cin.fail() || weights[i] < 0) {
                flushCin();
                cout << C_RED << "  ! Invalid weight. Defaulting to 0.\n" << C_RST;
                weights[i] = 0;
            } else {
                flushCin();
            }
            totalWeighted += weights[i];
        }

        if (totalWeighted == 0) {
            cout << C_RED << "  ! Total weight cannot be zero. Falling back to Equal Split.\n" << C_RST;
            double share = amount / count;
            
            if (payerId == "FUND") {
                for (int i = 0; i < count; i++) {
                    profiles.updateBalance(members[i]->id, -share);
                    outChanges[changeIdx++] = {members[i]->id, -share};
                }
            } else {
                profiles.updateBalance(payerId, amount - share);
                outChanges[changeIdx++] = {payerId, amount - share};
                for (int i = 0; i < count; i++) {
                    MemberNode* m = members[i];
                    if (m->id != payerId) {
                        profiles.updateBalance(m->id, -share);
                        outChanges[changeIdx++] = {m->id, -share};
                    }
                }
            }
        } else {
            // If paid by a specific member, credit them first
            if (payerId != "FUND") {
                profiles.updateBalance(payerId, amount);
            }
            
            // Then deduct everyone's share based on weight
            for (int i = 0; i < count; i++) {
                MemberNode* m = members[i];
                double share = (weights[i] / totalWeighted) * amount;
                profiles.updateBalance(m->id, -share);
                
                cout << C_GRY << "  - " << m->name << " share: Rs. " 
                     << fixed << setprecision(2) << share << C_RST << "\n";
            }
        }
        delete[] weights;
    }

    delete[] members;
}

// Save current data state to CSV files
void saveData(MemberHash& profiles, TransactionTimeline& timeline) {
    ofstream mf("members.csv");
    int count = 0;
    MemberNode** members = profiles.getAllMembers(count);
    for (int i = 0; i < count; i++) {
        mf << members[i]->id << "|" << members[i]->name << "|" << members[i]->password << "|" << members[i]->balance << "\n";
    }
    delete[] members;
    mf.close();

    ofstream tf("transactions.csv");
    tf << timeline.getCounter() << "\n"; // Save the counter to prevent ID conflicts
    TransactionNode* current = timeline.getHead();
    while (current != nullptr) {
        tf << current->transactionId << "|" << current->description << "|" 
           << current->amount << "|" << current->payerId << "|" << current->numChanges;
        for (int i = 0; i < current->numChanges; i++) {
            tf << "|" << current->changes[i].memberId << "|" << current->changes[i].changeAmount;
        }
        tf << "\n";
        current = current->next;
    }
    tf.close();
}

void loadData(MemberHash& profiles, TransactionTimeline& timeline, int& membersCount, int& txnsCount) {
    ifstream mf("members.csv");
    if (mf.is_open()) {
        string line;
        while (getline(mf, line)) {
            stringstream ss(line);
            string id, name, part3, part4;
            if (getline(ss, id, '|') && getline(ss, name, '|')) {
                string password = "123"; // default password
                double bal = 0.0;
                if (getline(ss, part3, '|')) {
                    if (getline(ss, part4, '|')) {
                        // formats: id|name|password|balance
                        password = part3;
                        bal = stod(part4);
                    } else {
                        // formats: id|name|balance (old format)
                        bal = stod(part3);
                    }
                }
                if (profiles.addMember(id, name, password, 0)) { // Add with 0 initial deposit
                    profiles.updateBalance(id, bal);   // Sync to exact balance
                    membersCount++;
                }
            }
        }
        mf.close();
    }

    ifstream tf("transactions.csv");
    if (tf.is_open()) {
        string counterStr;
        if (getline(tf, counterStr)) {
            timeline.setCounter(stoi(counterStr));
        }
        string line;
        while (getline(tf, line)) {
            stringstream ss(line);
            string tId, desc, amtStr, payerId, numChgStr;
            if (getline(ss, tId, '|') && getline(ss, desc, '|') && 
                getline(ss, amtStr, '|') && getline(ss, payerId, '|') && getline(ss, numChgStr, '|')) {
                
                int id = stoi(tId);
                double amt = stod(amtStr);
                int numChg = stoi(numChgStr);
                
                BalanceChange* changes = nullptr;
                if (numChg > 0) {
                    changes = new BalanceChange[numChg];
                    for (int i = 0; i < numChg; i++) {
                        string cId, cAmt;
                        getline(ss, cId, '|');
                        getline(ss, cAmt, '|');
                        changes[i].memberId = cId;
                        changes[i].changeAmount = stod(cAmt);
                    }
                }
                timeline.restoreTransaction(id, desc, amt, payerId, changes, numChg);
                txnsCount++;
            }
        }
        tf.close();
    }
}

// ══════════════════════════════════════════════════════════════════════════════
int main() {
    setupConsole();
    printSplash();

    MemberHash          profiles;
    globalMemberHash  = &profiles;
    TransactionTimeline timeline;
    DebtHeap            analytics(100);

    int members = 0, txns = 0;
    
    loadData(profiles, timeline, members, txns);

    bool portalRunning = true;
    while (portalRunning) {
        clearScreen();
        cout << "\n\n";
        divTop();
        centerRow("HOSTEL KHARCHA MANAGER PORTAL", 29, C_CYN + C_BD);
        divMid();
        menuRow("1", "Log In (Roommate or Admin)", "Access System", C_GRN, C_GRN);
        menuRow("2", "Sign Up (Register Roommate)", "Join System",   C_YLW, C_YLW);
        menuRow("3", "Exit Portal",                 "Quit App",      C_RED, C_RED);
        divBot();
        cout << "\n  " << C_BD << "Your choice: " << C_RST;

        int portalChoice = 0;
        cin >> portalChoice;
        if (cin.fail()) {
            flushCin();
            cout << C_RED << "\n  ! Invalid option. Enter 1-3.\n" << C_RST;
            pause();
            continue;
        }
        flushCin();

        if (portalChoice == 1) {
            clearScreen();
            cout << "\n\n";
            divTop();
            centerRow("ROOMMATE / ADMIN LOGIN", 22, C_CYN + C_BD);
            divBot();
            cout << "\n";

            string user, pass;
            cout << "  Username / Member ID : ";
            cin >> user;
            flushCin();

            cout << "  Password             : ";
            cin >> pass;
            flushCin();

            bool loginSuccess = false;
            string displayName = "";

            if (user == "admin" && pass == "admin123") {
                loginSuccess = true;
                displayName = "Administrator";
            } else {
                MemberNode* m = profiles.getMember(user);
                if (m != nullptr && m->password == pass) {
                    loginSuccess = true;
                    displayName = m->name;
                }
            }

            if (loginSuccess) {
                cout << C_GRN << "\n  Access Granted! Welcome, " << displayName << ".\n" << C_RST;
                pause();

                int choice = 0;
                while (choice != 9) {
                    printMenu(members, txns);
                    cin >> choice;

                    if (cin.fail()) {
                        flushCin();
                        cout << C_RED << "\n  ! Invalid option. Please enter a number.\n" << C_RST;
                        pause();
                        continue;
                    }
                    flushCin();

                    // [1] Add Roommate
                    if (choice == 1) {
                        clearScreen();
                        sectionHeader("ADD NEW ROOMMATE", "Register a new member to the hostel pool", C_GRN);

                        string id, name, roommatePass;
                        double deposit = 0.0;
                        cout << "  Enter Member ID   (e.g. A01) : ";  cin >> id;
                        flushCin();
                        cout << "  Enter Member Name            : ";  getline(cin, name);
                        cout << "  Enter Password               : ";  cin >> roommatePass;
                        flushCin();
                        cout << "  Initial Deposit   (Rs.)      : ";  cin >> deposit;
                        if (cin.fail() || deposit < 0) {
                            flushCin();
                            cout << C_RED << "  ! Invalid deposit. Defaulting to 0.\n" << C_RST;
                            deposit = 0.0;
                        } else {
                            flushCin();
                        }

                        if (profiles.addMember(id, name, roommatePass, deposit)) {
                            members++;
                            cout << C_GRN << "\n  + '" << name << "'  (ID: " << id << ")  added!\n" << C_RST;
                            saveData(profiles, timeline);
                        } else {
                            cout << C_RED << "\n  ! ID '" << id << "' already exists or input is empty.\n" << C_RST;
                        }
                        pause();
                    }

                    // [2] Remove Roommate
                    else if (choice == 2) {
                        clearScreen();
                        sectionHeader("REMOVE A ROOMMATE", "Safely delete a member from the hostel pool", C_RED);
                        
                        if (members == 0) {
                            cout << C_RED << "  ! No members to remove.\n" << C_RST;
                            pause();
                            continue;
                        }
                        
                        string id;
                        cout << "  Enter Member ID to remove: "; cin >> id;
                        flushCin();
                        
                        MemberNode* m = profiles.getMember(id);
                        if (m == nullptr) {
                            cout << C_RED << "\n  ! Member ID '" << id << "' not found.\n" << C_RST;
                        } else {
                            bool proceedWithDeletion = false;
                            
                            if (m->balance > 0.0) {
                                cout << C_YLW << "\n  ! " << m->name << " has a surplus of Rs. " << fixed << setprecision(2) << m->balance << ".\n" << C_RST;
                                cout << "  Do you want to issue a refund and remove them? (Y/N): ";
                                char confirm;
                                cin >> confirm;
                                flushCin();
                                if (toupper(confirm) == 'Y') {
                                    double refundAmt = m->balance;
                                    profiles.updateBalance(id, -refundAmt); // Bring to 0
                                    
                                    BalanceChange* changes = new BalanceChange[1];
                                    changes[0].memberId = id;
                                    changes[0].changeAmount = -refundAmt;
                                    timeline.logTransaction("Final Refund (Clearance)", -refundAmt, "FUND", changes, 1);
                                    txns++;
                                    cout << C_GRN << "  + Refund issued.\n" << C_RST;
                                    proceedWithDeletion = true;
                                }
                            } else if (m->balance < 0.0) {
                                cout << C_RED << "\n  ! " << m->name << " owes Rs. " << fixed << setprecision(2) << -m->balance << ".\n" << C_RST;
                                cout << "  Has this debt been settled in cash? (Y/N): ";
                                char confirm;
                                cin >> confirm;
                                flushCin();
                                if (toupper(confirm) == 'Y') {
                                    double paymentAmt = -m->balance;
                                    profiles.updateBalance(id, paymentAmt); // Bring to 0
                                    
                                    BalanceChange* changes = new BalanceChange[1];
                                    changes[0].memberId = id;
                                    changes[0].changeAmount = paymentAmt;
                                    timeline.logTransaction("Final Settlement (Clearance)", paymentAmt, id, changes, 1);
                                    txns++;
                                    cout << C_GRN << "  + Debt cleared.\n" << C_RST;
                                    proceedWithDeletion = true;
                                }
                            } else {
                                cout << C_YLW << "\n  Are you sure you want to remove " << m->name << "? (Y/N): " << C_RST;
                                char confirm;
                                cin >> confirm;
                                flushCin();
                                if (toupper(confirm) == 'Y') {
                                    proceedWithDeletion = true;
                                }
                            }
                            
                            if (proceedWithDeletion) {
                                if (profiles.removeMember(id)) {
                                    members--;
                                    cout << C_GRN << "\n  + Member '" << id << "' removed successfully.\n" << C_RST;
                                }
                            } else {
                                cout << C_RED << "\n  ! Deletion cancelled.\n" << C_RST;
                            }
                        }
                        pause();
                    }

                    // [3] Log Expense
                    else if (choice == 3) {
                        clearScreen();
                        sectionHeader("LOG SHARED EXPENSE", "Record a new expense and split it among members", C_YLW);

                        if (members == 0) {
                            cout << C_RED << "  ! No members yet. Add roommates first (Option 1).\n" << C_RST;
                            pause();
                            continue;
                        }

                        string payerId, desc;
                        double amount;

                        cout << "  Who paid? (Member ID or type 'FUND') : ";  cin >> payerId;
                        flushCin();
                        
                        for (char &c : payerId) c = toupper(c);
                        
                        if (payerId != "FUND" && !profiles.getMember(payerId)) {
                            cout << C_RED << "\n  ! Member ID '" << payerId << "' not found.\n" << C_RST;
                            pause();
                            continue;
                        }

                        cout << "  What was it for?     : ";  getline(cin, desc);
                        cout << "  Amount (Rs.)         : ";  cin >> amount;
                        if (cin.fail() || amount <= 0) {
                            flushCin();
                            cout << C_RED << "\n  ! Invalid amount. Must be a positive number.\n" << C_RST;
                            pause();
                            continue;
                        }
                        flushCin();
                        
                        BalanceChange* changes = nullptr;
                        int numChanges = 0;

                        splitExpense(profiles, payerId, amount, changes, numChanges);
                        
                        timeline.logTransaction(desc, amount, payerId, changes, numChanges);
                        txns++;

                        cout << C_GRN << "\n  + Logged and split among " << members << " members.\n" << C_RST;
                        pause();
                    }
                    // [4] Undo Transaction
                    else if (choice == 4) {
                        clearScreen();
                        sectionHeader("UNDO LAST TRANSACTION", "Revert the most recent expense completely", C_RED);
                        timeline.undoLastTransaction(globalMemberHash);
                        if (txns > 0) txns--;
                        pause();
                    }

                    // [5] Timeline
                    else if (choice == 5) {
                        clearScreen();
                        sectionHeader("TRANSACTION TIMELINE", "View all recorded expenses chronologically", C_CYN);
                        timeline.printTimeline();
                        pause();
                    }

                    // [6] Balances
                    else if (choice == 6) {
                        clearScreen();
                        sectionHeader("ROOMMATE BALANCES", "View live net balances for all members", C_BLU);
                        profiles.printAllMembers();
                        pause();
                    }

                    // [7] Debt Report
                    else if (choice == 7) {
                        clearScreen();
                        sectionHeader("DEBT PRIORITY REPORT", "Identify who owes the most money immediately", C_MGT);
                        analytics.buildHeapFromHashMap();
                        analytics.printSummaryReport();
                        pause();
                    }

                    // [8] Settle Debt / Deposit
                    else if (choice == 8) {
                        clearScreen();
                        sectionHeader("SETTLE DEBT / DEPOSIT", "Add money to a roommate's account to clear debts", C_GRN);
                        
                        if (members == 0) {
                            cout << C_RED << "  ! No members yet. Add roommates first.\n" << C_RST;
                            pause();
                            continue;
                        }
                        
                        string id;
                        double amount;
                        cout << "  Enter Member ID (who is paying?): "; cin >> id;
                        flushCin();
                        
                        MemberNode* m = profiles.getMember(id);
                        if (m == nullptr) {
                            cout << C_RED << "\n  ! Member ID '" << id << "' not found.\n" << C_RST;
                            pause();
                            continue;
                        }
                        
                        cout << "  Enter Amount (Rs.) (+ for Deposit, - for Withdraw) : "; cin >> amount;
                        if (cin.fail() || amount == 0) {
                            flushCin();
                            cout << C_RED << "\n  ! Invalid amount. Must be non-zero.\n" << C_RST;
                            pause();
                            continue;
                        }
                        flushCin();
                        
                        profiles.updateBalance(id, amount);
                        
                        // Log as a transaction to allow undo
                        BalanceChange* changes = new BalanceChange[1];
                        changes[0].memberId = id;
                        changes[0].changeAmount = amount;
                        string tDesc = (amount > 0) ? "Debt Settlement (Deposit)" : "Refund (Withdrawal)";
                        timeline.logTransaction(tDesc, amount, (amount > 0) ? id : "FUND", changes, 1);
                        txns++;
                        
                        if (amount > 0) {
                            cout << C_GRN << "\n  + Rs. " << fixed << setprecision(2) << amount 
                                 << " deposited for " << m->name << ".\n" << C_RST;
                        } else {
                            cout << C_GRN << "\n  + Rs. " << fixed << setprecision(2) << -amount 
                                 << " withdrawn/refunded for " << m->name << ".\n" << C_RST;
                        }
                        pause();
                    }

                    // [9] Logout / Exit menu
                    else if (choice == 9) {
                        clearScreen();
                        cout << "\n";
                        divTop();
                        centerRow("Logging Out & Saving Data...", 28, C_YLW);
                        divBot();
                        cout << "\n";
                        saveData(profiles, timeline);
                        pause();
                    }

                    else {
                        cout << C_RED << "\n  ! Invalid option. Enter 1-9.\n" << C_RST;
                        pause();
                    }
                }
            } else {
                cout << C_RED << "\n  Invalid Credentials! Access Denied.\n" << C_RST;
                pause();
            }
        }
        else if (portalChoice == 2) {
            clearScreen();
            cout << "\n\n";
            divTop();
            centerRow("ROOMMATE REGISTRATION / SIGN UP", 30, C_YLW + C_BD);
            divBot();
            cout << "\n";

            string id, name, roommatePass;
            double deposit = 0.0;
            cout << "  Enter Member ID   (e.g. A01) : ";  cin >> id;
            flushCin();
            cout << "  Enter Member Name            : ";  getline(cin, name);
            cout << "  Enter Password               : ";  cin >> roommatePass;
            flushCin();
            cout << "  Initial Deposit   (Rs.)      : ";  cin >> deposit;
            if (cin.fail() || deposit < 0) {
                flushCin();
                cout << C_RED << "  ! Invalid deposit. Defaulting to 0.\n" << C_RST;
                deposit = 0.0;
            } else {
                flushCin();
            }

            if (profiles.addMember(id, name, roommatePass, deposit)) {
                members++;
                cout << C_GRN << "\n  + Roommate '" << name << "' registered successfully!\n" << C_RST;
                cout << "    You can now log in using ID: " << id << "\n";
                saveData(profiles, timeline);
            } else {
                cout << C_RED << "\n  ! ID '" << id << "' already exists or input is empty.\n" << C_RST;
            }
            pause();
        }
        else if (portalChoice == 3) {
            portalRunning = false;
            clearScreen();
            cout << "\n";
            divTop();
            centerRow("Thanks for using Kharcha Manager!", 33, C_GRN);
            centerRow("Goodbye! :)", 11, C_DIM + C_GRN);
            divBot();
            cout << "\n";
        }
        else {
            cout << C_RED << "\n  ! Invalid option. Enter 1-3.\n" << C_RST;
            pause();
        }
    }

    saveData(profiles, timeline);
    return 0;
}