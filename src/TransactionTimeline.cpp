#include "../include/TransactionTimeline.h"

// Role 2: Implement the Doubly Linked List and Stack logic here!
// Note: Be extremely careful with next and prev pointers.

TransactionTimeline::TransactionTimeline() {
    head = nullptr;
    tail = nullptr;
    undoStackTop = nullptr;
    transactionCounter = 1;
}

TransactionTimeline::~TransactionTimeline() {
    // TODO: Safely delete all nodes in the DLL and the Stack
}

void TransactionTimeline::logTransaction(string desc, double amount, string payerId) {
    // TODO: Append to the Doubly Linked List
    // TODO: Push the new transaction onto the undoStackTop
}

void TransactionTimeline::undoLastTransaction() {
    // TODO: Pop from stack
    // TODO: Remove that node from the DLL
    // TODO: Update balances!
}

void TransactionTimeline::printTimeline() {
    // TODO: Traverse DLL from head to tail and print beautifully
}
