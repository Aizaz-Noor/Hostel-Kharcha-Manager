#include "../include/TransactionTimeline.h"
#include <iomanip>

// Constructor - set everything to null/zero at start
TransactionTimeline::TransactionTimeline() {
    head = nullptr;
    tail = nullptr;
    undoStackTop = nullptr;
    transactionCounter = 1;
}

// Destructor - free all DLL nodes, then free all stack nodes
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

// Add a new transaction to the timeline
// DSA: Append to tail of Doubly Linked List, then Push onto Stack
void TransactionTimeline::logTransaction(string desc, double amount, string payerId) {

    // Create a new node for this transaction
    TransactionNode* newNode = new TransactionNode(transactionCounter, desc, amount, payerId);
    transactionCounter++;

    // Add to the end of the Doubly Linked List
    if (head == nullptr) {
        // List is empty, this node is both head and tail
        head = newNode;
        tail = newNode;
    } else {
        // Connect new node after the current tail
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }

    // Push this node's address onto the undo stack
    StackNode* stackNode = new StackNode(newNode);
    stackNode->next = undoStackTop;
    undoStackTop = stackNode;

    cout << "Transaction #" << newNode->transactionId << " added successfully!\n";
}

// Remove the last transaction (undo)
// DSA: Pop from Stack to find which node to remove, then remove it from DLL
void TransactionTimeline::undoLastTransaction() {

    // Check if there is anything to undo
    if (undoStackTop == nullptr) {
        cout << "Nothing to undo!\n";
        return;
    }

    // Pop from the stack - this tells us which transaction to remove
    StackNode* topStack = undoStackTop;
    TransactionNode* toRemove = topStack->transactionRef;
    undoStackTop = undoStackTop->next;
    delete topStack;

    cout << "Undoing Transaction #" << toRemove->transactionId
         << " - " << toRemove->description << "\n";

    // Remove the node from the Doubly Linked List
    // We have 4 cases depending on where the node is

    if (toRemove->prev == nullptr && toRemove->next == nullptr) {
        // Only node in the list
        head = nullptr;
        tail = nullptr;
    }
    else if (toRemove->prev == nullptr) {
        // Node is at the head
        head = toRemove->next;
        head->prev = nullptr;
    }
    else if (toRemove->next == nullptr) {
        // Node is at the tail
        tail = toRemove->prev;
        tail->next = nullptr;
    }
    else {
        // Node is in the middle
        toRemove->prev->next = toRemove->next;
        toRemove->next->prev = toRemove->prev;
    }

    delete toRemove;
    cout << "Undo successful!\n";
}

// Print all transactions from first to last
// DSA: Traverse DLL using next pointers (head to tail)
void TransactionTimeline::printTimeline() {

    if (head == nullptr) {
        cout << "\n  No transactions recorded yet.\n";
        return;
    }

    // Print table header
    cout << "\n";
    cout << "  +----+----------------------------+-------------+--------------+\n";
    cout << "  | #  | Description                | Amount      | Paid By      |\n";
    cout << "  +----+----------------------------+-------------+--------------+\n";

    // Walk the DLL from head to tail using next pointers
    TransactionNode* current = head;
    while (current != nullptr) {

        // Trim description and payer if too long
        string desc  = current->description;
        string payer = current->payerId;
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
    cout << "\n  Direction: HEAD --> [next] --> [next] --> TAIL\n";
}
