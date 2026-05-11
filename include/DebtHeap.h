#ifndef DEBTHEAP_H
#define DEBTHEAP_H

#include <iostream>
#include <string>

using namespace std;

// Structure to hold a member's debt info for the priority queue
struct DebtRecord {
    string memberId;
    string memberName;
    double amountOwed; // Only members in debt (negative balance) go here
    
    DebtRecord() : memberId(""), memberName(""), amountOwed(0.0) {}
    DebtRecord(string id, string name, double amt) : memberId(id), memberName(name), amountOwed(amt) {}
};

class DebtHeap {
private:
    DebtRecord* heapArray; // Dynamic array for the Max-Heap
    int capacity;
    int size;

    // Helper functions for math
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return (2 * i) + 1; }
    int rightChild(int i) { return (2 * i) + 2; }

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    DebtHeap(int cap);
    ~DebtHeap();

    void insertDebt(string id, string name, double amount);
    DebtRecord extractMaxDebtor();
    void buildHeapFromHashMap(); // Called to generate the priority queue
    void printSummaryReport();
};

#endif // DEBTHEAP_H
