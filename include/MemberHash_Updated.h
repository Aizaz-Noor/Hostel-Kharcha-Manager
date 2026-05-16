#ifndef MEMBERHASH_H
#define MEMBERHASH_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Node for the Chaining in the Hash Map
struct MemberNode {
    int id;
    string userName;
    string password;
    double balance; // Positive means they are owed money, negative means they owe money
    MemberNode* next;

    MemberNode(int _id, string user_name, string _password) { 
        id = _id;
        userName = user_name;
        password = _password;
        balance = 0.0;
        next = nullptr;
    }
};

class MemberHash {
private:
    static const int TABLE_SIZE = 100; // Fixed size array for the Hash Map
    MemberNode* table[TABLE_SIZE];
    int nextId;  // Auto-increment ID

    // Simple hash function to convert ID to array index
    int hashFunction(int id);

public:
    MemberHash();
    ~MemberHash(); // Destructor to manually delete all nodes and prevent memory leaks

    void addMember(const string& userName, const string& password);
    MemberNode* getMember(const string& userName);
    MemberNode* getMemberById(int id);
    void updateBalance(int id, double amount);
    void printAllMembers();
    
    // NEW: Get all members for DebtHeap testing
    vector<MemberNode*> getAllMembers();
};

#endif
