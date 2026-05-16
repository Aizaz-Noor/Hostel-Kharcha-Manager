# DebtHeap Functions - Quick Reference Table

## All 6 Functions at a Glance

| Function | Purpose | When Called | Time | Key Logic |
|----------|---------|-------------|------|-----------|
| **heapifyUp()** | Move element UP to maintain max-heap | After insertDebt() | O(log n) | Compare with parent, swap if larger, repeat |
| **heapifyDown()** | Move element DOWN to maintain max-heap | After extractMaxDebtor() | O(log n) | Find larger child, swap if needed, repeat |
| **insertDebt()** | Add new debtor to heap | When logging expense | O(log n) | Add at end → heapifyUp → increment size |
| **extractMaxDebtor()** | Remove & return top debtor | When querying top debtor | O(log n) | Save root → move last to root → heapifyDown |
| **buildHeapFromHashMap()** | Populate from MemberHash data | When user asks "Who owes most?" | O(n log n) | Get members → filter by balance < 0 → insertDebt each |
| **printSummaryReport()** | Print all debtors sorted | After buildHeapFromHashMap() | O(n log n) | Loop: extract all → print → show total |

---

## Function Dependencies

```
buildHeapFromHashMap()
    ↓ calls
insertDebt()
    ↓ calls
heapifyUp()
    ↓
maintains max-heap property


printSummaryReport()
    ↓ calls
extractMaxDebtor()
    ↓ calls
heapifyDown()
    ↓
maintains max-heap property
```

---

## Parameter & Return Reference

| Function | Parameters | Returns | Notes |
|----------|-----------|---------|-------|
| heapifyUp(index) | int index | void | Index of element to move up |
| heapifyDown(index) | int index | void | Index of element to move down |
| insertDebt(id, name, amt) | string, string, double | void | Adds to heap, increments size |
| extractMaxDebtor() | none | DebtRecord | Returns member who owes most |
| buildHeapFromHashMap(ref) | MemberHash& | void | Needs getAllMembers() from Member1 |
| printSummaryReport() | none | void | Destructive - empties heap |

---

## Step-by-Step Operation Flow

### **Scenario: User wants to see top debtor**

```
Step 1: User chooses option 6
        main.cpp calls:
        analytics.buildHeapFromHashMap(profiles);

Step 2: buildHeapFromHashMap() executes:
        ├─ size = 0  (clear heap)
        ├─ allMembers = profiles.getAllMembers()
        └─ for each member:
            if (balance < 0):
               insertDebt(id, name, -balance)

Step 3: insertDebt() executes:
        ├─ Check capacity
        ├─ Add at end of heapArray
        ├─ heapifyUp(size)  ← ensures heap property
        └─ size++

Step 4: heapifyUp() executes:
        ├─ Compare with parent
        ├─ Swap if child > parent
        ├─ Move to parent index
        └─ Repeat until root or heap property ok

Step 5: Back in main.cpp:
        analytics.printSummaryReport()

Step 6: printSummaryReport() executes:
        ├─ Print header
        └─ while (size > 0):
            ├─ top = extractMaxDebtor()
            │   ├─ Save root
            │   ├─ Move last to root
            │   ├─ heapifyDown(0)
            │   └─ return saved
            ├─ Print top member
            └─ heapifyDown() maintains order

Result: All debtors printed from highest to lowest ✓
```

---

## Index Calculation Reference

```
For element at index i:
─────────────────────────
Parent:     (i - 1) / 2
Left child:  2*i + 1
Right child: 2*i + 2

Example: index = 5
Parent:     (5-1)/2 = 2
Left child:  2*5+1 = 11
Right child: 2*5+2 = 12

Array indices:  [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, ...]
                        2           5        11  12
               (parent of 5)    (at 5)  (children of 5)
```

---

## Common Patterns

### **Pattern 1: Insert and Maintain**
```cpp
insertDebt(id, name, amt)
{
    heapArray[size] = new record
    heapifyUp(size)  // ← maintains property
    size++
}
```

### **Pattern 2: Extract and Maintain**
```cpp
extractMaxDebtor()
{
    save heapArray[0]
    heapArray[0] = heapArray[size-1]
    size--
    heapifyDown(0)  // ← maintains property
    return saved
}
```

### **Pattern 3: Swap Elements**
```cpp
temp = heapArray[a]
heapArray[a] = heapArray[b]
heapArray[b] = temp
```

---

## Key Conditions to Check

```
In heapifyUp():
├─ while (index > 0)  ← stop at root
└─ if (child > parent)  ← swap if larger

In heapifyDown():
├─ while (has children)  ← stop at leaf
├─ if (leftChild > parent or rightChild > parent)  ← swap if needed
└─ Always compare with LARGER child

In insertDebt():
├─ if (size >= capacity)  ← check before adding
└─ size++ after inserting

In extractMaxDebtor():
├─ if (size <= 0)  ← check if empty
└─ if (size > 0) heapifyDown  ← only if elements remain

In buildHeapFromHashMap():
├─ if (balance < 0)  ← only add debtors
└─ -balance  ← convert negative to positive
```

---

## Heap Property to Maintain

```
MAX-HEAP PROPERTY:
parent >= left child AND parent >= right child

Examples of VALID max-heaps:
        [1000]              [900]              [500]
       /     \             /    \            /     \
     [900]  [800]       [800]  [700]      [400]  [300]
     /  \                  
  [700][500]           

Examples of INVALID max-heaps (would violate property):
        [500]   ← ROOT should be MAX!
       /    \
    [900]  [800]   ← WRONG: children > parent


Array representation check:
[1000, 900, 800, 700, 500]
 i=0   i=1  i=2  i=3  i=4

parent(1) = (1-1)/2 = 0 → 900 <= 1000 ✓
parent(2) = (2-1)/2 = 0 → 800 <= 1000 ✓
parent(3) = (3-1)/2 = 1 → 700 <= 900 ✓
parent(4) = (4-1)/2 = 1 → 500 <= 900 ✓
All valid → Heap property maintained ✓
```

---

## How to Debug

### **Problem: Extract not returning top debtor**
```
Check:
1. Is extractMaxDebtor() returning heapArray[0]? (root)
2. Does heapifyDown() restore heap property after removing?
3. Are indices calculated correctly?
```

### **Problem: Heap is not sorted correctly**
```
Check:
1. Are you calling heapifyUp() after insert?
2. Are you calling heapifyDown() after extract?
3. Is comparison (>) correct for max-heap?
```

### **Problem: buildHeapFromHashMap() not finding debtors**
```
Check:
1. Does MemberHash provide getAllMembers()?
2. Are you checking (balance < 0)?
3. Are you converting to positive: -balance?
```

---

## Testing Checklist

```
☐ insertDebt() adds elements correctly
☐ heapifyUp() moves to correct position
☐ heapifyDown() moves to correct position
☐ extractMaxDebtor() returns root first
☐ extractMaxDebtor() order is highest to lowest
☐ buildHeapFromHashMap() only adds negative balances
☐ printSummaryReport() displays in correct order
☐ Empty heap handled gracefully
☐ Full heap handled gracefully
☐ Capacity check prevents overflow
```

---

**Reference this table while implementing!** 📋
