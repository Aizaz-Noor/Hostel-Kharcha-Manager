#include "../include/TransactionTimeline.h"
#include "../include/MemberHash.h"
#include <iomanip>
#include <iostream>

// Initialize timeline fields
TransactionTimeline::TransactionTimeline() {
    head = nullptr;
    tail = nullptr;
    undoStackTop = nullptr;
    transactionCounter = 1;
}

// Free all memory in list and stack
TransactionTimeline::~TransactionTimeline() {
    TransactionNode* current = head;
    while (current != nullptr) {
        TransactionNode* temp = current->next;
        delete current;
        current = temp;
    }

    StackNode* stackCurrent = undoStackTop;
    while (stackCurrent != nullptr) {
        StackNode* temp = stackCurrent->next;
        delete stackCurrent;
        stackCurrent = temp;
    }
}

// Insert transaction directly into doubly linked list (used when loading saved files)
void TransactionTimeline::restoreTransaction(int id, string desc, double amount, string payerId, BalanceChange* changes, int numChanges) {
    TransactionNode* newNode = new TransactionNode(id, desc, amount, payerId, changes, numChanges);
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
}

// Add a transaction node to the end of list and push it onto the undo stack
void TransactionTimeline::logTransaction(string desc, double amount, string payerId, BalanceChange* changes, int numChanges) {
    TransactionNode* newNode = new TransactionNode(transactionCounter, desc, amount, payerId, changes, numChanges);
    transactionCounter++;

    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }

    // Push transaction node onto stack
    StackNode* stackNode = new StackNode(newNode);
    stackNode->next = undoStackTop;
    undoStackTop = stackNode;

    cout << "  Expense '" << desc << "' (Rs. " << amount << ") logged as Transaction #" << newNode->transactionId << ".\n";
}

// Pop latest transaction from stack, reverse balance changes, and remove it from list
void TransactionTimeline::undoLastTransaction(MemberHash* hash) {
    if (undoStackTop == nullptr) {
        cout << "  Nothing to undo — No transactions recorded yet!\n";
        return;
    }

    // Pop the top of stack
    StackNode* topStack = undoStackTop;
    TransactionNode* toRemove = topStack->transactionRef;
    undoStackTop = undoStackTop->next;
    delete topStack;

    cout << "  Undoing Transaction #" << toRemove->transactionId
         << " - " << toRemove->description << "\n";

    // Reverse changes applied to member balances
    if (hash != nullptr && toRemove->changes != nullptr) {
        for (int i = 0; i < toRemove->numChanges; i++) {
            // Apply negative of the change amount to reset
            hash->updateBalance(toRemove->changes[i].memberId, -(toRemove->changes[i].changeAmount));
        }
        cout << "  Successfully reversed balance changes.\n";
    }

    // Remove the node from the doubly linked list
    if (toRemove->prev == nullptr && toRemove->next == nullptr) {
        // Only one node in list
        head = nullptr;
        tail = nullptr;
    }
    else if (toRemove->prev == nullptr) {
        // Removing head node
        head = toRemove->next;
        head->prev = nullptr;
    }
    else if (toRemove->next == nullptr) {
        // Removing tail node
        tail = toRemove->prev;
        tail->next = nullptr;
    }
    else {
        // Removing middle node
        toRemove->prev->next = toRemove->next;
        toRemove->next->prev = toRemove->prev;
    }

    delete toRemove;
    cout << "  Undo complete!\n";
}

// Print transaction records from head to tail
void TransactionTimeline::printTimeline() {
    if (head == nullptr) {
        cout << "\n  No transactions recorded yet.\n";
        return;
    }

    cout << "\n";
    cout << "  +----+----------------------------+-------------+--------------+\n";
    cout << "  | #  | Description                | Amount      | Paid By      |\n";
    cout << "  +----+----------------------------+-------------+--------------+\n";

    TransactionNode* current = head;
    while (current != nullptr) {
        string desc  = current->description;
        string payer = current->payerId;
        
        // Truncate values if they exceed column widths
        if (desc.length()  > 26) desc  = desc.substr(0, 23) + "...";
        if (payer.length() > 12) payer = payer.substr(0, 12);

        cout << "  | "
             << setw(2)  << left << current->transactionId << " | "
             << setw(26) << left << desc                   << " | Rs. "
             << setw(7)  << right << fixed << setprecision(2) << current->amount << " | "
             << setw(12) << left << payer                  << " |\n";

        current = current->next;
    }

    cout << "  +----+----------------------------+-------------+--------------+\n";
}
