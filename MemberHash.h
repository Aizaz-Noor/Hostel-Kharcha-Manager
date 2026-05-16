#ifndef MEMBERHASH_H
#define MEMBERHASH_H

#include <iostream>
#include <string>

using namespace std;

// Node for the Chaining in the Hash Map
struct MemberNode {
    int id;
    string userName;
    string password;
    double balance; // Positive means they are owed money, negative means they owe money
    MemberNode* next;

    MemberNode(string user_name, string _password) { 
        int id = 1000;
        string userName= user_name;
        string password = _password;
        double balance = 0.0;
        next = nullptr;
    }

};

class MemberHash {
private:
    static const int TABLE_SIZE = 100; // Fixed size array for the Hash Map
    MemberNode* table[TABLE_SIZE];

    // Simple hash function to convert string ID to array index
    int hashFunction(const string& userName);

public:
    MemberHash();
    ~MemberHash(); // Destructor to manually delete all nodes and prevent memory leaks

    void addMember(const string& userName, const string& password);
    MemberNode* getMember(const string& userName);
    void updateBalance(const string& id, double amount);
    bool login(const string& username, const string& password);
    void printAllMembers();
};

#endif 
