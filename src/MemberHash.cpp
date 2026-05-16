#include "../include/MemberHash.h"
#include <iomanip>

// ── Constructor ──────────────────────────────────────────────────────────────
// DSA: Initialize every bucket to nullptr (empty chains)
MemberHash::MemberHash() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table[i] = nullptr;
    }
}

// ── Destructor ───────────────────────────────────────────────────────────────
// DSA: Walk every chain and delete each node to prevent memory leaks
MemberHash::~MemberHash() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        MemberNode* current = table[i];
        while (current != nullptr) {
            MemberNode* temp = current->next;
            delete current;
            current = temp;
        }
        table[i] = nullptr;
    }
}

// ── Hash Function ─────────────────────────────────────────────────────────────
// DSA: Sum ASCII values of every character in the ID, then mod by TABLE_SIZE
// This distributes keys across 0..99 with minimal collisions for short IDs.
int MemberHash::hashFunction(const string& id) {
    int hash = 0;
    for (char c : id) {
        hash += (int)c;
    }
    return hash % TABLE_SIZE;
}

// ── addMember ─────────────────────────────────────────────────────────────────
// DSA: Hash the ID -> go to that bucket -> check for duplicate -> insert at HEAD
// Inserting at HEAD is O(1) and avoids traversing the whole chain.
void MemberHash::addMember(const string& id, const string& name) {
    // Reject empty inputs
    if (id.empty() || name.empty()) {
        cout << "  ERROR: ID and Name cannot be empty.\n";
        return;
    }

    int index = hashFunction(id);

    // Check for duplicate ID (walk the chain)
    MemberNode* current = table[index];
    while (current != nullptr) {
        if (current->id == id) {
            cout << "  ERROR: Member with ID '" << id << "' already exists!\n";
            return;
        }
        current = current->next;
    }

    // Insert new node at the HEAD of the chain (O(1))
    MemberNode* newNode = new MemberNode(id, name);
    newNode->next = table[index];
    table[index] = newNode;

    cout << "  Member '" << name << "' (ID: " << id << ") registered.\n";
}

// ── getMember ─────────────────────────────────────────────────────────────────
// DSA: Hash the ID -> walk the chain at that bucket -> return matching node
// Average case O(1) — only degrades if many IDs hash to the same bucket.
MemberNode* MemberHash::getMember(const string& id) {
    int index = hashFunction(id);

    MemberNode* current = table[index];
    while (current != nullptr) {
        if (current->id == id) {
            return current; // Found
        }
        current = current->next;
    }

    return nullptr; // Not found
}

// ── updateBalance ─────────────────────────────────────────────────────────────
// DSA: Find the member via getMember(), then adjust their balance.
// Positive amount = member gains money. Negative = member spent / owes.
void MemberHash::updateBalance(const string& id, double amount) {
    MemberNode* member = getMember(id);

    if (member == nullptr) {
        cout << "  ERROR: Member ID '" << id << "' not found in Hash Map.\n";
        return;
    }

    member->balance += amount;
}

// ── printAllMembers ───────────────────────────────────────────────────────────
// DSA: Walk all TABLE_SIZE buckets; for each non-empty bucket, walk the chain.
void MemberHash::printAllMembers() {
    bool anyMember = false;

    cout << "\n";
    cout << "  +--------+--------------------+----------------+--------+\n";
    cout << "  | ID     | Name               | Balance (Rs.)  | Status |\n";
    cout << "  +--------+--------------------+----------------+--------+\n";

    for (int i = 0; i < TABLE_SIZE; ++i) {
        MemberNode* current = table[i];
        while (current != nullptr) {
            anyMember = true;

            string status;
            if (current->balance > 0)       status = "CREDIT";
            else if (current->balance < 0)  status = "OWES  ";
            else                            status = "EVEN  ";

            // Trim name if too long for column
            string name = current->name;
            if (name.length() > 18) name = name.substr(0, 15) + "...";

            cout << "  | "
                 << left  << setw(6)  << current->id      << " | "
                 << left  << setw(18) << name              << " | Rs. "
                 << right << setw(9)  << fixed << setprecision(2) << current->balance << " | "
                 << left  << setw(6)  << status            << " |\n";

            current = current->next;
        }
    }

    if (!anyMember) {
        cout << "  |              No members registered yet.              |\n";
    }

    cout << "  +--------+--------------------+----------------+--------+\n\n";
}

// ── getAllMembers ─────────────────────────────────────────────────────────────
// DSA: Collect every member pointer into a vector for the DebtHeap to process.
// This bridges the Hash Map and the Max-Heap modules.
vector<MemberNode*> MemberHash::getAllMembers() {
    vector<MemberNode*> result;

    for (int i = 0; i < TABLE_SIZE; ++i) {
        MemberNode* current = table[i];
        while (current != nullptr) {
            result.push_back(current);
            current = current->next;
        }
    }

    return result;
}
