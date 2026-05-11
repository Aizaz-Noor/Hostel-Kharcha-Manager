#include "../include/DebtHeap.h"

// Role 3: Implement the Max-Heap Priority Queue here!
// Note: You must manually re-balance the array tree using heapify.

DebtHeap::DebtHeap(int cap) {
    capacity = cap;
    size = 0;
    heapArray = new DebtRecord[capacity];
}

DebtHeap::~DebtHeap() {
    delete[] heapArray;
}

void DebtHeap::heapifyUp(int index) {
    // TODO: Compare with parent and swap if larger
}

void DebtHeap::heapifyDown(int index) {
    // TODO: Compare with children and swap with largest
}

void DebtHeap::insertDebt(string id, string name, double amount) {
    // TODO: Insert at end and heapifyUp
}

DebtRecord DebtHeap::extractMaxDebtor() {
    // TODO: Return root, move last element to root, heapifyDown
    return DebtRecord();
}

void DebtHeap::buildHeapFromHashMap() {
    // TODO: Pull data from the Hash Map and insert debts
}

void DebtHeap::printSummaryReport() {
    // TODO: Extract everyone and print the final financial summary
}
