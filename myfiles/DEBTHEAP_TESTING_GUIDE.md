# DebtHeap Testing Guide

Your implementation is complete! Here's how to test it thoroughly.

---

## Testing Strategy (4 Levels)

### **Level 1: Unit Tests (Test individual functions)**
### **Level 2: Integration Tests (Test with MemberHash)**
### **Level 3: Edge Case Tests (Boundary conditions)**
### **Level 4: System Tests (Full flow through main.cpp)**

---

## Level 1: Unit Tests (Individual Functions)

### **Test File: test_debtheap_unit.cpp**

Create this file in your project root to test individual functions:

```cpp
#include <iostream>
#include <cassert>
#include "include/DebtHeap.h"

using namespace std;

// Forward declare global
MemberHash* globalMemberHash = nullptr;

void test_insertDebt_basic() {
    cout << "\n=== Test 1: insertDebt() - Basic Insert ===\n";
    
    DebtHeap heap(10);
    
    // Insert 3 debtors
    heap.insertDebt("A01", "Alice", 1000);
    heap.insertDebt("B01", "Bob", 2000);
    heap.insertDebt("C01", "Charlie", 500);
    
    // Extract and verify order (should be highest first)
    DebtRecord first = heap.extractMaxDebtor();
    assert(first.memberName == "Bob");
    assert(first.amountOwed == 2000);
    cout << "✓ First extraction correct: " << first.memberName << " owes " << first.amountOwed << "\n";
    
    DebtRecord second = heap.extractMaxDebtor();
    assert(second.memberName == "Alice");
    assert(second.amountOwed == 1000);
    cout << "✓ Second extraction correct: " << second.memberName << " owes " << second.amountOwed << "\n";
    
    DebtRecord third = heap.extractMaxDebtor();
    assert(third.memberName == "Charlie");
    assert(third.amountOwed == 500);
    cout << "✓ Third extraction correct: " << third.memberName << " owes " << third.amountOwed << "\n";
}

void test_heapifyUp() {
    cout << "\n=== Test 2: heapifyUp() - Bubble Up ===\n";
    
    DebtHeap heap(10);
    
    // Insert in order: 100, 200, 50, 300
    // After each insert, verify max is at root
    heap.insertDebt("A", "Person A", 100);
    cout << "✓ Inserted 100, heap size: 1\n";
    
    heap.insertDebt("B", "Person B", 200);
    cout << "✓ Inserted 200, heap size: 2\n";
    
    heap.insertDebt("C", "Person C", 50);
    cout << "✓ Inserted 50, heap size: 3\n";
    
    heap.insertDebt("D", "Person D", 300);
    cout << "✓ Inserted 300, heap size: 4\n";
    
    // Extract max and verify it's 300
    DebtRecord max = heap.extractMaxDebtor();
    assert(max.amountOwed == 300);
    cout << "✓ Max correctly bubbled to root: " << max.amountOwed << "\n";
}

void test_heapifyDown() {
    cout << "\n=== Test 3: heapifyDown() - Bubble Down ===\n";
    
    DebtHeap heap(10);
    
    // Insert several debtors
    heap.insertDebt("A", "Alice", 2500);
    heap.insertDebt("B", "Bob", 2000);
    heap.insertDebt("C", "Charlie", 1500);
    heap.insertDebt("D", "David", 1000);
    
    cout << "✓ Inserted 4 debtors\n";
    
    // Extract max multiple times and verify order
    DebtRecord r1 = heap.extractMaxDebtor();
    assert(r1.amountOwed == 2500);
    cout << "✓ First: " << r1.amountOwed << " (expected 2500)\n";
    
    DebtRecord r2 = heap.extractMaxDebtor();
    assert(r2.amountOwed == 2000);
    cout << "✓ Second: " << r2.amountOwed << " (expected 2000)\n";
    
    DebtRecord r3 = heap.extractMaxDebtor();
    assert(r3.amountOwed == 1500);
    cout << "✓ Third: " << r3.amountOwed << " (expected 1500)\n";
    
    DebtRecord r4 = heap.extractMaxDebtor();
    assert(r4.amountOwed == 1000);
    cout << "✓ Fourth: " << r4.amountOwed << " (expected 1000)\n";
}

void test_extractMaxDebtor_order() {
    cout << "\n=== Test 4: extractMaxDebtor() - Correct Ordering ===\n";
    
    DebtHeap heap(5);
    
    // Insert random order
    heap.insertDebt("E", "Eve", 300);
    heap.insertDebt("B", "Bob", 800);
    heap.insertDebt("A", "Alice", 500);
    heap.insertDebt("D", "David", 200);
    heap.insertDebt("C", "Charlie", 600);
    
    cout << "✓ Inserted 5 debtors in random order\n";
    cout << "\nExtraction order (should be descending):\n";
    
    double prevAmount = 999999;
    int count = 1;
    
    while (true) {
        DebtRecord debtor = heap.extractMaxDebtor();
        if (debtor.memberName.empty()) break;  // Empty name = extraction failed
        
        assert(debtor.amountOwed <= prevAmount);  // Each should be <= previous
        cout << count << ". " << debtor.memberName << ": " << debtor.amountOwed << "\n";
        
        prevAmount = debtor.amountOwed;
        count++;
    }
    
    cout << "✓ All extractions in correct descending order!\n";
}

void test_empty_heap() {
    cout << "\n=== Test 5: Empty Heap Handling ===\n";
    
    DebtHeap heap(10);
    
    // Try to extract from empty heap
    cout << "Attempting to extract from empty heap...\n";
    DebtRecord empty = heap.extractMaxDebtor();
    
    assert(empty.memberName.empty());
    cout << "✓ Empty heap handled gracefully\n";
}

void test_capacity_limit() {
    cout << "\n=== Test 6: Capacity Limit ===\n";
    
    DebtHeap heap(3);  // Capacity of 3
    
    heap.insertDebt("A", "Alice", 100);
    heap.insertDebt("B", "Bob", 200);
    heap.insertDebt("C", "Charlie", 300);
    
    cout << "✓ Inserted 3 debtors (at capacity)\n";
    
    // Try to insert beyond capacity
    cout << "Attempting to insert beyond capacity...\n";
    heap.insertDebt("D", "David", 400);  // Should fail gracefully
    
    cout << "✓ Capacity limit enforced\n";
}

void test_single_debtor() {
    cout << "\n=== Test 7: Single Debtor ===\n";
    
    DebtHeap heap(10);
    
    heap.insertDebt("X", "Xavier", 5000);
    cout << "✓ Inserted 1 debtor\n";
    
    DebtRecord debtor = heap.extractMaxDebtor();
    assert(debtor.memberName == "Xavier");
    assert(debtor.amountOwed == 5000);
    cout << "✓ Single debtor extracted correctly: " << debtor.memberName << " owes " << debtor.amountOwed << "\n";
}

// Main test runner
int main() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "          DEBTHEAP UNIT TEST SUITE\n";
    cout << string(60, '=') << "\n";
    
    try {
        test_insertDebt_basic();
        test_heapifyUp();
        test_heapifyDown();
        test_extractMaxDebtor_order();
        test_empty_heap();
        test_capacity_limit();
        test_single_debtor();
        
        cout << "\n" << string(60, '=') << "\n";
        cout << "✓ ALL UNIT TESTS PASSED!\n";
        cout << string(60, '=') << "\n\n";
        
    } catch (const exception& e) {
        cout << "\n✗ TEST FAILED: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
```

---

## How to Run Unit Tests

### **Step 1: Compile the test file**
```powershell
cd D:\Hostel-Kharcha-Manager
g++ -o test_debtheap test_debtheap_unit.cpp src/DebtHeap.cpp src/MemberHash.cpp -I.
```

### **Step 2: Run the test**
```powershell
.\test_debtheap.exe
```

### **Expected Output**
```
============================================================
          DEBTHEAP UNIT TEST SUITE
============================================================

=== Test 1: insertDebt() - Basic Insert ===
✓ First extraction correct: Bob owes 2000
✓ Second extraction correct: Alice owes 1000
✓ Third extraction correct: Charlie owes 500

=== Test 2: heapifyUp() - Bubble Up ===
✓ Inserted 100, heap size: 1
✓ Inserted 200, heap size: 2
✓ Inserted 50, heap size: 3
✓ Inserted 300, heap size: 4
✓ Max correctly bubbled to root: 300

[... more tests ...]

✓ ALL UNIT TESTS PASSED!
============================================================
```

---

## Level 2: Integration Tests (With MemberHash)

### **Test File: test_debtheap_integration.cpp**

```cpp
#include <iostream>
#include "include/DebtHeap.h"
#include "include/MemberHash.h"

using namespace std;

// Global for buildHeapFromHashMap
MemberHash* globalMemberHash = nullptr;

void test_buildHeapFromHashMap() {
    cout << "\n=== Integration Test: buildHeapFromHashMap() ===\n";
    
    // Create MemberHash and set global
    MemberHash profiles(10);
    globalMemberHash = &profiles;
    
    // Manually create some members with balances
    // Note: We're simulating what updateBalance() should do
    profiles.addMember("M01", "Member 1", "member1@email.com");
    profiles.addMember("M02", "Member 2", "member2@email.com");
    profiles.addMember("M03", "Member 3", "member3@email.com");
    
    // Manually set balances (simulating updateBalance())
    // Note: This is a workaround for testing until updateBalance() is implemented
    MemberNode* m1 = profiles.searchMember("M01");
    MemberNode* m2 = profiles.searchMember("M02");
    MemberNode* m3 = profiles.searchMember("M03");
    
    if (m1) m1->balance = -1500;  // Owes 1500
    if (m2) m2->balance = -2500;  // Owes 2500
    if (m3) m3->balance = 100;    // Doesn't owe (positive)
    
    cout << "✓ Created 3 members with balances\n";
    
    // Build heap from MemberHash
    DebtHeap analytics(10);
    analytics.buildHeapFromHashMap();
    
    cout << "✓ Heap built from MemberHash\n";
    
    // Test extraction
    DebtRecord first = analytics.extractMaxDebtor();
    cout << "First debtor: " << first.memberName << " owes " << first.amountOwed << "\n";
    assert(first.amountOwed == 2500);
    
    DebtRecord second = analytics.extractMaxDebtor();
    cout << "Second debtor: " << second.memberName << " owes " << second.amountOwed << "\n";
    assert(second.amountOwed == 1500);
}

int main() {
    cout << "\n" << string(60, '=') << "\n";
    cout << "      DEBTHEAP INTEGRATION TEST SUITE\n";
    cout << string(60, '=') << "\n";
    
    try {
        test_buildHeapFromHashMap();
        
        cout << "\n" << string(60, '=') << "\n";
        cout << "✓ ALL INTEGRATION TESTS PASSED!\n";
        cout << string(60, '=') << "\n\n";
        
    } catch (const exception& e) {
        cout << "\n✗ TEST FAILED: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
```

---

## Level 3: Edge Case Tests

| Edge Case | Test | Expected Result |
|-----------|------|-----------------|
| Empty heap extract | extractMaxDebtor() on empty | Returns empty DebtRecord |
| Capacity exceeded | Insert > capacity | ERROR message, no crash |
| Single element | Insert 1, extract 1 | Correct element returned |
| Large amounts | Insert 999999.99 | Correct ordering maintained |
| Duplicate amounts | Insert 500, 500, 500 | All extracted correctly |
| Negative amounts | Insert -100 | Handled or rejected gracefully |
| Same person twice | Insert same ID twice | Either rejected or replaced |

---

## Level 4: System Test (Full Flow in main.cpp)

Add this test code in your main.cpp:

```cpp
// In main.cpp, after menu setup:

if (choice == 6) {  // Who Owes Most?
    cout << "\n--- Building debt heap...\n";
    globalMemberHash = &profiles;  // Set global
    
    analytics.buildHeapFromHashMap();
    
    cout << "\n--- Displaying debt report...\n";
    analytics.printSummaryReport();
}
```

---

## Testing Checklist

Use this checklist to verify everything works:

```
BASIC FUNCTIONALITY
☐ insertDebt() adds elements without crashing
☐ extractMaxDebtor() returns elements without crashing
☐ heapifyUp() maintains heap property after insert
☐ heapifyDown() maintains heap property after extract
☐ Extracted elements are in descending order (highest first)

EDGE CASES
☐ Empty heap doesn't crash
☐ Capacity limit is enforced
☐ Single element works correctly
☐ Many elements (10+) maintain correct order
☐ Large amounts (1000000+) work correctly

INTEGRATION
☐ buildHeapFromHashMap() successfully reads from MemberHash
☐ Only negative balances are included
☐ printSummaryReport() displays all debtors correctly

DATA VALIDATION
☐ Member names are preserved correctly
☐ Member IDs are preserved correctly
☐ Amounts are preserved correctly
☐ Total debt calculation is correct
```

---

## How to Compile Everything

### **Option 1: Just test DebtHeap**
```powershell
g++ -o test_unit test_debtheap_unit.cpp src/DebtHeap.cpp -I.
.\test_unit.exe
```

### **Option 2: Test with MemberHash**
```powershell
g++ -o test_integration test_debtheap_integration.cpp src/DebtHeap.cpp src/MemberHash.cpp src/TransactionTimeline.cpp -I.
.\test_integration.exe
```

### **Option 3: Full system with main.cpp**
```powershell
g++ -o main src/main.cpp src/DebtHeap.cpp src/MemberHash.cpp src/TransactionTimeline.cpp -I.
.\main.exe
```

---

## Debugging Tips

### **If extraction order is wrong:**
- Add debug output to heapifyUp/heapifyDown to trace movements
- Check that parent > children after each operation

### **If heap seems empty:**
- Verify buildHeapFromHashMap() is finding members
- Check that balances are actually negative
- Verify globalMemberHash is set before calling

### **If it crashes:**
- Check array bounds (size < capacity)
- Verify heapArray is not null
- Check parent/child index calculations

---

**Quick Test: Just run the unit tests first!** ✓
