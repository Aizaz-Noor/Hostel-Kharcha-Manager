#include "../include/DebtHeap.h"
#include "../include/MemberHash.h"

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
    //Compare with parent and swap if larger
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

void DebtHeap::heapifyDown(int index) {
    //Compare with children and swap with largest
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

void DebtHeap::insertDebt(string id, string name, double amount) {
    // Insert at end and heapifyUp
    if (size >= capacity) {
        cout << "ERROR: Heap is full! Cannot insert more debtors.\n";
        return;
    }
    
    heapArray[size] = DebtRecord(id, name, amount);
    size++;              // Claim the array slot before heapifying
    heapifyUp(size - 1); // Restore heap property on the newly added element

}

DebtRecord DebtHeap::extractMaxDebtor() {
    // Return root, move last element to root, heapifyDown
     if (size <= 0) {
        cout << "ERROR: Heap is empty! No debtors to extract.\n";
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

void DebtHeap::buildHeapFromHashMap() {
    //Pull data from the Hash Map and insert debts
 // Clear existing heap
    size = 0;
    
    // Check if global MemberHash is available
    if (globalMemberHash == nullptr) {
        cout << "ERROR: MemberHash not initialized! Cannot build heap.\n";
        return;
    }
    
    // Get all members from MemberHash
    vector<MemberNode*> allMembers = globalMemberHash->getAllMembers();
    
    // Add each debtor to heap (only members with negative balance)
    for (MemberNode* member : allMembers) {
        if (member != nullptr && member->balance < 0) {
            // Convert negative balance to positive amount owed
            double amountOwed = -(member->balance);
            insertDebt(member->id, member->name, amountOwed);
        }
    }
    
    cout << "Heap built with " << size << " debtors.\n";
}

void DebtHeap::printSummaryReport() {
    //  Extract everyone and print the final financial summary
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
