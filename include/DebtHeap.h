#ifndef DEBTHEAP_H
#define DEBTHEAP_H

#include <iostream>
#include <string>

using namespace std;

class MemberHash;
struct MemberNode;
extern MemberHash* globalMemberHash;

// Structure for tracking a roommate's debt amount
struct DebtRecord {
    string memberId;
    string memberName;
    double amountOwed; // Positive value representing the amount the roommate owes

    DebtRecord() : memberId(""), memberName(""), amountOwed(0.0) {}
    DebtRecord(string id, string name, double amt) : memberId(id), memberName(name), amountOwed(amt) {}
};

// Max-heap to sort debtors by amount owed
class DebtHeap {
private:
    DebtRecord* heapArray; // Array for binary heap tree
    int capacity;
    int size;

    // Helper functions for binary heap indices
    int parent(int i)     { return (i - 1) / 2; }
    int leftChild(int i)  { return (2 * i) + 1; }
    int rightChild(int i) { return (2 * i) + 2; }

    // Shift elements up/down to keep heap properties intact
    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    DebtHeap(int cap);
    ~DebtHeap();

    // Disable copying and assignment to avoid memory leaks
    DebtHeap(const DebtHeap&) = delete;
    DebtHeap& operator=(const DebtHeap&) = delete;

    // Insert a debt entry
    void insertDebt(string id, string name, double amount);
    
    // Extract and return the largest debtor
    DebtRecord extractMaxDebtor();
    
    // Read balances from MemberHash and build the heap
    void buildHeapFromHashMap();
    
    // Print all debtors sorted by size of debt
    void printSummaryReport();
};

#endif // DEBTHEAP_H