#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <windows.h>          // For SetConsoleOutputCP + ANSI terminal mode
#include "../include/MemberHash.h"
#include "../include/TransactionTimeline.h"
#include "../include/DebtHeap.h"

// ── Global pointer so DebtHeap can reach MemberHash ─────────────────────────
MemberHash* globalMemberHash = nullptr;

using namespace std;

// ── Enable UTF-8 + ANSI colors in Windows cmd ───────────────────────────────
void setupConsole() {
    SetConsoleOutputCP(65001);          // UTF-8 output
    SetConsoleCP(65001);                // UTF-8 input
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

// ── ANSI color codes ─────────────────────────────────────────────────────────
const string RESET   = "\033[0m";
const string BOLD    = "\033[1m";
const string DIM     = "\033[2m";
const string RED     = "\033[31m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string BLUE    = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN    = "\033[36m";
const string WHITE   = "\033[97m";
const string BRED    = "\033[91m";
const string BGREEN  = "\033[92m";
const string BYELLOW = "\033[93m";
const string BBLUE   = "\033[94m";
const string BMAGENTA= "\033[95m";
const string BCYAN   = "\033[96m";
const string BG_DARK = "\033[40m";

// ── Helpers ──────────────────────────────────────────────────────────────────
void clearScreen() { system("cls"); }

void pause() {
    cout << "\n" << DIM << CYAN << "  Press Enter to continue..." << RESET;
    cin.ignore();
    cin.get();
}

void printDivider(const string& color = CYAN, int width = 50) {
    cout << color << "  " << string(width, '-') << RESET << "\n";
}

void dsa(const string& label, const string& detail = "") {
    cout << DIM << YELLOW << "  ⟨DSA⟩ " << label;
    if (!detail.empty()) cout << " → " << detail;
    cout << RESET << "\n";
}

// ── Splash Screen ────────────────────────────────────────────────────────────
void printSplash() {
    clearScreen();
    cout << "\n\n";
    cout << BCYAN  << "       ██╗  ██╗██╗  ██╗███╗   ███╗\n";
    cout           << "       ██║  ██║██║ ██╔╝████╗ ████║\n";
    cout           << "       ███████║█████╔╝ ██╔████╔██║\n";
    cout           << "       ██╔══██║██╔═██╗ ██║╚██╔╝██║\n";
    cout           << "       ██║  ██║██║  ██╗██║ ╚═╝ ██║\n";
    cout           << "       ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝\n" << RESET;
    cout << "\n";
    cout << WHITE  << "    Hostel Kharcha Manager  " << DIM << "v2.0\n" << RESET;
    cout << DIM    << "    Track Expenses. Split Fairly. Owe Less.\n" << RESET;
    cout << "\n";
    cout << DIM << CYAN;
    cout << "    Data Structures Used:\n";
    cout << "      Hash Map    →  Member profiles & balances\n";
    cout << "      DLL + Stack →  Transaction timeline & undo\n";
    cout << "      Max-Heap    →  Debt priority queue\n";
    cout << RESET << "\n";
    cout << DIM << "    Team: Basit Shahid  |  Aizaz Noor  |  Abdul\n" << RESET;
    cout << "\n";
}

// ── Header (shown on every menu) ─────────────────────────────────────────────
void printHeader(int memberCount, int txCount) {
    cout << "\n";
    cout << BCYAN  << "  ┌─────────────────────────────────────────────────┐\n";
    cout           << "  │" << BOLD << WHITE << "        HOSTEL KHARCHA MANAGER  v2.0" << RESET << BCYAN << "          │\n";
    cout           << "  │" << DIM  << CYAN  << "   Members: " << memberCount
                   << "   │   Transactions: " << txCount
                   << "               " << RESET << BCYAN << " │\n";
    cout           << "  └─────────────────────────────────────────────────┘\n" << RESET;
}

// ── Main Menu ────────────────────────────────────────────────────────────────
void printMenu(int memberCount, int txCount) {
    clearScreen();
    printHeader(memberCount, txCount);
    cout << "\n";
    cout << CYAN   << "  ┌─────────────────────────────────────────────────┐\n";
    cout           << "  │                    MAIN MENU                    │\n";
    cout           << "  ├─────────────────────────────────────────────────┤\n";
    cout           << "  │                                                 │\n";
    cout << "  │   " << BGREEN   << "[ 1 ]  Add New Roommate              " << DIM << CYAN << "(Hash Map)" << RESET << CYAN << "   │\n";
    cout << "  │   " << BYELLOW  << "[ 2 ]  Log a Shared Expense        " << DIM << CYAN << "(DLL+Stack)" << RESET << CYAN << "   │\n";
    cout << "  │   " << BRED     << "[ 3 ]  Undo Last Transaction          " << DIM << CYAN << "(Stack)" << RESET << CYAN << "   │\n";
    cout << "  │   " << BCYAN    << "[ 4 ]  View Transaction Timeline        " << DIM << CYAN << "(DLL)" << RESET << CYAN << "   │\n";
    cout << "  │   " << BBLUE    << "[ 5 ]  View Roommate Balances        " << DIM << CYAN << "(HashMap)" << RESET << CYAN << "   │\n";
    cout << "  │   " << BMAGENTA << "[ 6 ]  Who Owes The Most?           " << DIM << CYAN << "(MaxHeap)" << RESET << CYAN << "   │\n";
    cout << "  │   " << WHITE    << "[ 7 ]  Exit                                   " << RESET << CYAN << "   │\n";
    cout           << "  │                                                 │\n";
    cout           << "  └─────────────────────────────────────────────────┘\n" << RESET;
    cout << "\n  " << BOLD << "Your choice: " << RESET;
}

// ── Bill-split helper ─────────────────────────────────────────────────────────
// After logging an expense, split it among all members:
//   payer    → credited (amount - their own share)
//   everyone else → debited (their share)
void splitExpense(MemberHash& profiles, const string& payerId, double amount) {
    vector<MemberNode*> members = profiles.getAllMembers();
    int count = (int)members.size();
    if (count == 0) return;

    double share = amount / count;

    dsa("Hash Map UPDATE × " + to_string(count), "split Rs." + to_string((int)amount) + " by " + to_string(count));

    // Payer gets back everything minus their own share
    profiles.updateBalance(payerId, amount - share);

    // Every other member owes their share
    for (MemberNode* m : members) {
        if (m->id != payerId) {
            profiles.updateBalance(m->id, -share);
        }
    }

    cout << "\n" << DIM << CYAN;
    cout << "  Per-person share: Rs. " << fixed << setprecision(2) << share << "\n";
    cout << "  Payer '" << payerId << "' credited: Rs. "
         << fixed << setprecision(2) << (amount - share) << "\n";
    cout << RESET;
}

// ═══════════════════════════════════════════════════════════════════════════════
int main() {
    setupConsole();
    printSplash();

    cout << CYAN << "  Press Enter to start..." << RESET;
    cin.get();

    MemberHash    profiles;
    globalMemberHash = &profiles;

    TransactionTimeline timeline;
    DebtHeap            analytics(100);

    int memberCount = 0;
    int txCount     = 0;
    int choice      = 0;

    while (choice != 7) {
        printMenu(memberCount, txCount);
        cin >> choice;

        // ── [1] Add Roommate ────────────────────────────────────────────────
        if (choice == 1) {
            clearScreen();
            cout << BGREEN << "\n  ┌──────────────────────────────────┐\n";
            cout           <<   "  │      ADD NEW ROOMMATE            │\n";
            cout           <<   "  └──────────────────────────────────┘\n" << RESET;

            string id, name;
            cout << "\n  Enter Member ID   (e.g. A01) : ";  cin >> id;
            cout << "  Enter Member Name            : ";  cin >> name;

            dsa("Hash Map INSERT", "hashFunction(\"" + id + "\") → bucket [" + to_string((int)(id[0]+id[1])%100) + "]");
            profiles.addMember(id, name);
            memberCount++;

            cout << BGREEN << "\n  ✔  '" << name << "' (ID: " << id << ") registered!\n" << RESET;
            pause();
        }

        // ── [2] Log Expense ─────────────────────────────────────────────────
        else if (choice == 2) {
            clearScreen();
            cout << BYELLOW << "\n  ┌──────────────────────────────────┐\n";
            cout            <<   "  │      LOG SHARED EXPENSE          │\n";
            cout            <<   "  └──────────────────────────────────┘\n" << RESET;

            if (memberCount == 0) {
                cout << BRED << "\n  ✘  No members yet! Add roommates first (Option 1).\n" << RESET;
                pause();
                continue;
            }

            string payerId, desc;
            double amount;

            cout << "\n  Who paid? (Member ID) : ";  cin >> payerId;

            // Validate payer exists
            if (profiles.getMember(payerId) == nullptr) {
                cout << BRED << "\n  ✘  Member ID '" << payerId << "' not found!\n" << RESET;
                pause();
                continue;
            }

            cin.ignore();
            cout << "  What was it for?     : ";  getline(cin, desc);
            cout << "  Amount (Rs.)         : ";  cin >> amount;

            if (amount <= 0) {
                cout << BRED << "\n  ✘  Amount must be greater than 0.\n" << RESET;
                pause();
                continue;
            }

            dsa("DLL APPEND + STACK PUSH", "tail->next = new Node(#" + to_string(txCount+1) + ")");
            timeline.logTransaction(desc, amount, payerId);
            txCount++;

            // Split the expense among all members
            splitExpense(profiles, payerId, amount);

            cout << BGREEN << "\n  ✔  Expense logged and split among " << memberCount << " members!\n" << RESET;
            pause();
        }

        // ── [3] Undo ────────────────────────────────────────────────────────
        else if (choice == 3) {
            clearScreen();
            cout << BRED << "\n  ┌──────────────────────────────────┐\n";
            cout         <<   "  │      UNDO LAST TRANSACTION       │\n";
            cout         <<   "  └──────────────────────────────────┘\n" << RESET;

            dsa("STACK POP → DLL DELETE");
            timeline.undoLastTransaction();
            if (txCount > 0) txCount--;
            pause();
        }

        // ── [4] Timeline ────────────────────────────────────────────────────
        else if (choice == 4) {
            clearScreen();
            cout << BCYAN << "\n  ┌──────────────────────────────────┐\n";
            cout          <<   "  │      TRANSACTION TIMELINE        │\n";
            cout          <<   "  │      Doubly Linked List          │\n";
            cout          <<   "  └──────────────────────────────────┘\n" << RESET;
            dsa("DLL TRAVERSE", "head → [next] → [next] → tail");
            timeline.printTimeline();
            pause();
        }

        // ── [5] Balances ────────────────────────────────────────────────────
        else if (choice == 5) {
            clearScreen();
            cout << BBLUE << "\n  ┌──────────────────────────────────┐\n";
            cout          <<   "  │      ROOMMATE BALANCES           │\n";
            cout          <<   "  │      Hash Map Full Scan          │\n";
            cout          <<   "  └──────────────────────────────────┘\n" << RESET;
            dsa("Hash Map SCAN", "iterate all " + to_string(100) + " buckets + chains");
            profiles.printAllMembers();
            pause();
        }

        // ── [6] Debt Report ─────────────────────────────────────────────────
        else if (choice == 6) {
            clearScreen();
            cout << BMAGENTA << "\n  ┌──────────────────────────────────┐\n";
            cout             <<   "  │      DEBT PRIORITY REPORT        │\n";
            cout             <<   "  │      Max-Heap (Priority Queue)   │\n";
            cout             <<   "  └──────────────────────────────────┘\n" << RESET;
            dsa("HEAP BUILD", "insert debtors from Hash Map → heapifyUp each");
            analytics.buildHeapFromHashMap();
            dsa("HEAP EXTRACT-MAX × N", "always removes root = biggest debtor");
            analytics.printSummaryReport();
            pause();
        }

        // ── [7] Exit ────────────────────────────────────────────────────────
        else if (choice == 7) {
            clearScreen();
            cout << BGREEN << "\n  ┌──────────────────────────────────┐\n";
            cout           <<   "  │   Thanks for using Kharcha Mgr! │\n";
            cout           <<   "  │         Goodbye!  :)             │\n";
            cout           <<   "  └──────────────────────────────────┘\n" << RESET << "\n";
        }

        else {
            cout << BRED << "\n  ✘  Invalid option. Please enter 1-7.\n" << RESET;
            pause();
        }
    }

    return 0;
}