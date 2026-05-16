#include "../include/MemberHash.h"

// Role 1: Implement the Hash Map logic here!
// Note: You must not use any STL containers.

MemberHash::MemberHash() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table[i] = nullptr;
    }
}

MemberHash::~MemberHash() {
    // TODO: Write destructor to delete all chained nodes and prevent memory leaks
}

int MemberHash::hashFunction(const string& id) {
    // TODO: Implement a good string hashing algorithm
    int hash = 0;
    for (char c : id) {
        hash += c;
    }
    return hash % TABLE_SIZE;
}

void MemberHash::addMember(const string& id, const string& name) {
    // TODO: Implement insertion with collision handling (chaining)
}

MemberNode* MemberHash::getMember(const string& id) {
    // TODO: Implement O(1) lookup
    return nullptr;
}

void MemberHash::updateBalance(const string& id, double amount) {
    // TODO: Find member and update balance
}

void MemberHash::printAllMembers() {
    // TODO: Iterate through the table and print members
}
