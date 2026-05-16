# DebtHeap.cpp - Complete Function Breakdown & Implementation Guide

## Overview: What DebtHeap Does
A **Max-Heap Priority Queue** that keeps members sorted by how much they owe.
- **Root (index 0)** = member with HIGHEST debt
- **Array-based** = efficient memory usage
- **Dynamic** = grows as needed (up to capacity)

---

## Function 1: heapifyUp(int index)

### **Purpose**
Restore max-heap property by moving an element UP the tree when it's larger than its parent.

### **When It's Called**
After inserting a new element at the end of the array. The new element might violate the heap property.

### **How It Works**
```
BEFORE heapifyUp:
        [900]          Alice owes 900
       /    \
    [600]  [800]       Bob, Charlie
    /
 [1000]                NEW: David owes 1000
                       Problem: 1000 > 600 (parent)!

AFTER heapifyUp:
        [1000]         David moved to correct position
       /    \
    [900]  [800]       
    /
 [600]                 Bob moved down
```

### **Algorithm (Pseudocode)**
```
while (index > 0):
    parentIndex = (index - 1) / 2
    if (heapArray[index].amountOwed > heapArray[parentIndex].amountOwed):
        swap(heapArray[index], heapArray[parentIndex])
        index = parentIndex
    else:
        break  // Heap property satisfied
```

### **Implementation in C++**
```cpp
void DebtHeap::heapifyUp(int index) {
    // Keep comparing with parent until heap property is satisfied
    while (index > 0) {
        int parentIdx = parent(index);  // Helper: (index-1)/2
        
        // If current element is larger than parent, they're out of order
        if (heapArray[index].amountOwed > heapArray[parentIdx].amountOwed) {
            // Swap them
            DebtRecord temp = heapArray[index];
            heapArray[index] = heapArray[parentIdx];
            heapArray[parentIdx] = temp;
            
            // Move up to parent's position and continue checking
            index = parentIdx;
        } else {
            // Heap property satisfied, stop
            break;
        }
    }
}
```

### **Example Trace**
```
Insert David (1000) at end:
Initial: [900, 600, 800, 1000]
         
Step 1: Compare 1000 with parent 600
        1000 > 600? YES → SWAP
        Result: [900, 1000, 800, 600]
        index = 1

Step 2: Compare 1000 with parent 900
        1000 > 900? YES → SWAP
        Result: [1000, 900, 800, 600]
        index = 0

Step 3: index = 0, no parent → STOP
Final: [1000, 900, 800, 600] ✓ Heap property satisfied!
```

---

## Function 2: heapifyDown(int index)

### **Purpose**
Restore max-heap property by moving an element DOWN the tree when it's smaller than one of its children.

### **When It's Called**
After extracting the root. We move the last element to root, and it might violate the heap property.

### **How It Works**
```
BEFORE heapifyDown:
        [600]          Moved last element (600) to root
       /    \
    [900]  [800]       Problem: 900 > 600 (root)!
    /
 [200]

AFTER heapifyDown:
        [900]          900 moved up
       /    \
    [600]  [800]       600 moved down
    /
 [200]                 Max-heap property restored!
```

### **Algorithm (Pseudocode)**
```
while (index has children):
    leftChildIdx = 2*index + 1
    rightChildIdx = 2*index + 2
    largestIdx = index
    
    // Find the largest among parent and children
    if (leftChild exists and leftChild > parent):
        largestIdx = leftChildIdx
    if (rightChild exists and rightChild > largestIdx):
        largestIdx = rightChildIdx
    
    // If largest is not current node, swap and continue down
    if (largestIdx != index):
        swap(heapArray[index], heapArray[largestIdx])
        index = largestIdx
    else:
        break  // Heap property satisfied
```

### **Implementation in C++**
```cpp
void DebtHeap::heapifyDown(int index) {
    // Keep comparing with children until heap property is satisfied
    while (true) {
        int leftIdx = leftChild(index);     // Helper: 2*index + 1
        int rightIdx = rightChild(index);   // Helper: 2*index + 2
        int largestIdx = index;
        
        // Check if left child exists and is larger than parent
        if (leftIdx < size && heapArray[leftIdx].amountOwed > heapArray[largestIdx].amountOwed) {
            largestIdx = leftIdx;
        }
        
        // Check if right child exists and is larger than current largest
        if (rightIdx < size && heapArray[rightIdx].amountOwed > heapArray[largestIdx].amountOwed) {
            largestIdx = rightIdx;
        }
        
        // If largest is not the current node, swap and continue down
        if (largestIdx != index) {
            DebtRecord temp = heapArray[index];
            heapArray[index] = heapArray[largestIdx];
            heapArray[largestIdx] = temp;
            
            index = largestIdx;
        } else {
            // Heap property satisfied, stop
            break;
        }
    }
}
```

### **Example Trace**
```
After extracting root, move 200 to root:
Initial: [200, 900, 800, ...]
         index = 0

Step 1: Compare 200 with children (900, 800)
        Largest = 900 (left child)
        200 < 900? YES → SWAP
        Result: [900, 200, 800, ...]
        index = 1

Step 2: Compare 200 with children (none at this level)
        No children → STOP
Final: [900, 200, 800, ...] ✓ Heap property satisfied!
```

---

## Function 3: insertDebt(string id, string name, double amount)

### **Purpose**
Add a new debtor to the heap.

### **Steps**
1. Check if heap is full
2. Add new element at the end
3. Increment size
4. Call heapifyUp to restore heap property

### **Implementation in C++**
```cpp
void DebtHeap::insertDebt(string id, string name, double amount) {
    // Step 1: Check if heap is full
    if (size >= capacity) {
        cout << "ERROR: Heap is full! Cannot insert more debtors.\n";
        return;
    }
    
    // Step 2: Create new DebtRecord and add at end
    heapArray[size] = DebtRecord(id, name, amount);
    
    // Step 3: Restore heap property by moving up
    heapifyUp(size);
    
    // Step 4: Increment size
    size++;
}
```

### **Example**
```
Heap capacity: 5
Current heap: [1000, 900, 800]  (size = 3)

Call: insertDebt("D01", "David", 1200)

Step 1: Check 3 < 5? YES ✓
Step 2: Add at position 3: [1000, 900, 800, 1200]
Step 3: heapifyUp(3) → moves 1200 up
        Result: [1200, 1000, 800, 900]
Step 4: size = 4

Final: [1200, 1000, 800, 900] (size = 4) ✓
```

---

## Function 4: extractMaxDebtor()

### **Purpose**
Remove and return the member who owes the most money (root of max-heap).

### **Steps**
1. Save the root (max debtor)
2. Move last element to root
3. Decrement size
4. Call heapifyDown to restore heap property
5. Return saved max debtor

### **Implementation in C++**
```cpp
DebtRecord DebtHeap::extractMaxDebtor() {
    // Step 1: Check if heap is empty
    if (size <= 0) {
        cout << "ERROR: Heap is empty!\n";
        return DebtRecord();  // Return empty record
    }
    
    // Step 2: Save the root (person with most debt)
    DebtRecord maxDebtor = heapArray[0];
    
    // Step 3: Move last element to root
    heapArray[0] = heapArray[size - 1];
    
    // Step 4: Decrement size
    size--;
    
    // Step 5: Restore heap property
    if (size > 0) {
        heapifyDown(0);
    }
    
    // Step 6: Return the maximum debtor
    return maxDebtor;
}
```

### **Example**
```
Heap: [1200, 1000, 800, 900]  (size = 4)
Alice owes 1200, Bob owes 1000, etc.

Call: extractMaxDebtor()

Step 1: Save Alice (1200)
Step 2: Move 900 to root: [900, 1000, 800]
Step 3: size = 3
Step 4: heapifyDown(0):
        Compare 900 with children (1000, 800)
        Swap with 1000: [1000, 900, 800]
Step 5: Return Alice (1200) ✓

Next call would return Bob (1000)
```

---

## Function 5: buildHeapFromHashMap()

### **Purpose**
Populate the heap with all members who owe money (negative balance) from MemberHash.

### **Considerations**
- MemberHash needs to provide a way to get all members
- Only insert members with negative balance
- Clear existing heap first

### **Implementation in C++**
```cpp
void DebtHeap::buildHeapFromHashMap(MemberHash& memberData) {
    // Step 1: Clear existing heap
    size = 0;
    
    // Step 2: Get all members from MemberHash
    // NOTE: This assumes Member 1 provides this method:
    // vector<MemberNode*> getAllMembers();
    
    vector<MemberNode*> allMembers = memberData.getAllMembers();
    
    // Step 3: For each member, if they owe money, insert into heap
    for (MemberNode* member : allMembers) {
        if (member->balance < 0) {  // Negative balance means they owe
            double amountOwed = -(member->balance);  // Convert to positive
            insertDebt(member->id, member->name, amountOwed);
        }
    }
    
    cout << "Heap built with " << size << " debtors.\n";
}
```

### **Example**
```
MemberHash contains:
├─ A01: Alice, balance = -2000  (owes 2000)
├─ B01: Bob, balance = +1000    (owed 1000)
├─ C01: Charlie, balance = -1500 (owes 1500)

Call: buildHeapFromHashMap(profiles)

Process Alice: -2000 < 0? YES → insertDebt("A01", "Alice", 2000)
Process Bob: +1000 < 0? NO → Skip
Process Charlie: -1500 < 0? YES → insertDebt("C01", "Charlie", 1500)

Result Heap: [2000(Alice), 1500(Charlie)]
Only debtors are in the heap!
```

---

## Function 6: printSummaryReport()

### **Purpose**
Extract all debtors from heap and print them in order (highest debt first).

### **Important Note**
This operation DESTROYS the heap (extracts everything).
If you need to keep the heap intact, save records first.

### **Implementation in C++**
```cpp
void DebtHeap::printSummaryReport() {
    cout << "\n========== DEBT SUMMARY REPORT ==========\n";
    cout << "Members sorted by amount owed (highest first):\n\n";
    
    int rank = 1;
    double totalDebt = 0;
    
    // Extract all debtors and print them
    while (size > 0) {
        DebtRecord debtor = extractMaxDebtor();
        
        cout << rank << ". " << debtor.memberName 
             << " (" << debtor.memberId << "): "
             << "Rs. " << debtor.amountOwed << "\n";
        
        totalDebt += debtor.amountOwed;
        rank++;
    }
    
    cout << "\n========== TOTAL DEBT ==========\n";
    cout << "Total Amount Owed: Rs. " << totalDebt << "\n";
    cout << "==========================================\n\n";
}
```

### **Example Output**
```
========== DEBT SUMMARY REPORT ==========
Members sorted by amount owed (highest first):

1. Alice (A01): Rs. 2000
2. Charlie (C01): Rs. 1500
3. David (D01): Rs. 800
4. Eva (E01): Rs. 300

========== TOTAL DEBT ==========
Total Amount Owed: Rs. 4600
==========================================
```

### **Alternative: Non-Destructive Version**
```cpp
// If you want to keep heap intact, save records first:
void DebtHeap::printSummaryReport() {
    // Create temporary copy of heap data
    vector<DebtRecord> temp;
    
    // Extract all and save
    while (size > 0) {
        temp.push_back(extractMaxDebtor());
    }
    
    // Print them
    cout << "Debtors sorted by amount:\n";
    for (int i = 0; i < temp.size(); i++) {
        cout << (i+1) << ". " << temp[i].memberName 
             << ": Rs. " << temp[i].amountOwed << "\n";
    }
    
    // Re-insert everything to restore heap
    for (DebtRecord& record : temp) {
        insertDebt(record.memberId, record.memberName, record.amountOwed);
    }
}
```

---

## Helper Functions (Already Defined in Header)

```cpp
int parent(int i) { return (i - 1) / 2; }      // Parent index
int leftChild(int i) { return (2 * i) + 1; }   // Left child index
int rightChild(int i) { return (2 * i) + 2; }  // Right child index
```

These are already in the header, just use them!

---

## Summary: Function Call Flow

```
1. Create heap:
   DebtHeap heap(100);
   └─ Constructor initializes capacity, size=0, allocates array

2. Build from MemberHash:
   heap.buildHeapFromHashMap(profiles);
   └─ Gets all members
   └─ For each with balance < 0:
      └─ insertDebt()
         ├─ Add at end
         └─ heapifyUp()

3. Extract max:
   DebtRecord top = heap.extractMaxDebtor();
   └─ Save root
   └─ Move last to root
   └─ heapifyDown()
   └─ Return saved

4. Print report:
   heap.printSummaryReport();
   └─ Loop through heap
   └─ Extract all and print

5. Destructor:
   └─ delete[] heapArray
```

---

## Testing Your Implementation

```cpp
// Simple test
int main() {
    DebtHeap heap(10);
    
    // Manually insert test data
    heap.insertDebt("A01", "Alice", 2000);
    heap.insertDebt("B01", "Bob", 1500);
    heap.insertDebt("C01", "Charlie", 2500);
    
    // Extract and verify order
    cout << heap.extractMaxDebtor().memberName;  // Should be Charlie (2500)
    cout << heap.extractMaxDebtor().memberName;  // Should be Alice (2000)
    cout << heap.extractMaxDebtor().memberName;  // Should be Bob (1500)
    
    return 0;
}
```

**You now have everything you need to implement DebtHeap.cpp!** 🚀
