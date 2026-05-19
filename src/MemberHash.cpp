#include "../include/MemberHash.h"
#include <iomanip>

// Set all hash table buckets to nullptr
MemberHash::MemberHash() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table[i] = nullptr;
    }
}

// Free all memory by deleting chained nodes
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

// Sum ASCII values of characters in ID and modulo by TABLE_SIZE
int MemberHash::hashFunction(const string& id) {
    int hash = 0;
    for (char c : id) {
        hash += (int)c;
    }
    return hash % TABLE_SIZE;
}

// Register a new member. Returns false if inputs are blank or ID is taken.
bool MemberHash::addMember(const string& id, const string& name, const string& password, double initialDeposit) {
    if (id.empty() || name.empty()) {
        return false;
    }

    int index = hashFunction(id);

    // Look for duplicate ID in the bucket chain
    MemberNode* current = table[index];
    while (current != nullptr) {
        if (current->id == id) {
            return false;
        }
        current = current->next;
    }

    // Insert new node at the head of the chain (constant time O(1))
    MemberNode* newNode = new MemberNode(id, name, password, initialDeposit);
    newNode->next = table[index];
    table[index] = newNode;
    return true;
}

// Delete a member node. Returns false if not found or if balance is not zero.
bool MemberHash::removeMember(const string& id) {
    if (id.empty()) return false;
    
    int index = hashFunction(id);
    MemberNode* current = table[index];
    MemberNode* prev = nullptr;

    while (current != nullptr) {
        if (current->id == id) {
            // Cannot delete if they still owe money or have a surplus
            if (current->balance != 0.0) {
                return false; 
            }
            
            // Unlink node from chain
            if (prev == nullptr) {
                table[index] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

// Search for a member by ID. Returns pointer or nullptr.
MemberNode* MemberHash::getMember(const string& id) {
    int index = hashFunction(id);

    MemberNode* current = table[index];
    while (current != nullptr) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// Validate credentials for portal entry
bool MemberHash::login(const string& id, const string& password) {
    MemberNode* member = getMember(id);
    if (member != nullptr && member->password == password) {
        return true;
    }
    return false;
}

// Add amount to the specified member's balance
void MemberHash::updateBalance(const string& id, double amount) {
    MemberNode* member = getMember(id);

    if (member == nullptr) {
        cout << "  ERROR: Member ID '" << id << "' not found in Hash Map.\n";
        return;
    }

    member->balance += amount;
}

// Print all registered members in a grid
void MemberHash::printAllMembers() {
    bool anyMember = false;

    cout << "\n";
    cout << "  +--------+--------------------+----------------+----------+\n";
    cout << "  | ID     | Name               | Balance (Rs.)  | Status   |\n";
    cout << "  +--------+--------------------+----------------+----------+\n";

    double totalFundRemaining = 0.0;

    for (int i = 0; i < TABLE_SIZE; ++i) {
        MemberNode* current = table[i];
        while (current != nullptr) {
            anyMember = true;
            totalFundRemaining += current->balance;

            string status;
            if (current->balance > 0)       status = "REMAINED";
            else if (current->balance < 0)  status = "OWES";
            else                            status = "SETTLED";

            // Limit display name length to fit column formatting
            string name = current->name;
            if (name.length() > 18) name = name.substr(0, 15) + "...";

            cout << "  | "
                 << left  << setw(6)  << current->id      << " | "
                 << left  << setw(18) << name              << " | Rs. "
                 << right << setw(9)  << fixed << setprecision(2) << current->balance << " | "
                 << left  << setw(8)  << status            << " |\n";

            current = current->next;
        }
    }

    if (!anyMember) {
        cout << "  |               No members registered yet.                |\n";
    }

    cout << "  +--------+--------------------+----------------+----------+\n\n";

    if (anyMember) {
        cout << "  Total Fund Cash Remaining : Rs. " 
             << fixed << setprecision(2) << totalFundRemaining << "\n";
    }
}

// Build a dynamic array of all member pointers for external processing (like heap sorting)
MemberNode** MemberHash::getAllMembers(int& outCount) {
    int count = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        MemberNode* curr = table[i];
        while (curr != nullptr) {
            count++;
            curr = curr->next;
        }
    }
    
    MemberNode** result = new MemberNode*[count];
    int index = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        MemberNode* curr = table[i];
        while (curr != nullptr) {
            result[index++] = curr;
            curr = curr->next;
        }
    }
    
    outCount = count;
    return result;
}
