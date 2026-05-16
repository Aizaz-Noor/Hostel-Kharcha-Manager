#ifndef TRANSACTIONTIMELINE_H
#define TRANSACTIONTIMELINE_H

#include <iostream>
#include <string>

using namespace std;

// Node for the Doubly Linked List
struct TransactionNode {
    int transactionId;
    string description;
    double amount;
    string payerId;
    
    TransactionNode* prev;
    TransactionNode* next;

    TransactionNode(int id, string desc, double amt, string payer) 
        : transactionId(id), description(desc), amount(amt), payerId(payer), prev(NULL), next(NULL) {}
};

// Node for the Stack (Undo Engine)
struct StackNode {
    TransactionNode* transactionRef; // Pointer to the transaction to undo
    StackNode* next;

    StackNode(TransactionNode* ref) : transactionRef(ref), next(NULL) {}
};

class TransactionTimeline {
private:
    TransactionNode* head;
    TransactionNode* tail;
    StackNode* undoStackTop;
    int transactionCounter;

public:
    TransactionTimeline();
    ~TransactionTimeline(); // Delete both DLL and Stack nodes

    void logTransaction(string desc, double amount, string payerId);
    void undoLastTransaction();
    void printTimeline();
};

#endif // TRANSACTIONTIMELINE_H
