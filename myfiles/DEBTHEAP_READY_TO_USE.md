# DebtHeap.cpp - Complete Ready-to-Use Implementation

Copy and paste this entire implementation into your src/DebtHeap.cpp file.

---

```cpp
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

// ============================================================================
// heapifyUp: Move element UP to maintain max-heap property
// Called after inserting at end
// ============================================================================
void DebtHeap::heapifyUp(int index) {
    // Keep moving up while element is larger than parent
    while (index > 0) {
        int parentIdx = parent(index);
        
        // If current element is larger than parent, swap them
        if (heapArray[index].amountOwed > heapArray[parentIdx].amountOwed) {
            // Swap
            DebtRecord temp = heapArray[index];
            heapArray[index] = heapArray[parentIdx];
            heapArray[parentIdx] = temp;
            
            // Continue checking up the tree
            index = parentIdx;
        } else {
            // Heap property satisfied
            break;
        }
    }
}

// ============================================================================
// heapifyDown: Move element DOWN to maintain max-heap property
// Called after extracting root
// ============================================================================
void DebtHeap::heapifyDown(int index) {
    // Keep moving down while element is smaller than one of its children
    while (true) {
        int leftIdx = leftChild(index);
        int rightIdx = rightChild(index);
        int largestIdx = index;
        
        // Find the largest among parent and its children
        if (leftIdx < size && 
            heapArray[leftIdx].amountOwed > heapArray[largestIdx].amountOwed) {
            largestIdx = leftIdx;
        }
        
        if (rightIdx < size && 
            heapArray[rightIdx].amountOwed > heapArray[largestIdx].amountOwed) {
            largestIdx = rightIdx;
        }
        
        // If largest is not current element, swap and continue down
        if (largestIdx != index) {
            DebtRecord temp = heapArray[index];
            heapArray[index] = heapArray[largestIdx];
            heapArray[largestIdx] = temp;
            
            index = largestIdx;
        } else {
            // Heap property satisfied
            break;
        }
    }
}

// ============================================================================
// insertDebt: Add a new debtor to the heap
// ============================================================================
void DebtHeap::insertDebt(string id, string name, double amount) {
    // Check if heap is full
    if (size >= capacity) {
        cout << "ERROR: Heap is full! Cannot insert more debtors.\n";
        return;
    }
    
    // Create new debt record and add at end
    heapArray[size] = DebtRecord(id, name, amount);
    
    // Restore heap property by moving up
    heapifyUp(size);
    
    // Increment size
    size++;
}

// ============================================================================
// extractMaxDebtor: Remove and return member with highest debt (root)
// ============================================================================
DebtRecord DebtHeap::extractMaxDebtor() {
    // Check if heap is empty
    if (size <= 0) {
        cout << "ERROR: Heap is empty! No debtors to extract.\n";
        return DebtRecord();
    }
    
    // Save the root (person with most debt)
    DebtRecord maxDebtor = heapArray[0];
    
    // Move last element to root
    heapArray[0] = heapArray[size - 1];
    
    // Decrease size
    size--;
    
    // Restore heap property if heap is not empty
    if (size > 0) {
        heapifyDown(0);
    }
    
    // Return the maximum debtor
    return maxDebtor;
}

// ============================================================================
// buildHeapFromHashMap: Populate heap from MemberHash data
// Only includes members with negative balance (they owe money)
// ============================================================================
void DebtHeap::buildHeapFromHashMap(MemberHash& memberData) {
    // Clear existing heap
    size = 0;
    
    // Get all members from MemberHash
    // IMPORTANT: This assumes Member 1 implements getAllMembers()
    vector<MemberNode*> allMembers = memberData.getAllMembers();
    
    // For each member, if they owe money, add to heap
    for (MemberNode* member : allMembers) {
        if (member->balance < 0) {
            // They owe money (negative balance)
            // Convert to positive amount and insert
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
    
    // Check if heap is empty
    if (size == 0) {
        cout << "Great news! No one owes any money.\n";
        cout << string(50, '=') << "\n\n";
        return;
    }
    
    cout << "Members sorted by amount owed (highest first):\n\n";
    
    int rank = 1;
    double totalDebt = 0;
    
    // Extract and print all debtors
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

## Key Points About This Implementation

1. **heapifyUp()**
   - Moves element UP when it violates max-heap property
   - Used after inserting at end
   - O(log n) time complexity

2. **heapifyDown()**
   - Moves element DOWN when it violates max-heap property
   - Used after removing root
   - O(log n) time complexity

3. **insertDebt()**
   - Adds new debtor to end, then heapifyUp
   - O(log n) time complexity
   - Checks capacity to prevent overflow

4. **extractMaxDebtor()**
   - Returns root (max debtor)
   - Moves last element to root, then heapifyDown
   - O(log n) time complexity
   - Handles empty heap case

5. **buildHeapFromHashMap()**
   - Depends on Member 1 implementing `getAllMembers()` method
   - Only includes members with negative balance
   - O(n log n) time complexity (n inserts, each O(log n))

6. **printSummaryReport()**
   - Extracts all debtors (destructive)
   - Prints in order with formatting
   - Shows total debt at end

---

## Important Dependency

For `buildHeapFromHashMap()` to work, **Member 1 must implement this in MemberHash.h:**

```cpp
class MemberHash {
public:
    // ... other methods ...
    
    vector<MemberNode*> getAllMembers();  // ← ADD THIS
};
```

And in MemberHash.cpp:
```cpp
vector<MemberNode*> MemberHash::getAllMembers() {
    vector<MemberNode*> result;
    for (int i = 0; i < TABLE_SIZE; i++) {
        MemberNode* current = table[i];
        while (current != nullptr) {
            result.push_back(current);
            current = current->next;
        }
    }
    return result;
}
```

---

## How to Use in main.cpp

```cpp
// In your main.cpp, when choice == 6 (Who Owes Most?):

// Step 1: Build heap from current balances
analytics.buildHeapFromHashMap(profiles);

// Step 2: Print the report
analytics.printSummaryReport();
```

---

## Testing This Implementation

```cpp
// Simple test code to verify your implementation works:

int main() {
    DebtHeap heap(10);
    
    cout << "Test 1: Insert test data\n";
    heap.insertDebt("A01", "Alice", 2000);
    heap.insertDebt("B01", "Bob", 1500);
    heap.insertDebt("C01", "Charlie", 2500);
    heap.insertDebt("D01", "David", 800);
    
    cout << "\nTest 2: Extract in order (should be highest first)\n";
    cout << "1st: " << heap.extractMaxDebtor().memberName << " (should be Charlie)\n";
    cout << "2nd: " << heap.extractMaxDebtor().memberName << " (should be Alice)\n";
    cout << "3rd: " << heap.extractMaxDebtor().memberName << " (should be Bob)\n";
    cout << "4th: " << heap.extractMaxDebtor().memberName << " (should be David)\n";
    
    return 0;
}

// Expected Output:
// 1st: Charlie
// 2nd: Alice
// 3rd: Bob
// 4th: David
```

---

## Common Issues & Solutions

### Issue 1: "heapArray[index].amountOwed" undefined
**Solution:** Make sure you're comparing the right field (amountOwed, not memberName)

### Issue 2: Index out of bounds
**Solution:** Always check `index < size` before accessing children

### Issue 3: Infinite loop in heapifyUp/heapifyDown
**Solution:** Make sure you have proper loop termination conditions

### Issue 4: buildHeapFromHashMap() doesn't compile
**Solution:** Wait for Member 1 to implement `getAllMembers()` in MemberHash

---

**Copy this code into your src/DebtHeap.cpp and you're done!** 🎉
