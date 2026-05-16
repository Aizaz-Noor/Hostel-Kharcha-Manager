#ifndef MEMBERHASH_H
#define MEMBERHASH_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Node for the Chaining in the Hash Map
struct MemberNode {
    string id;
    string name;
    double balance; // Positive = owed money by others. Negative = owes money.
    MemberNode* next;

    // Fixed constructor: uses initializer list so struct members are set correctly
    MemberNode(string _id, string _name)
        : id(_id), name(_name), balance(0.0), next(nullptr) {}
};

class MemberHash {
private:
    static const int TABLE_SIZE = 100; // Fixed size array for the Hash Map
    MemberNode* table[TABLE_SIZE];

    // Simple hash function: sums ASCII values of each character
    int hashFunction(const string& id);

public:
    MemberHash();
    ~MemberHash(); // Destructor to delete all chained nodes (no memory leaks)

    bool addMember(const string& id, const string& name);  // returns true on success
    MemberNode* getMember(const string& id);
    void updateBalance(const string& id, double amount);
    void printAllMembers();
    vector<MemberNode*> getAllMembers(); // Used by DebtHeap to build the priority queue
};

#endif // MEMBERHASH_H
