#include <iostream>
#include "../include/MemberHash.h"
#include "../include/TransactionTimeline.h"
#include "../include/DebtHeap.h"

using namespace std;

// ── Colors ──────────────────────────────────────────────
const string RESET   = "\033[0m";
const string BOLD    = "\033[1m";
const string RED     = "\033[31m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string BLUE    = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN    = "\033[36m";
const string WHITE   = "\033[37m";
const string BG_BLUE = "\033[44m";

// ── Helper: pause until user presses Enter ───────────────
void pause() {
    cout << "\n" << CYAN << "  Press Enter to continue..." << RESET;
    cin.ignore();
    cin.get();
}

// ── Helper: clear screen ─────────────────────────────────
void clearScreen() {
    system("cls");
}

// ── Header banner ────────────────────────────────────────
void printHeader() {
    cout << "\n";
    cout << CYAN << "  ╔══════════════════════════════════════════╗\n";
    cout <<         "  ║                                          ║\n";
    cout <<         "  ║   " << BOLD << WHITE << " HOSTEL KHARCHA MANAGER v1.0     " << RESET << CYAN << "  ║\n";
    cout <<         "  ║      " << YELLOW << "Track Expenses. Split Fairly.    " << CYAN << "  ║\n";
    cout <<         "  ║                                          ║\n";
    cout <<         "  ╚══════════════════════════════════════════╝\n" << RESET;
}

// ── Main menu ─────────────────────────────────────────────
void printMenu() {
    clearScreen();
    printHeader();

    cout << "\n";
    cout << CYAN  << "  ┌──────────────────────────────────────────┐\n";
    cout          << "  │            " << BOLD << WHITE << "MAIN MENU" << RESET << CYAN << "                      │\n";
    cout          << "  ├──────────────────────────────────────────┤\n";
    cout          << "  │                                          │\n";
    cout          << "  │   " << GREEN  << "[ 1 ]  Add New Roommate            " << CYAN << "    │\n";
    cout          << "  │   " << YELLOW << "[ 2 ]  Log a Shared Expense        " << CYAN << "    │\n";
    cout          << "  │   " << RED    << "[ 3 ]  Undo Last Transaction       " << CYAN << "    │\n";
    cout          << "  │   " << CYAN   << "[ 4 ]  View Transaction Timeline   " << CYAN << "    │\n";
    cout          << "  │   " << BLUE   << "[ 5 ]  View Roommate Balances      " << CYAN << "    │\n";
    cout          << "  │   " << MAGENTA<< "[ 6 ]  Who Owes The Most?          " << CYAN << "    │\n";
    cout          << "  │   " << WHITE  << "[ 7 ]  Exit                        " << CYAN << "    │\n";
    cout          << "  │                                          │\n";
    cout          << "  └──────────────────────────────────────────┘\n" << RESET;
    cout << "\n  " << BOLD << "Your choice: " << RESET;
}

int main() {
    MemberHash profiles;
    TransactionTimeline timeline;
    DebtHeap analytics(100);

    int choice = 0;

    while (choice != 7) {
        printMenu();
        cin >> choice;

        if (choice == 1) {
            clearScreen();
            cout << CYAN << "\n  ╔══════════════════════════════════╗\n";
            cout         << "  ║      ADD NEW ROOMMATE            ║\n";
            cout         << "  ╚══════════════════════════════════╝\n" << RESET;

            string id, name;
            cout << "\n  Enter Member ID   (e.g. A01) : ";
            cin >> id;
            cout << "  Enter Member Name            : ";
            cin >> name;

            profiles.addMember(id, name);

            cout << GREEN << "\n  ✔  Roommate '" << name << "' (ID: " << id << ") added!\n" << RESET;
            pause();
        }

        else if (choice == 2) {
            clearScreen();
            cout << YELLOW << "\n  ╔══════════════════════════════════╗\n";
            cout           << "  ║      LOG NEW EXPENSE             ║\n";
            cout           << "  ╚══════════════════════════════════╝\n" << RESET;

            string payerId, desc;
            double amount;

            cout << "\n  Who paid? (Member ID) : ";
            cin >> payerId;
            cin.ignore();
            cout << "  What was it for?     : ";
            getline(cin, desc);
            cout << "  Amount (Rs.)         : ";
            cin >> amount;

            timeline.logTransaction(desc, amount, payerId);
            pause();
        }

        else if (choice == 3) {
            clearScreen();
            cout << RED << "\n  ╔══════════════════════════════════╗\n";
            cout        << "  ║      UNDO LAST TRANSACTION       ║\n";
            cout        << "  ╚══════════════════════════════════╝\n" << RESET;

            timeline.undoLastTransaction();
            pause();
        }

        else if (choice == 4) {
            clearScreen();
            timeline.printTimeline();
            pause();
        }

        else if (choice == 5) {
            clearScreen();
            cout << BLUE << "\n  ╔══════════════════════════════════╗\n";
            cout         << "  ║      ROOMMATE BALANCES           ║\n";
            cout         << "  ╚══════════════════════════════════╝\n" << RESET;

            profiles.printAllMembers();
            pause();
        }

        else if (choice == 6) {
            clearScreen();
            cout << MAGENTA << "\n  ╔══════════════════════════════════╗\n";
            cout            << "  ║      TOP DEBTOR (MAX-HEAP)       ║\n";
            cout            << "  ╚══════════════════════════════════╝\n" << RESET;
            cout << "\n  This feature is handled by Role 3 (Debt Heap).\n";
            pause();
        }

        else if (choice == 7) {
            clearScreen();
            cout << GREEN << "\n  ╔══════════════════════════════════╗\n";
            cout          << "  ║   Thanks for using Kharcha Mgr! ║\n";
            cout          << "  ║         Goodbye!  :)             ║\n";
            cout          << "  ╚══════════════════════════════════╝\n" << RESET << "\n";
        }

        else {
            cout << RED << "\n  ✘  Invalid option. Please enter 1-7.\n" << RESET;
            pause();
        }
    }

    return 0;
}