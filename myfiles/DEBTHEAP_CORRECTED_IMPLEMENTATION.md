# DebtHeap.cpp - CORRECTED Implementation (Matches Header Signature)

Use this CORRECTED version that matches your header file exactly.

---

```cpp
#include "../include/DebtHeap.h"
#include "../include/MemberHash.h"  // Add this include

// Global reference to MemberHash (will be set by main.cpp)
// Alternative: Pass MemberHash as parameter in main.cpp
extern MemberHash* globalMemberHash;

DebtHeap::DebtHeap(int cap) {
    capacity = cap;
    size = 0;
    heapArray = new DebtRecord[capacity];
}

DebtHeap::~DebtHeap() {
    delete[] heapArray;
}

// ============================================================================
// heapifyUp: Move element UP to maintain max-heap property
// Called after inserting at end
// ============================================================================
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

// ============================================================================
// heapifyDown: Move element DOWN to maintain max-heap property
// Called after extracting root
// ============================================================================
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

// ============================================================================
// insertDebt: Add a new debtor to the heap
// ============================================================================
void DebtHeap::insertDebt(string id, string name, double amount) {
    if (size >= capacity) {
        cout << "ERROR: Heap is full! Cannot insert more debtors.\n";
        return;
    }
    
    heapArray[size] = DebtRecord(id, name, amount);
    heapifyUp(size);
    size++;
}

// ============================================================================
// extractMaxDebtor: Remove and return member with highest debt (root)
// ============================================================================
DebtRecord DebtHeap::extractMaxDebtor() {
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

// ============================================================================
// buildHeapFromHashMap: Populate heap from MemberHash data
// NO PARAMETERS - Uses global MemberHash pointer
// ============================================================================
void DebtHeap::buildHeapFromHashMap() {
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

// ============================================================================
// printSummaryReport: Extract all debtors and print in order
// ============================================================================
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
```

---

## Key Changes from Previous Version

### **Change 1: buildHeapFromHashMap() Signature**
```cpp
// OLD (wrong signature):
void DebtHeap::buildHeapFromHashMap(MemberHash& memberData)

// NEW (correct - matches header):
void DebtHeap::buildHeapFromHashMap()
```

### **Change 2: Access to MemberHash**
```cpp
// NEW: Use global pointer instead of parameter
extern MemberHash* globalMemberHash;

void DebtHeap::buildHeapFromHashMap() {
    if (globalMemberHash == nullptr) { ... }
    vector<MemberNode*> allMembers = globalMemberHash->getAllMembers();
}
```

---

## What You Need in main.cpp

Add this **before** using DebtHeap:

```cpp
#include "include/DebtHeap.h"
#include "include/MemberHash.h"

// Declare global
MemberHash* globalMemberHash = nullptr;

int main() {
    DebtHeap analytics(100);
    MemberHash profiles(10);
    
    // Set global reference for DebtHeap to use
    globalMemberHash = &profiles;
    
    // Now when you call:
    analytics.buildHeapFromHashMap();
    // It will automatically use globalMemberHash
    
    return 0;
}
```

---

## Alternative Approach (Better Design)

If you want to avoid globals, ask **Member 1** to add a setter method to MemberHash:

```cpp
// In MemberHash.h
class MemberHash {
private:
    MemberNode** table;
    static MemberHash* instance;  // singleton pattern
    
public:
    static MemberHash* getInstance() { return instance; }
    static void setInstance(MemberHash* inst) { instance = inst; }
    // ...
};

// In DebtHeap.cpp
void DebtHeap::buildHeapFromHashMap() {
    MemberHash* memberData = MemberHash::getInstance();
    if (memberData == nullptr) {
        cout << "ERROR: MemberHash not set!\n";
        return;
    }
    // ... rest of code
}
```

---

## Summary

| Aspect | Status | Notes |
|--------|--------|-------|
| **Helper functions** | ✓ Already defined | No changes needed |
| **Signature match** | ✓ Now corrected | buildHeapFromHashMap() takes NO parameters |
| **Global access** | ✓ Implemented | Uses `extern MemberHash* globalMemberHash` |
| **Future updates** | ✗ None needed | Code is final once global is set in main.cpp |

**Use this corrected version!** ✓
