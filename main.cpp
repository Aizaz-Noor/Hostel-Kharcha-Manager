#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <cstdlib>
#include "DataStructures.h"

using namespace std;

// Global Data Structures
MemberMap members;
HistoryDLL timeline;
UndoStack undoStack;
DebtMaxHeap debtHeap(100);
int currentTxId = 1;

// Utility Functions
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pressEnterToContinue() {
    cout << "\n" << YELLOW << "Press Enter to return to menu..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int getSafeInt(string prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            return value;
        } else {
            cout << RED << "Invalid input. Please enter a number." << RESET << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void printHeader(string title) {
    clearScreen();
    // Vertical padding
    for (int i = 0; i < 5; i++) cout << endl;
    
    // Horizontal padding (approx 20 spaces)
    string hPad = "                    ";
    cout << hPad << BLUE << "-------------------------------------------------------" << RESET << "\n";
    cout << hPad << BOLD << CYAN << "            " << title << RESET << "\n";
    cout << hPad << BLUE << "-------------------------------------------------------" << RESET << "\n\n";
}

void displayMenu() {
    clearScreen();
    // Vertical padding
    for (int i = 0; i < 5; i++) cout << endl;
    
    string hPad = "                    "; // Horizontal padding
    cout << hPad << BLUE << "+-----------------------------------------------------+" << RESET << "\n";
    cout << hPad << BLUE << "| " << BOLD << MAGENTA << "             HOSTEL KHARCHA MANAGER              " << BLUE << " |" << RESET << "\n";
    cout << hPad << BLUE << "+-----------------------------------------------------+" << RESET << "\n";
    cout << hPad << BLUE << "| " << YELLOW << "1." << RESET << " Member Management                               " << BLUE << " |" << RESET << "\n";
    cout << hPad << BLUE << "| " << YELLOW << "2." << RESET << " Transaction Management                          " << BLUE << " |" << RESET << "\n";
    cout << hPad << BLUE << "| " << YELLOW << "3." << RESET << " Undo Last Transaction                           " << BLUE << " |" << RESET << "\n";
    cout << hPad << BLUE << "| " << YELLOW << "4." << RESET << " View Timeline                                   " << BLUE << " |" << RESET << "\n";
    cout << hPad << BLUE << "| " << YELLOW << "5." << RESET << " Debt Analysis (Highest Debtor)                  " << BLUE << " |" << RESET << "\n";
    cout << hPad << BLUE << "| " << YELLOW << "6." << RESET << " View All Members & Balances                     " << BLUE << " |" << RESET << "\n";
    cout << hPad << BLUE << "| " << YELLOW << "7." << RESET << " Save & Exit                                     " << BLUE << " |" << RESET << "\n";
    cout << hPad << BLUE << "+-----------------------------------------------------+" << RESET << "\n";
    cout << "\n" << hPad << "Choose an option (1-7): ";
}

void addMember() {
    printHeader("ADD NEW MEMBER");
    string name;
    cout << "Enter Member Name: ";
    cin >> name;
    members.addMember(name);
    cout << "\n" << GREEN << "✔ Member '" << name << "' added successfully!" << RESET << "\n";
    pressEnterToContinue();
}

void addTransaction() {
    printHeader("ADD TRANSACTION (FEATURE 4)");
    
    if (!members.hasMembers()) {
        cout << RED << "✖ No members found. Please add members first." << RESET << "\n";
        pressEnterToContinue();
        return;
    }

    cout << "--- Registered Members ---" << endl;
    members.printAllMembers();
    cout << "--------------------------" << endl;

    string payer, desc;
    double amount;
    cout << "\nEnter Payer Name: ";
    cin >> payer;
    
    Member* payerPtr = members.getMember(payer);
    if (!payerPtr) {
        cout << RED << "✖ Member '" << payer << "' does not exist." << RESET << "\n";
        pressEnterToContinue();
        return;
    }

    cout << "Enter Description: ";
    cin.ignore(); // Clear newline
    getline(cin, desc);
    
    cout << "Enter Amount: ";
    while (!(cin >> amount) || amount <= 0) {
        cout << RED << "Invalid amount. Enter a positive number: " << RESET;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "\nSplit Type:\n1. Equal Split\n2. Weighted Split\nChoice: ";
    int splitChoice = getSafeInt("");

    if (splitChoice == 2) {
        double totalWeight = 0;
        double* weights = new double[members.getMemberCount()];
        for (int i = 0; i < members.getMemberCount(); i++) {
            Member* m = members.getMemberAt(i);
            cout << "Enter weight for " << m->name << ": ";
            cin >> weights[i];
            totalWeight += weights[i];
        }

        payerPtr->totalPaid += amount;
        for (int i = 0; i < members.getMemberCount(); i++) {
            Member* m = members.getMemberAt(i);
            m->totalOwed += (amount * (weights[i] / totalWeight));
        }
        delete[] weights;
    } else {
        // Default: Equal Split
        members.updateBalances(payer, amount);
    }

    Transaction* newTx = new Transaction(currentTxId++, payer, desc, amount);
    timeline.append(newTx);
    undoStack.push(newTx);
    
    // Update DebtHeap (Feature 5: Live Ranking)
    debtHeap.update(members);
    
    cout << "\n" << GREEN << "✔ Transaction recorded and split successfully!" << RESET << "\n";
    cout << "  " << desc << " ($" << amount << ")\n";
    
    delete newTx; 
    pressEnterToContinue();
}

void undoTransaction() {
    printHeader("UNDO TRANSACTION (FEATURE 3)");
    Transaction* t = undoStack.pop();
    if (!t) {
        cout << RED << "✖ No transactions to undo!" << RESET << "\n";
        pressEnterToContinue();
        return;
    }

    // Role 2 logic: Reverse balance and remove tail in timeline
    // Reversing balance splitting: subtract from payer's Paid, add back to everyone's Owed
    Member* payer = members.getMember(t->payer);
    if (payer) {
        payer->totalPaid -= t->amount;
        double share = t->amount / members.getMemberCount();
        for (int i = 0; i < members.getMemberCount(); i++) {
            Member* m = members.getMemberAt(i);
            if (m) m->totalOwed -= share;
        }
    }
    
    timeline.removeTail();
    debtHeap.update(members); // Update heap after undo
    
    cout << YELLOW << "↺ Undid Transaction: " << t->description << " ($" << t->amount << ")" << RESET << "\n";
    delete t; 
    pressEnterToContinue();
}

void viewTimeline() {
    printHeader("TRANSACTION TIMELINE (FEATURE 2)");
    Transaction* temp = timeline.head;
    if (!temp) {
        cout << YELLOW << "No transactions yet." << RESET << "\n";
    } else {
        int count = 1;
        while (temp) {
            cout << count++ << ". " << CYAN << "[" << temp->payer << "]" << RESET 
                 << " paid " << GREEN << "$" << temp->amount << RESET 
                 << " for " << BOLD << temp->description << RESET << "\n";
            temp = temp->next;
        }
    }
    pressEnterToContinue();
}

void viewTopDebtor() {
    printHeader("DEBT PRIORITIZATION (FEATURE 5)");
    debtHeap.update(members); // Ensure fresh data
    debtHeap.printTopDebtor();
    pressEnterToContinue();
}

void generateSummaryReport() {
    printHeader("SUMMARY REPORT (FEATURE 6)");
    members.printSummaryReport();
    pressEnterToContinue();
}

int main() {
    int choice;
    do {
        displayMenu();
        choice = getSafeInt("Choose an option (1-7): ");
        
        switch (choice) {
            case 1: addMember(); break;
            case 2: addTransaction(); break;
            case 3: undoTransaction(); break;
            case 4: viewTimeline(); break;
            case 5: viewTopDebtor(); break;
            case 6: generateSummaryReport(); break;
            case 7: 
                printHeader("EXITING");
                cout << "Saving data and exiting. Goodbye!\n"; 
                break;
            default: 
                cout << RED << "Invalid choice! Press Enter to try again." << RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
        }
    } while (choice != 7);

    return 0;
}
