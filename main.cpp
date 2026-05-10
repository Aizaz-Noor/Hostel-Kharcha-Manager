#include <iostream>
#include <string>
#include <fstream>
#include "DataStructures.h"

using namespace std;

// ANSI Escape Codes for Styling
const string RESET = "\033[0m";
const string GREEN = "\033[32m";
const string RED = "\033[31m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";

// Global Data Structures
MemberMap members;
HistoryDLL timeline;
UndoStack undoStack;
DebtMaxHeap debtHeap(100);
int currentTxId = 1;

void displayMenu() {
    cout << "\n=========================================\n";
    cout << BLUE << " Welcome to Hostel Kharcha Manager " << RESET << "\n";
    cout << "=========================================\n";
    cout << "1. Add Member (Role 1)\n";
    cout << "2. Add Transaction (Role 2 & 3)\n";
    cout << "3. Undo Last Transaction (Role 2)\n";
    cout << "4. View Timeline (Role 2)\n";
    cout << "5. View Highest Debtor (Role 3)\n";
    cout << "6. Save & Exit\n";
    cout << "Choose an option: ";
}

void addMember() {
    string name;
    cout << "Enter Member Name: ";
    cin >> name;
    members.addMember(name);
    cout << GREEN << "Member " << name << " added successfully!" << RESET << "\n";
}

void addTransaction() {
    string payer, desc;
    double amount;
    cout << "Enter Payer Name: ";
    cin >> payer;
    cout << "Enter Description: ";
    cin >> desc;
    cout << "Enter Amount: ";
    cin >> amount;

    if (!members.getMember(payer)) {
        cout << RED << "Member does not exist. Please add them first." << RESET << "\n";
        return;
    }

    Transaction* newTx = new Transaction(currentTxId++, payer, desc, amount);
    
    // Core Roles Logic:
    
    // 1. Appending to Timeline
    timeline.append(newTx);
    
    // 2. Pushing to Undo Engine
    undoStack.push(newTx);
    
    // 3. Updating Balances in HashMap
    members.updateBalance(payer, amount);
    double owedAmount = (amount / 3.0); // Simple dummy logic: assuming 3 members evenly split
    
    // 4. Update DebtHeap (Mocked implementation for priority)
    debtHeap.insertOrUpdate(payer, 0); // Payer's debt decreases
    
    cout << GREEN << "Transaction added (" << desc << " : " << amount << ")" << RESET << "\n";
    delete newTx;
}

void undoTransaction() {
    Transaction* t = undoStack.pop();
    if (!t) {
        cout << RED << "No transactions to undo!" << RESET << "\n";
        return;
    }

    // Role 2 logic: Reverse balance and remote tail in timeline
    members.updateBalance(t->payer, -t->amount);
    timeline.removeTail();
    
    cout << YELLOW << "Undid Transaction: " << t->description << RESET << "\n";
    delete t; // Manual Memory Management
}

void viewTimeline() {
    Transaction* temp = timeline.head;
    cout << "\n--- Transaction Timeline ---\n";
    while (temp) {
        cout << "[" << temp->id << "] " << temp->payer << " paid " << GREEN << "$" << temp->amount << RESET << " for " << temp->description << "\n";
        temp = temp->next;
    }
}

int main() {
    int choice;
    do {
        displayMenu();
        cin >> choice;
        
        switch (choice) {
            case 1: addMember(); break;
            case 2: addTransaction(); break;
            case 3: undoTransaction(); break;
            case 4: viewTimeline(); break;
            case 5: debtHeap.printTopDebtor(); break;
            case 6: cout << "Saving and exiting...\n"; break;
            default: cout << RED << "Invalid choice!\n" << RESET;
        }
    } while (choice != 6);

    return 0;
}
