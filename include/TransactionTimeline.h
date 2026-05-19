#ifndef TRANSACTIONTIMELINE_H
#define TRANSACTIONTIMELINE_H

#include <iostream>
#include <string>

class MemberHash;

using namespace std;

// Structure to track balance updates for individuals during a split
struct BalanceChange {
    string memberId;
    double changeAmount;
};

// Node for the doubly linked transaction list
struct TransactionNode {
    int transactionId;
    string description;
    double amount;
    string payerId;
    
    BalanceChange* changes; // Array of updates made to individual balances
    int numChanges;         // Number of updates in the changes array
    
    TransactionNode* prev;
    TransactionNode* next;

    TransactionNode(int id, string desc, double amt, string payer, BalanceChange* chgs, int numChgs) 
        : transactionId(id), description(desc), amount(amt), payerId(payer), 
          changes(chgs), numChanges(numChgs), prev(NULL), next(NULL) {}
    
    ~TransactionNode() {
        if (changes != NULL) {
            delete[] changes;
        }
    }
};

// Node for the stack used to undo transactions
struct StackNode {
    TransactionNode* transactionRef; // Pointer to transaction node to undo
    StackNode* next;

    StackNode(TransactionNode* ref) : transactionRef(ref), next(NULL) {}
};

// Chronological transaction timeline manager
class TransactionTimeline {
private:
    TransactionNode* head;
    TransactionNode* tail;
    StackNode* undoStackTop;
    int transactionCounter;

public:
    TransactionTimeline();
    ~TransactionTimeline(); // Free both list and stack node memory

    // Add and split a new expense
    void logTransaction(string desc, double amount, string payerId, BalanceChange* changes, int numChanges);
    
    // Add transaction directly from file history
    void restoreTransaction(int id, string desc, double amount, string payerId, BalanceChange* changes, int numChanges);
    
    // Revert the latest transaction
    void undoLastTransaction(MemberHash* hash);
    
    // Print all transactions in a list
    void printTimeline();
    
    TransactionNode* getHead() const { return head; }
    int getCounter() const { return transactionCounter; }
    void setCounter(int val) { transactionCounter = val; }
};

#endif // TRANSACTIONTIMELINE_H
