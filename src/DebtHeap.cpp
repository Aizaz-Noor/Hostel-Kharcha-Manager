#include "../include/DebtHeap.h"
#include "../include/MemberHash.h"

DebtHeap::DebtHeap(int cap) {
    capacity = cap;
    size = 0;
    heapArray = new DebtRecord[capacity];
}

DebtHeap::~DebtHeap() {
    delete[] heapArray;
}

// Push the element up the tree if it is larger than its parent
void DebtHeap::heapifyUp(int index) {
     while (index > 0) {
        int parentIdx = parent(index);
        
        if (heapArray[index].amountOwed > heapArray[parentIdx].amountOwed) {
            DebtRecord temp = heapArray[index];
            heapArray[index] = heapArray[parentIdx];
            heapArray[parentIdx] = temp;
            
            index = parentIdx;
        } else {
            break;
        }
    }
}

// Push the element down the tree to its correct spot
void DebtHeap::heapifyDown(int index) {
    while (true) {
        int leftIdx = leftChild(index);
        int rightIdx = rightChild(index);
        int largestIdx = index;
        
        if (leftIdx < size && 
            heapArray[leftIdx].amountOwed > heapArray[largestIdx].amountOwed) {
            largestIdx = leftIdx;
        }
        
        if (rightIdx < size && 
            heapArray[rightIdx].amountOwed > heapArray[largestIdx].amountOwed) {
            largestIdx = rightIdx;
        }
        
        if (largestIdx != index) {
            DebtRecord temp = heapArray[index];
            heapArray[index] = heapArray[largestIdx];
            heapArray[largestIdx] = temp;
            
            index = largestIdx;
        } else {
            break;
        }
    }
}

// Add a debtor record and restore heap property
void DebtHeap::insertDebt(string id, string name, double amount) {
    if (size >= capacity) {
        cout << "  ! Maximum limit reached! Cannot process more debtors.\n";
        return;
    }
    
    heapArray[size] = DebtRecord(id, name, amount);
    size++;
    heapifyUp(size - 1);
}

// Remove and return the debtor who owes the most
DebtRecord DebtHeap::extractMaxDebtor() {
     if (size <= 0) {
        cout << "  ! No debtors found.\n";
        return DebtRecord();
    }
    
    DebtRecord maxDebtor = heapArray[0];
    heapArray[0] = heapArray[size - 1];
    size--;
    
    if (size > 0) {
        heapifyDown(0);
    }
    
    return maxDebtor;
}

// Scan the roommate hash map and build a heap from members with negative balances
void DebtHeap::buildHeapFromHashMap() {
    size = 0;
    
    if (globalMemberHash == nullptr) {
        cout << "  ! System not initialized! Cannot generate report.\n";
        return;
    }
    
    int count = 0;
    MemberNode** allMembers = globalMemberHash->getAllMembers(count);
    
    for (int i = 0; i < count; i++) {
        MemberNode* member = allMembers[i];
        if (member != nullptr && member->balance < 0) {
            double amountOwed = -(member->balance);
            insertDebt(member->id, member->name, amountOwed);
        }
    }
    
    delete[] allMembers;
    cout << "  Generated report for " << size << " debtors.\n";
}

// Extract all debtors from the heap one by one to print a sorted list
void DebtHeap::printSummaryReport() {
    cout << "\n" << string(50, '=') << "\n";
    cout << "              DEBT SUMMARY REPORT\n";
    cout << string(50, '=') << "\n\n";
    
    if (size == 0) {
        cout << "Great news! No one owes any money.\n";
        cout << string(50, '=') << "\n\n";
        return;
    }
    
    cout << "Members sorted by amount owed (highest first):\n\n";
    
    int rank = 1;
    double totalDebt = 0;
    
    while (size > 0) {
        DebtRecord debtor = extractMaxDebtor();
        
        cout << rank << ". " << debtor.memberName 
             << " (" << debtor.memberId << "): "
             << "Rs. " << debtor.amountOwed << "\n";
        
        totalDebt += debtor.amountOwed;
        rank++;
    }
    
    cout << "\n" << string(50, '=') << "\n";
    cout << "Total Amount Owed: Rs. " << totalDebt << "\n";
    cout << string(50, '=') << "\n\n";
}
