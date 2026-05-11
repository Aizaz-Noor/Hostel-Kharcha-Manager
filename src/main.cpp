#include <iostream>
#include "../include/MemberHash.h"
#include "../include/TransactionTimeline.h"
#include "../include/DebtHeap.h"

using namespace std;

// ANSI Color Codes for Premium Console UI
const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";

void printMenu() {
    cout << "\n============================================\n";
    cout << CYAN << BOLD << "       HOSTEL KHARCHA MANAGER v1.0" << RESET << "\n";
    cout << "============================================\n";
    cout << "1. " << GREEN << "Add New Roommate" << RESET << "\n";
    cout << "2. " << YELLOW << "Log New Shared Expense" << RESET << "\n";
    cout << "3. " << RED << "Undo Last Transaction" << RESET << "\n";
    cout << "4. " << CYAN << "View Transaction Timeline" << RESET << "\n";
    cout << "5. " << BOLD << "View Roommate Balances" << RESET << "\n";
    cout << "6. " << RED << BOLD << "Who Owes The Most? (Top Debtor)" << RESET << "\n";
    cout << "7. Exit\n";
    cout << "============================================\n";
    cout << "Select an option: ";
}

int main() {
    // Instantiate our core data structures
    MemberHash profiles;
    TransactionTimeline timeline;
    DebtHeap analytics(100);

    // TODO: You guys can work together to build out the Switch Statement for the menu!
    
    int choice = 0;
    while (choice != 7) {
        printMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Coming soon (Hash Map Integration)...\n";
                break;
            case 2:
                cout << "Coming soon (Timeline & Split Logic)...\n";
                break;
            // Add other cases
            case 7:
                cout << GREEN << "Exiting Kharcha Manager. Goodbye!\n" << RESET;
                break;
            default:
                cout << RED << "Invalid option!\n" << RESET;
        }
    }

    return 0;
}
