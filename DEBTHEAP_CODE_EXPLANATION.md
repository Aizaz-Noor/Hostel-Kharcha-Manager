# DEBTHEAP.CPP - LINE-BY-LINE CODE EXPLANATION

**Complete Walkthrough - Every Function, Every Line**

---

## 📖 QUICK SUMMARY

Your DebtHeap contains 8 functions:

1. **Constructor** - Create heap
2. **Destructor** - Delete heap  
3. **heapifyUp()** - Move element UP (after insert)
4. **heapifyDown()** - Move element DOWN (after extract)
5. **insertDebt()** - Add new debtor
6. **extractMaxDebtor()** - Remove maximum debtor
7. **buildHeapFromHashMap()** - Load data from MemberHash
8. **printSummaryReport()** - Display all sorted debtors

---

## 🏗️ CONSTRUCTOR: Initialize Heap

**Lines 6-10**

```cpp
DebtHeap::DebtHeap(int cap) {
    capacity = cap;           // Set maximum size
    size = 0;                 // Start empty
    heapArray = new DebtRecord[capacity];  // Allocate array
}
```

**What happens:**
- User creates: `DebtHeap analytics(100);`
- Sets capacity to 100 (max 100 debtors)
- Creates empty array with 100 slots
- size = 0 (no elements yet)

**Memory visualization:**
```
heapArray → [empty][empty]...[empty]  (100 slots)
capacity = 100
size = 0
```

---

## 🗑️ DESTRUCTOR: Free Memory

**Lines 12-14**

```cpp
DebtHeap::~DebtHeap() {
    delete[] heapArray;     // Free the array
}
```

**What happens:**
- When heap object is destroyed
- delete[] frees all allocated memory
- Prevents memory leaks

---

## ⬆️ heapifyUp() - Bubble Element UP

**Lines 16-31** | **After inserting at end, move to correct position**

```cpp
void DebtHeap::heapifyUp(int index) {
    while (index > 0) {                    // While not at root
        int parentIdx = parent(index);     // Get parent index
        
        if (heapArray[index].amountOwed > heapArray[parentIdx].amountOwed) {
            // Swap if child > parent
            DebtRecord temp = heapArray[index];
            heapArray[index] = heapArray[parentIdx];
            heapArray[parentIdx] = temp;
            
            index = parentIdx;              // Move up to parent position
        } else {
            break;                          // Heap property satisfied
        }
    }
}
```

**Key concepts:**
- `parent(index) = (index - 1) / 2` - Get parent position
- Max-heap: parent ≥ children
- If child > parent, swap them
- Keep moving up until heap property is satisfied

**Example:**
```
Before:    After inserting 600 at end:    After heapifyUp:
   500                500                        600
  /  \               /  \                      /  \
 400 300            400 300                   500 300
                    /                          /  \
                  600                        400  (600 moved)
```

---

## ⬇️ heapifyDown() - Bubble Element DOWN

**Lines 33-56** | **After removing root, move last element to correct position**

```cpp
void DebtHeap::heapifyDown(int index) {
    while (true) {
        int leftIdx = leftChild(index);         // Get left child
        int rightIdx = rightChild(index);       // Get right child
        int largestIdx = index;                 // Assume current is largest
        
        // Check if left child is larger
        if (leftIdx < size && 
            heapArray[leftIdx].amountOwed > heapArray[largestIdx].amountOwed) {
            largestIdx = leftIdx;
        }
        
        // Check if right child is larger
        if (rightIdx < size && 
            heapArray[rightIdx].amountOwed > heapArray[largestIdx].amountOwed) {
            largestIdx = rightIdx;
        }
        
        // If a child is larger, swap and continue
        if (largestIdx != index) {
            DebtRecord temp = heapArray[index];
            heapArray[index] = heapArray[largestIdx];
            heapArray[largestIdx] = temp;
            
            index = largestIdx;                 // Move down
        } else {
            break;                              // Done
        }
    }
}
```

**Key concepts:**
- `leftChild(index) = 2*index + 1`
- `rightChild(index) = 2*index + 2`
- Find larger child
- Swap if child > parent
- Continue down tree

---

## ➕ insertDebt() - Add to Heap

**Lines 58-71**

```cpp
void DebtHeap::insertDebt(string id, string name, double amount) {
    if (size >= capacity) {                    // Check space
        cout << "ERROR: Heap is full!...\n";
        return;
    }
    
    heapArray[size] = DebtRecord(id, name, amount);  // Add at end
    heapifyUp(size);                           // Move to correct position
    size++;                                     // Increment count
}
```

**Algorithm:**
1. Check if heap is full
2. Add new element at end
3. Call heapifyUp to restore max-heap property
4. Increment size

---

## ➖ extractMaxDebtor() - Remove Maximum

**Lines 73-90**

```cpp
DebtRecord DebtHeap::extractMaxDebtor() {
    if (size <= 0) {                            // Check if empty
        cout << "ERROR: Heap is empty!...\n";
        return DebtRecord();
    }
    
    DebtRecord maxDebtor = heapArray[0];       // Save root (max)
    heapArray[0] = heapArray[size - 1];        // Move last to root
    size--;                                     // Decrement size
    
    if (size > 0) {
        heapifyDown(0);                         // Restore heap property
    }
    
    return maxDebtor;                           // Return max debtor
}
```

**Algorithm:**
1. Check if heap is empty
2. Save maximum (at root)
3. Move last element to root
4. Decrease size
5. Call heapifyDown to restore max-heap property
6. Return saved maximum

---

## 🏗️ buildHeapFromHashMap() - Load Data

**Lines 92-113**

```cpp
void DebtHeap::buildHeapFromHashMap() {
    size = 0;                                   // Clear heap
    
    if (globalMemberHash == nullptr) {         // Check if initialized
        cout << "ERROR: MemberHash not initialized!...\n";
        return;
    }
    
    vector<MemberNode*> allMembers = globalMemberHash->getAllMembers();
    
    for (MemberNode* member : allMembers) {    // For each member
        if (member != nullptr && member->balance < 0) {  // If debtor
            double amountOwed = -(member->balance);      // Convert to positive
            insertDebt(member->id, member->name, amountOwed);
        }
    }
    
    cout << "Heap built with " << size << " debtors.\n";
}
```

**Algorithm:**
1. Clear existing heap
2. Check globalMemberHash is initialized
3. Get all members from MemberHash
4. For each member:
   - If they have negative balance (owe money)
   - Convert negative to positive
   - Insert into heap

---

## 📊 printSummaryReport() - Display Results

**Lines 115-153**

```cpp
void DebtHeap::printSummaryReport() {
    cout << "\n" << string(50, '=') << "\n";
    cout << "              DEBT SUMMARY REPORT\n";
    cout << string(50, '=') << "\n\n";
    
    if (size == 0) {                           // If no debtors
        cout << "Great news! No one owes any money.\n";
        cout << string(50, '=') << "\n\n";
        return;
    }
    
    cout << "Members sorted by amount owed (highest first):\n\n";
    
    int rank = 1;
    double totalDebt = 0;
    
    while (size > 0) {                         // Extract all
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

**Algorithm:**
1. Print header
2. If no debtors, print good news and exit
3. Print column header
4. Loop while heap has elements:
   - Extract maximum debtor
   - Print ranking with name and amount
   - Add to total
   - Increment rank
5. Print separator and total

**Example output:**
```
==================================================
              DEBT SUMMARY REPORT
==================================================

Members sorted by amount owed (highest first):

1. Ali (M001): Rs. 560
2. Omar (M005): Rs. 660  
3. Ayesha (M002): Rs. 360

==================================================
Total Amount Owed: Rs. 1580
==================================================
```

---

## 🎯 KEY FORMULAS

**Parent-Child Relationship in Array:**
```
For element at index i:
- Parent: (i - 1) / 2
- Left child: 2*i + 1
- Right child: 2*i + 2

Example: Element at index 5
- Parent: (5-1)/2 = 2
- Left child: 2*5+1 = 11
- Right child: 2*5+2 = 12
```

---

## ✅ VERIFICATION CHECKLIST

Your DebtHeap.cpp should have:
- ✅ Constructor initializes capacity, size, and heapArray
- ✅ Destructor frees heapArray with delete[]
- ✅ heapifyUp moves element up while child > parent
- ✅ heapifyDown moves element down, comparing with larger child
- ✅ insertDebt adds at end and calls heapifyUp
- ✅ extractMaxDebtor removes root, moves last to root, calls heapifyDown
- ✅ buildHeapFromHashMap filters negative balances and builds heap
- ✅ printSummaryReport extracts all and displays ranked

All present? ✅ **CODE IS COMPLETE AND CORRECT!**

