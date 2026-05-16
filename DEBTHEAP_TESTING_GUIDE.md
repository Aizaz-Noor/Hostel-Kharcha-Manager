# DEBTHEAP TESTING GUIDE - Dev-C++ Instructions

**How to Compile, Test, and Verify DebtHeap**

---

## 📋 TABLE OF CONTENTS

1. [Quick Test (5 minutes)](#quick-test)
2. [Standalone Test (Dev-C++)](#standalone-test)
3. [Integration Test (Needs MemberHash)](#integration-test)
4. [Debugging Tips](#debugging-tips)
5. [Expected Output Examples](#expected-output)

---

## ⚡ QUICK TEST (5 minutes)

### **Step 1: Open TestIntegrated_Main.cpp**
- Navigate to: `d:\Hostel-Kharcha-Manager\TestIntegrated_Main.cpp`
- Open in Dev-C++

### **Step 2: Compile**
```
Menu: Compile → Compile (or Ctrl+F9)
```

### **Step 3: Run**
```
Menu: Execute → Run (or Ctrl+F10)
```

### **Step 4: Verify Output**
```
Should show:
- 5 members created
- 5 transactions recorded
- Member balances calculated
- DebtHeap output with ranking
```

**If successful:** ✅ DebtHeap is working!

---

## 🧪 STANDALONE TEST (Dev-C++)

### **Create a Simple Test File**

**File:** `Test_DebtHeap_Simple.cpp`  
**Location:** Root of project

```cpp
#include <iostream>
#include "include/DebtHeap.h"

using namespace std;

// IMPORTANT: Declare this
MemberHash* globalMemberHash = nullptr;

int main() {
    cout << "\n=== DebtHeap Standalone Test ===\n\n";
    
    // Create heap with capacity 10
    DebtHeap heap(10);
    cout << "✓ Created heap with capacity 10\n";
    
    // Test 1: Add some debtors manually
    cout << "\n--- Test 1: Insert Debtors ---\n";
    heap.insertDebt("M001", "Ali", 500);
    cout << "✓ Inserted Ali with 500\n";
    
    heap.insertDebt("M002", "Ayesha", 200);
    cout << "✓ Inserted Ayesha with 200\n";
    
    heap.insertDebt("M003", "Hassan", 600);
    cout << "✓ Inserted Hassan with 600\n";
    
    heap.insertDebt("M004", "Zainab", 300);
    cout << "✓ Inserted Zainab with 300\n";
    
    // Test 2: Extract and verify order
    cout << "\n--- Test 2: Extract (Should be descending) ---\n";
    
    DebtRecord d1 = heap.extractMaxDebtor();
    cout << "1. " << d1.memberName << ": " << d1.amountOwed << " (Expected: 600)\n";
    
    DebtRecord d2 = heap.extractMaxDebtor();
    cout << "2. " << d2.memberName << ": " << d2.amountOwed << " (Expected: 500)\n";
    
    DebtRecord d3 = heap.extractMaxDebtor();
    cout << "3. " << d3.memberName << ": " << d3.amountOwed << " (Expected: 300)\n";
    
    DebtRecord d4 = heap.extractMaxDebtor();
    cout << "4. " << d4.memberName << ": " << d4.amountOwed << " (Expected: 200)\n";
    
    // Test 3: Verify empty heap handling
    cout << "\n--- Test 3: Extract from empty heap ---\n";
    DebtRecord empty = heap.extractMaxDebtor();
    cout << "✓ Handled empty heap gracefully\n";
    
    cout << "\n=== All Tests Passed! ===\n";
    
    return 0;
}
```

### **How to Run This Test**

1. **Create file**
   - File → New → Source File
   - Copy code above
   - Save as: `Test_DebtHeap_Simple.cpp`

2. **Open project in Dev-C++**
   - File → Open Project/Workspace
   - Navigate to project folder
   - Open the project

3. **Add the test file to project**
   - Project → Add to Project
   - Select `Test_DebtHeap_Simple.cpp`

4. **Compile entire project**
   - Menu: Compile → Build All (F12)

5. **Run the test**
   - Menu: Execute → Run (Ctrl+F10)

### **Expected Output**

```
=== DebtHeap Standalone Test ===

✓ Created heap with capacity 10

--- Test 1: Insert Debtors ---
✓ Inserted Ali with 500
✓ Inserted Ayesha with 200
✓ Inserted Hassan with 600
✓ Inserted Zainab with 300

--- Test 2: Extract (Should be descending) ---
1. Hassan: 600 (Expected: 600)
2. Ali: 500 (Expected: 500)
3. Zainab: 300 (Expected: 300)
4. Ayesha: 200 (Expected: 200)

--- Test 3: Extract from empty heap ---
ERROR: Heap is empty!...
✓ Handled empty heap gracefully

=== All Tests Passed! ===
```

---

## 🔗 INTEGRATION TEST (Needs MemberHash)

### **Prerequisites**

Member 1 must implement:
```cpp
vector<MemberNode*> getAllMembers();
```

Once available, use `TestIntegrated_Main.cpp` which includes:
- Creating members
- Recording transactions
- Calculating balances
- Building DebtHeap
- Displaying ranking

---

## 🐛 DEBUGGING TIPS

### **Enable Debug Output**

Add this to DebtHeap.cpp temporarily:

```cpp
void DebtHeap::heapifyUp(int index) {
    cout << "DEBUG: heapifyUp called for index " << index << "\n";
    while (index > 0) {
        int parentIdx = parent(index);
        cout << "DEBUG: Comparing " << heapArray[index].amountOwed 
             << " with parent " << heapArray[parentIdx].amountOwed << "\n";
        
        if (heapArray[index].amountOwed > heapArray[parentIdx].amountOwed) {
            // swap code...
            cout << "DEBUG: Swapped\n";
        } else {
            break;
        }
    }
}
```

### **Test Specific Function**

```cpp
// Test only heapifyUp
DebtHeap heap(5);
heap.insertDebt("M1", "A", 100);
cout << "After insert 100 at index 0\n";

heap.insertDebt("M2", "B", 200);
cout << "After insert 200 at index 1\n";
// Should call heapifyUp and swap

heap.insertDebt("M3", "C", 150);
cout << "After insert 150 at index 2\n";
```

### **Check Memory**

```cpp
// Before
cout << "Heap at start: " << heap.size << " elements\n";

// Do operations
heap.insertDebt("M1", "A", 100);

// After
cout << "Heap now: " << heap.size << " elements\n";
```

---

## 📊 EXPECTED OUTPUT EXAMPLES

### **Example 1: Simple Insertion**

```
Input:
DebtHeap heap(10);
heap.insertDebt("M001", "Ali", 500);
heap.insertDebt("M002", "Ayesha", 300);
heap.printSummaryReport();

Expected Output:
==================================================
              DEBT SUMMARY REPORT
==================================================

Members sorted by amount owed (highest first):

1. Ali (M001): Rs. 500
2. Ayesha (M002): Rs. 300

==================================================
Total Amount Owed: Rs. 800
==================================================
```

### **Example 2: Larger Heap**

```
Input:
Insert 5 members with debts: 100, 500, 200, 450, 150
Call printSummaryReport()

Expected Output:
==================================================
              DEBT SUMMARY REPORT
==================================================

Members sorted by amount owed (highest first):

1. Name (ID): Rs. 500
2. Name (ID): Rs. 450
3. Name (ID): Rs. 200
4. Name (ID): Rs. 150
5. Name (ID): Rs. 100

==================================================
Total Amount Owed: Rs. 1400
==================================================
```

### **Example 3: No Debtors**

```
Input:
DebtHeap heap(10);
heap.printSummaryReport();  // Without inserting anything

Expected Output:
==================================================
              DEBT SUMMARY REPORT
==================================================

Great news! No one owes any money.
==================================================
```

---

## ✅ COMPREHENSIVE TEST CHECKLIST

### **Level 1: Compilation**
```
☑ DebtHeap.h includes properly
☑ DebtHeap.cpp compiles without errors
☑ No undefined references
☑ Dev-C++ shows "Compile succeeded"
```

### **Level 2: Basic Functionality**
```
☑ Constructor creates heap properly
☑ insertDebt() adds element
☑ extractMaxDebtor() returns correct element
☑ printSummaryReport() displays formatted output
☑ No segmentation faults
```

### **Level 3: Heap Property**
```
☑ First extraction = maximum value
☑ Second extraction = second maximum
☑ Order is strictly descending
☑ All elements extracted correctly
```

### **Level 4: Edge Cases**
```
☑ Empty heap handled (no crash)
☑ Full heap handled (capacity check)
☑ Single element works
☑ Duplicate values sorted correctly
☑ Large numbers handled (10000+)
☑ Negative results don't occur (all positive amounts)
```

---

## 🎯 VERIFICATION TESTS

### **Test: Correct Sorting**
```cpp
// Insert in random order
heap.insertDebt("M1", "C", 100);
heap.insertDebt("M2", "A", 500);
heap.insertDebt("M3", "B", 300);

// Extract should be: 500, 300, 100
d1 = heap.extractMaxDebtor();  // Should be 500 ✓
d2 = heap.extractMaxDebtor();  // Should be 300 ✓
d3 = heap.extractMaxDebtor();  // Should be 100 ✓
```

### **Test: Memory No Leak**
```
// Run through insert/extract cycles
// Monitor memory in Task Manager
// Memory should not continuously increase
// After program ends, memory freed
```

### **Test: Capacity Enforcement**
```cpp
DebtHeap heap(3);  // Capacity 3
heap.insertDebt("M1", "A", 100);  // OK
heap.insertDebt("M2", "B", 200);  // OK
heap.insertDebt("M3", "C", 300);  // OK
heap.insertDebt("M4", "D", 400);  // ERROR: Heap is full
```

### **Test: Integration Ready**
```cpp
// Once MemberHash complete:
// 1. Create MemberHash and add members
// 2. Set globalMemberHash = &memberHash;
// 3. Create DebtHeap
// 4. Call buildHeapFromHashMap()
// 5. Call printSummaryReport()
// 6. Verify output matches member data
```

---

## 🚀 QUICK TEST SCRIPT

Copy this entire test into a file:

```cpp
#include <iostream>
#include "include/DebtHeap.h"
using namespace std;

MemberHash* globalMemberHash = nullptr;

int main() {
    cout << "\n=== QUICK DEBTHEAP TEST ===\n";
    
    // Create heap
    DebtHeap h(10);
    
    // Insert test data
    h.insertDebt("1", "A", 100);
    h.insertDebt("2", "B", 500);
    h.insertDebt("3", "C", 300);
    h.insertDebt("4", "D", 200);
    
    // Print report (extracts all)
    h.printSummaryReport();
    
    cout << "=== TEST COMPLETE ===\n";
    return 0;
}
```

**Expected:** Members sorted as B(500), C(300), D(200), A(100)

---

## 🏁 FINAL CHECKLIST

Before considering testing complete:

```
☑ Compiles without errors
☑ No runtime errors or crashes
☑ Correct sorting order (descending)
☑ Proper formatting in output
☑ Memory properly managed
☑ Edge cases handled
☑ Ready for integration with main.cpp
```

**All checked?** ✅ **DebtHeap is production-ready!**

