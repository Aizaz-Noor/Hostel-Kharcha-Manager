#ifndef MEMBERHASH_H
#define MEMBERHASH_H

#include <iostream>
#include <string>

using namespace std;

// Node for separate chaining in the hash map
struct MemberNode {
    string id;
    string name;
    string password;
    double balance; // Positive = owed money, Negative = owes money
    MemberNode* next;

    // Initialize member node data
    MemberNode(string _id, string _name, string _password, double _initialDeposit)
        : id(_id), name(_name), password(_password), balance(_initialDeposit), next(nullptr) {}
};

// Hash map storage for roommate profiles
class MemberHash {
private:
    static const int TABLE_SIZE = 100; // Total buckets in the hash table
    MemberNode* table[TABLE_SIZE];

    // Compute bucket index from ID by summing ASCII values
    int hashFunction(const string& id);

public:
    MemberHash();
    ~MemberHash(); // Destructor to free all allocated chain nodes

    // Add a new member. Returns true on success, false if ID exists.
    bool addMember(const string& id, const string& name, const string& password, double initialDeposit = 0.0);
    
    // Remove a member. Only allowed if balance is settled (0.0).
    bool removeMember(const string& id);
    
    // Find a member node by ID. Returns nullptr if not found.
    MemberNode* getMember(const string& id);
    
    // Verify member ID and password
    bool login(const string& id, const string& password);
    
    // Adjust balance of a member by the given amount
    void updateBalance(const string& id, double amount);
    
    // Print all registered members in a grid format
    void printAllMembers();
    
    // Collect all member pointers into a dynamic array for heap analytics
    MemberNode** getAllMembers(int& count);
};

#endif // MEMBERHASH_H
