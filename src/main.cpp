#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <windows.h>
#include "../include/MemberHash.h"
#include "../include/TransactionTimeline.h"
#include "../include/DebtHeap.h"

MemberHash* globalMemberHash = nullptr;

using namespace std;

// ── Windows console setup ────────────────────────────────────────────────────
void setupConsole() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

// ── ANSI color codes (prefixed with C_ to avoid Windows.h name conflicts) ────
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

// ── Panel width (visible chars) ───────────────────────────────────────────────
const int W = 54;

// ── Dividers ──────────────────────────────────────────────────────────────────
void divTop()  { cout << C_CYN << "  +" << string(W, '-') << "+" << C_RST << "\n"; }
void divMid()  { cout << C_CYN << "  |" << string(W, '-') << "|" << C_RST << "\n"; }
void divBot()  { cout << C_CYN << "  +" << string(W, '-') << "+" << C_RST << "\n"; }
void divLine() { cout << C_GRY << "  " << string(W + 2, '-') << C_RST << "\n"; }

// ── Center a string inside the panel ─────────────────────────────────────────
// visLen = actual visible character count (excludes ANSI codes in colorPrefix)
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

// ── Menu row: [ N ]  Label text               [Tag] ──────────────────────────
void menuRow(const string& num, const string& label, const string& tag,
             const string& numColor, const string& tagColor) {
    // Visible layout inside the | borders:
    //  "  [ N ]  <label padded to 30>  [tag padded to 10]  "  = 52 visible chars = W-2
    string visNum = "[ " + num + " ]";   // 5 chars
    int labelW    = 30;
    int tagW      = 10;

    string tagStr = tag.empty() ? string(tagW, ' ') : ("[" + tag + "]");
    int tagPad    = tagW - (int)tagStr.size();
    if (tagPad < 0) tagPad = 0;

    int labelPad  = labelW - (int)label.size();
    if (labelPad < 1) labelPad = 1;

    // visible width of content = 2 + 5 + 2 + label + labelPad + tagStr + tagPad + 2
    // = 2+5+2+30+1+10+0+2 = 52 = W-2   ✓ (left | adds 1, right | closes)
    cout << C_CYN << "  |" << C_RST << "  "
         << numColor << C_BD << visNum << C_RST << "  "
         << C_WHT << label << C_RST
         << string(labelPad, ' ')
         << tagColor << C_DIM << tagStr << C_RST
         << string(tagPad, ' ') << "  "
         << C_CYN << "|" << C_RST << "\n";
}

// ── Helpers ───────────────────────────────────────────────────────────────────
void clearScreen() { system("cls"); }

void pause() {
    cout << "\n" << C_DIM << C_CYN << "  Press Enter to continue..." << C_RST;
    cin.ignore();
    cin.get();
}

// Reproduce the same hash as MemberHash::hashFunction for display
int computeBucket(const string& id) {
    int h = 0;
    for (char c : id) h += (int)c;
    return h % 100;
}

void dsaLabel(const string& op, const string& detail = "") {
    cout << C_GRY << "  [DSA] " << C_YLW << op;
    if (!detail.empty()) cout << C_GRY << "  ->  " << C_DIM << detail;
    cout << C_RST << "\n";
}

// ── Splash Screen ─────────────────────────────────────────────────────────────
void printSplash() {
    clearScreen();
    cout << "\n";
    cout << C_CYN << "   +-+-+-+-+ +-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+\n";
    cout          << "   |H|O|S|T|E|L| |K|H|A|R|C|H|A| |M|A|N|A|G|E|R|\n";
    cout          << "   +-+-+-+-+ +-+-+-+-+-+-+-+ +-+-+-+-+-+-+-+\n" << C_RST;
    cout << "\n";
    cout << C_BD << C_WHT << "   Hostel Kharcha Manager" << C_RST
         << C_DIM         << "  v2.0  |  3rd Semester DSA Project\n" << C_RST;
    cout << "\n";
    cout << C_GRY;
    cout << "   Data Structures Implemented:\n";
    cout << "     [1] Hash Map with Chaining    ->  Member profiles & balances\n";
    cout << "     [2] Doubly Linked List         ->  Transaction timeline\n";
    cout << "     [3] Stack (Linked List based)  ->  Undo engine\n";
    cout << "     [4] Max-Heap                   ->  Debt priority queue\n";
    cout << C_RST << "\n";
    cout << C_DIM << C_GRY << "   Team: Basit Shahid  |  Aizaz Noor  |  Abdul\n" << C_RST;
    cout << "\n";
    divLine();
    cout << "\n" << C_DIM << C_CYN << "   Press Enter to start..." << C_RST;
    cin.get();
}

// ── Header (shown above every menu) ──────────────────────────────────────────
void printHeader(int members, int txns) {
    string title    = "HOSTEL KHARCHA MANAGER  v2.0";
    string subtitle = "Members: " + to_string(members)
                    + "   |   Transactions: " + to_string(txns);
    cout << "\n";
    divTop();
    centerRow(title,    (int)title.size(),    C_BD + C_WHT);
    centerRow(subtitle, (int)subtitle.size(), C_DIM + C_GRY);
    divBot();
}

// ── Main Menu ─────────────────────────────────────────────────────────────────
void printMenu(int members, int txns) {
    clearScreen();
    printHeader(members, txns);
    cout << "\n";
    divTop();
    centerRow("MAIN MENU", 9, C_BD + C_CYN);
    divMid();
    menuRow("1", "Add New Roommate",          "Hash Map",  C_GRN, C_GRN);
    menuRow("2", "Log a Shared Expense",      "DLL+Stack", C_YLW, C_YLW);
    menuRow("3", "Undo Last Transaction",     "Stack",     C_RED, C_RED);
    menuRow("4", "View Transaction Timeline", "DLL",       C_CYN, C_CYN);
    menuRow("5", "View Roommate Balances",    "Hash Map",  C_BLU, C_BLU);
    menuRow("6", "Who Owes The Most?",        "Max-Heap",  C_MGT, C_MGT);
    menuRow("7", "Exit",                      "",          C_GRY, C_GRY);
    divBot();
    cout << "\n  " << C_BD << "Your choice: " << C_RST;
}

// ── Section header for each feature screen ────────────────────────────────────
void sectionHeader(const string& title, const string& dsaNote, const string& color) {
    cout << "\n";
    divTop();
    centerRow(title,   (int)title.size(),   C_BD + color);
    centerRow(dsaNote, (int)dsaNote.size(), C_DIM + C_GRY);
    divBot();
    cout << "\n";
}

// ── Split an expense equally among all members ────────────────────────────────
void splitExpense(MemberHash& profiles, const string& payerId, double amount) {
    vector<MemberNode*> members = profiles.getAllMembers();
    int count = (int)members.size();
    if (count == 0) return;

    double share = amount / count;

    dsaLabel("Hash Map UPDATE x" + to_string(count),
             "debit Rs." + to_string((int)share) + " from each non-payer");

    // Payer gets back (amount - their own share)
    profiles.updateBalance(payerId, amount - share);

    // Everyone else is debited their share
    for (MemberNode* m : members) {
        if (m->id != payerId) {
            profiles.updateBalance(m->id, -share);
        }
    }

    cout << C_GRY
         << "\n  Per-person share : Rs. " << fixed << setprecision(2) << share
         << "\n  Payer credited   : Rs. " << fixed << setprecision(2) << (amount - share)
         << "\n" << C_RST;
}

// ══════════════════════════════════════════════════════════════════════════════
int main() {
    setupConsole();
    printSplash();

    MemberHash          profiles;
    globalMemberHash  = &profiles;
    TransactionTimeline timeline;
    DebtHeap            analytics(100);

    int members = 0, txns = 0, choice = 0;

    while (choice != 7) {
        printMenu(members, txns);
        cin >> choice;

        // ── [1] Add Roommate ─────────────────────────────────────────────────
        if (choice == 1) {
            clearScreen();
            sectionHeader("ADD NEW ROOMMATE", "Hash Map: hashFunction -> bucket -> insert at HEAD", C_GRN);

            string id, name;
            cout << "  Enter Member ID   (e.g. A01) : ";  cin >> id;
            cout << "  Enter Member Name            : ";  cin >> name;

            dsaLabel("Hash Map INSERT",
                     "bucket [" + to_string(computeBucket(id)) + "]  (O(1) head insert)");

            if (profiles.addMember(id, name)) {
                members++;
                cout << C_GRN << "\n  + '" << name << "'  (ID: " << id << ")  added!\n" << C_RST;
            } else {
                cout << C_RED << "\n  ! ID '" << id << "' already exists or input is empty.\n" << C_RST;
            }
            pause();
        }

        // ── [2] Log Expense ──────────────────────────────────────────────────
        else if (choice == 2) {
            clearScreen();
            sectionHeader("LOG SHARED EXPENSE", "DLL: append to tail   Stack: push node address", C_YLW);

            if (members == 0) {
                cout << C_RED << "  ! No members yet. Add roommates first (Option 1).\n" << C_RST;
                pause();
                continue;
            }

            string payerId, desc;
            double amount;

            cout << "  Who paid? (Member ID) : ";  cin >> payerId;
            if (!profiles.getMember(payerId)) {
                cout << C_RED << "\n  ! Member ID '" << payerId << "' not found.\n" << C_RST;
                pause();
                continue;
            }

            cin.ignore();
            cout << "  What was it for?     : ";  getline(cin, desc);
            cout << "  Amount (Rs.)         : ";  cin >> amount;
            if (amount <= 0) {
                cout << C_RED << "\n  ! Amount must be greater than 0.\n" << C_RST;
                pause();
                continue;
            }

            dsaLabel("DLL APPEND + STACK PUSH",
                     "Node #" + to_string(txns + 1) + " -> tail  (both O(1))");
            timeline.logTransaction(desc, amount, payerId);
            txns++;

            splitExpense(profiles, payerId, amount);

            cout << C_GRN << "\n  + Logged and split among " << members << " members.\n" << C_RST;
            pause();
        }

        // ── [3] Undo ─────────────────────────────────────────────────────────
        else if (choice == 3) {
            clearScreen();
            sectionHeader("UNDO LAST TRANSACTION", "Stack POP -> find node -> DLL DELETE", C_RED);
            dsaLabel("STACK POP -> DLL NODE DELETE", "O(1) pop, O(1) tail delete");
            timeline.undoLastTransaction();
            if (txns > 0) txns--;
            pause();
        }

        // ── [4] Timeline ─────────────────────────────────────────────────────
        else if (choice == 4) {
            clearScreen();
            sectionHeader("TRANSACTION TIMELINE", "Doubly Linked List  (head -> next -> ... -> tail)", C_CYN);
            dsaLabel("DLL TRAVERSE", "head -> [next] -> [next] -> tail");
            timeline.printTimeline();
            pause();
        }

        // ── [5] Balances ─────────────────────────────────────────────────────
        else if (choice == 5) {
            clearScreen();
            sectionHeader("ROOMMATE BALANCES", "Hash Map: scan all 100 buckets + chains", C_BLU);
            dsaLabel("Hash Map FULL SCAN", "O(TABLE_SIZE + n)");
            profiles.printAllMembers();
            pause();
        }

        // ── [6] Debt Report ──────────────────────────────────────────────────
        else if (choice == 6) {
            clearScreen();
            sectionHeader("DEBT PRIORITY REPORT", "Max-Heap: build from Hash Map -> extract-max", C_MGT);
            dsaLabel("HEAP BUILD", "insert each debtor -> heapifyUp  O(n log n)");
            analytics.buildHeapFromHashMap();
            dsaLabel("HEAP EXTRACT-MAX x N", "root = max debtor, heapifyDown after each");
            analytics.printSummaryReport();
            pause();
        }

        // ── [7] Exit ─────────────────────────────────────────────────────────
        else if (choice == 7) {
            clearScreen();
            cout << "\n";
            divTop();
            centerRow("Thanks for using Kharcha Manager!", 33, C_GRN);
            centerRow("Goodbye! :)", 11, C_DIM + C_GRN);
            divBot();
            cout << "\n";
        }

        else {
            cout << C_RED << "\n  ! Invalid option. Enter 1-7.\n" << C_RST;
            pause();
        }
    }

    return 0;
}