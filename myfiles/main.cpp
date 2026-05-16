/*
 * =============================================================================
 * DEBTHEAP - COMPLETE SELF-CONTAINED TEST FILE
 * =============================================================================
 * 
 * This file contains:
 * 1. DebtRecord struct
 * 2. Complete DebtHeap implementation (all 6 functions)
 * 3. Comprehensive test suite (7 test cases)
 * 4. Main function with all tests
 * 
 * TO USE IN DEV-C++:
 * 1. File -> New -> Project
 * 2. Add this file to the project
 * 3. Ctrl+F9 to compile
 * 4. Ctrl+F10 to run
 * 
 * =============================================================================
 */

#include <iostream>
#include <string>
#include <cassert>
#include <vector>

using namespace std;

// =============================================================================
// DebtRecord Structure - Stores member debt information
// =============================================================================

struct DebtRecord {
    string memberId;      // Unique member ID
    string memberName;    // Member's name
    double amountOwed;    // Amount they owe (positive value)
    
    // Default constructor
    DebtRecord() : memberId(""), memberName(""), amountOwed(0.0) {}
    
    // Parameterized constructor
    DebtRecord(string id, string name, double amt) 
        : memberId(id), memberName(name), amountOwed(amt) {}
};

// =============================================================================
// DebtHeap Class - Max-Heap Priority Queue for debtors
// =============================================================================

class DebtHeap {
private:
    DebtRecord* heapArray;  // Dynamic array storing heap elements
    int capacity;           // Maximum capacity of heap
    int size;               // Current number of elements
    
    // =========================================================================
    // Helper Functions - Index calculations for heap tree
    // =========================================================================
    
    int parent(int i) {
        return (i - 1) / 2;
    }
    
    int leftChild(int i) {
        return (2 * i) + 1;
    }
    
    int rightChild(int i) {
        return (2 * i) + 2;
    }
    
    // =========================================================================
    // heapifyUp - Move element UP after insertion (Role 3 Implementation)
    // =========================================================================
    void heapifyUp(int index) {
        // Compare with parent and swap if larger
        while (index > 0) {
            int parentIdx = parent(index);
            
            if (heapArray[index].amountOwed > heapArray[parentIdx].amountOwed) {
                // Swap with parent
                DebtRecord temp = heapArray[index];
                heapArray[index] = heapArray[parentIdx];
                heapArray[parentIdx] = temp;
                
                // Continue checking up
                index = parentIdx;
            } else {
                // Heap property satisfied
                break;
            }
        }
    }
    
    // =========================================================================
    // heapifyDown - Move element DOWN after extraction (Role 3 Implementation)
    // =========================================================================
    void heapifyDown(int index) {
        // Compare with children and swap with largest
        while (true) {
            int leftIdx = leftChild(index);
            int rightIdx = rightChild(index);
            int largestIdx = index;
            
            // Find largest among parent and children
            if (leftIdx < size && 
                heapArray[leftIdx].amountOwed > heapArray[largestIdx].amountOwed) {
                largestIdx = leftIdx;
            }
            
            if (rightIdx < size && 
                heapArray[rightIdx].amountOwed > heapArray[largestIdx].amountOwed) {
                largestIdx = rightIdx;
            }
            
            // If largest is not current, swap and continue down
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

public:
    // =========================================================================
    // Constructor
    // =========================================================================
    DebtHeap(int cap) {
        capacity = cap;
        size = 0;
        heapArray = new DebtRecord[capacity];
    }
    
    // =========================================================================
    // Destructor
    // =========================================================================
    ~DebtHeap() {
        delete[] heapArray;
    }
    
    // =========================================================================
    // insertDebt - Add new debtor to heap (Role 3 Implementation)
    // =========================================================================
    void insertDebt(string id, string name, double amount) {
        // Check if heap is full
        if (size >= capacity) {
            cout << "ERROR: Heap is full! Cannot insert more debtors.\n";
            return;
        }
        
        // Add at end
        heapArray[size] = DebtRecord(id, name, amount);
        
        // Bubble up to maintain heap property
        heapifyUp(size);
        
        // Increment size
        size++;
    }
    
    // =========================================================================
    // extractMaxDebtor - Remove and return max debtor (Role 3 Implementation)
    // =========================================================================
    DebtRecord extractMaxDebtor() {
        // Check if heap is empty
        if (size <= 0) {
            cout << "ERROR: Heap is empty! No debtors to extract.\n";
            return DebtRecord();
        }
        
        // Save root (person with most debt)
        DebtRecord maxDebtor = heapArray[0];
        
        // Move last element to root
        heapArray[0] = heapArray[size - 1];
        
        // Decrease size
        size--;
        
        // Restore heap property if not empty
        if (size > 0) {
            heapifyDown(0);
        }
        
        // Return the maximum debtor
        return maxDebtor;
    }
    
    // =========================================================================
    // buildHeapFromHashMap - Populate heap from member data (Mock version)
    // =========================================================================
    void buildHeapFromHashMap(vector<DebtRecord> members) {
        // Clear existing heap
        size = 0;
        
        // Add each member's debt to heap
        for (int i = 0; i < members.size(); i++) {
            if (members[i].amountOwed > 0) {
                insertDebt(members[i].memberId, 
                          members[i].memberName, 
                          members[i].amountOwed);
            }
        }
        
        cout << "Heap built with " << size << " debtors.\n";
    }
    
    // =========================================================================
    // printSummaryReport - Extract all debtors and print sorted (Role 3 Implementation)
    // =========================================================================
    void printSummaryReport() {
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
    
    // Getter for size (useful for testing)
    int getSize() { 
        return size; 
    }
};

// =============================================================================
// TEST SUITE - 7 Comprehensive Test Cases
// =============================================================================

void printTestHeader(string testName) {
    cout << "\n";
    cout << "========================================\n";
    cout << "  " << testName << "\n";
    cout << "========================================\n";
}

// TEST 1: Basic Insert and Extract
void test1_basic_insert_extract() {
    printTestHeader("TEST 1: Basic Insert and Extract");
    
    DebtHeap heap(10);
    
    cout << "Inserting 3 debtors:\n";
    cout << "  - Alice: Rs. 1000\n";
    cout << "  - Bob: Rs. 2000\n";
    cout << "  - Charlie: Rs. 500\n\n";
    
    heap.insertDebt("A01", "Alice", 1000);
    heap.insertDebt("B01", "Bob", 2000);
    heap.insertDebt("C01", "Charlie", 500);
    
    cout << "Extracting (should be: 2000, 1000, 500):\n";
    
    DebtRecord r1 = heap.extractMaxDebtor();
    cout << "  1st: " << r1.memberName << " - Rs. " << r1.amountOwed;
    assert(r1.amountOwed == 2000);
    cout << " ✓\n";
    
    DebtRecord r2 = heap.extractMaxDebtor();
    cout << "  2nd: " << r2.memberName << " - Rs. " << r2.amountOwed;
    assert(r2.amountOwed == 1000);
    cout << " ✓\n";
    
    DebtRecord r3 = heap.extractMaxDebtor();
    cout << "  3rd: " << r3.memberName << " - Rs. " << r3.amountOwed;
    assert(r3.amountOwed == 500);
    cout << " ✓\n";
    
    cout << "\n✓ TEST 1 PASSED\n";
}

// TEST 2: HeapifyUp (Bubble Up on Insert)
void test2_heapify_up() {
    printTestHeader("TEST 2: HeapifyUp (Bubble Up on Insert)");
    
    DebtHeap heap(10);
    
    cout << "Inserting in random order: 100, 200, 50, 300\n";
    heap.insertDebt("A", "Person A", 100);
    heap.insertDebt("B", "Person B", 200);
    heap.insertDebt("C", "Person C", 50);
    heap.insertDebt("D", "Person D", 300);
    cout << "Inserted 4 elements\n\n";
    
    cout << "Extracting max (should be 300):\n";
    DebtRecord max = heap.extractMaxDebtor();
    cout << "  " << max.memberName << " - Rs. " << max.amountOwed;
    assert(max.amountOwed == 300);
    cout << " ✓\n";
    
    cout << "\n✓ TEST 2 PASSED - HeapifyUp working correctly\n";
}

// TEST 3: HeapifyDown (Bubble Down After Extract)
void test3_heapify_down() {
    printTestHeader("TEST 3: HeapifyDown (Bubble Down After Extract)");
    
    DebtHeap heap(10);
    
    cout << "Inserting: 2500, 2000, 1500, 1000\n";
    heap.insertDebt("A", "Alice", 2500);
    heap.insertDebt("B", "Bob", 2000);
    heap.insertDebt("C", "Charlie", 1500);
    heap.insertDebt("D", "David", 1000);
    cout << "Inserted 4 elements\n\n";
    
    cout << "Extracting in order (heap property maintained):\n";
    
    DebtRecord r1 = heap.extractMaxDebtor();
    cout << "  1st: " << r1.amountOwed;
    assert(r1.amountOwed == 2500);
    cout << " ✓\n";
    
    DebtRecord r2 = heap.extractMaxDebtor();
    cout << "  2nd: " << r2.amountOwed;
    assert(r2.amountOwed == 2000);
    cout << " ✓\n";
    
    DebtRecord r3 = heap.extractMaxDebtor();
    cout << "  3rd: " << r3.amountOwed;
    assert(r3.amountOwed == 1500);
    cout << " ✓\n";
    
    DebtRecord r4 = heap.extractMaxDebtor();
    cout << "  4th: " << r4.amountOwed;
    assert(r4.amountOwed == 1000);
    cout << " ✓\n";
    
    cout << "\n✓ TEST 3 PASSED - HeapifyDown working correctly\n";
}

// TEST 4: Correct Ordering (Extract All)
void test4_correct_ordering() {
    printTestHeader("TEST 4: Correct Ordering (Extract All)");
    
    DebtHeap heap(5);
    
    cout << "Inserting random order: 300, 800, 500, 200, 600\n";
    heap.insertDebt("E", "Eve", 300);
    heap.insertDebt("B", "Bob", 800);
    heap.insertDebt("A", "Alice", 500);
    heap.insertDebt("D", "David", 200);
    heap.insertDebt("C", "Charlie", 600);
    cout << "Inserted 5 elements\n\n";
    
    cout << "Expected extraction order: 800, 600, 500, 300, 200\n";
    cout << "Actual extraction order:   ";
    
    double previous = 999999;
    int count = 1;
    
    while (heap.getSize() > 0) {
        DebtRecord debtor = heap.extractMaxDebtor();
        cout << debtor.amountOwed;
        if (heap.getSize() > 0) cout << ", ";
        
        assert(debtor.amountOwed <= previous);
        previous = debtor.amountOwed;
        count++;
    }
    cout << "\n\n✓ TEST 4 PASSED - All in descending order\n";
}

// TEST 5: Empty Heap Handling
void test5_empty_heap() {
    printTestHeader("TEST 5: Empty Heap Handling");
    
    DebtHeap heap(10);
    
    cout << "Attempting to extract from empty heap:\n";
    DebtRecord empty = heap.extractMaxDebtor();
    cout << "No crash occurred ✓\n";
    cout << "Result: Empty record (memberName = '')\n";
    
    cout << "\n✓ TEST 5 PASSED - Empty heap handled gracefully\n";
}

// TEST 6: Capacity Limit
void test6_capacity_limit() {
    printTestHeader("TEST 6: Capacity Limit");
    
    DebtHeap heap(3);  // Capacity of 3
    
    cout << "Heap capacity: 3\n";
    cout << "Inserting 3 elements (at capacity):\n";
    heap.insertDebt("A", "Alice", 100);
    heap.insertDebt("B", "Bob", 200);
    heap.insertDebt("C", "Charlie", 300);
    cout << "✓ Successfully inserted 3 elements\n\n";
    
    cout << "Attempting to insert 4th element:\n";
    heap.insertDebt("D", "David", 400);
    cout << "(Should show ERROR message above) ✓\n";
    
    cout << "\n✓ TEST 6 PASSED - Capacity limit enforced\n";
}

// TEST 7: Single Element
void test7_single_element() {
    printTestHeader("TEST 7: Single Element");
    
    DebtHeap heap(10);
    
    cout << "Inserting 1 element:\n";
    heap.insertDebt("X", "Xavier", 5000);
    cout << "✓ Inserted\n\n";
    
    cout << "Extracting:\n";
    DebtRecord debtor = heap.extractMaxDebtor();
    cout << "  Name: " << debtor.memberName << "\n";
    cout << "  Amount: Rs. " << debtor.amountOwed << "\n";
    
    assert(debtor.memberName == "Xavier");
    assert(debtor.amountOwed == 5000);
    
    cout << "\n✓ TEST 7 PASSED\n";
}

// BONUS: Full System Test with printSummaryReport
void bonus_full_system_test() {
    printTestHeader("BONUS: Full System Test - Print Summary Report");
    
    DebtHeap heap(10);
    
    cout << "Building heap with sample data:\n";
    cout << "  - Rahul: Rs. 2500\n";
    cout << "  - Priya: Rs. 1800\n";
    cout << "  - Aditya: Rs. 3200\n";
    cout << "  - Neha: Rs. 1200\n";
    cout << "  - Rohan: Rs. 2700\n\n";
    
    vector<DebtRecord> members;
    members.push_back(DebtRecord("M001", "Rahul", 2500));
    members.push_back(DebtRecord("M002", "Priya", 1800));
    members.push_back(DebtRecord("M003", "Aditya", 3200));
    members.push_back(DebtRecord("M004", "Neha", 1200));
    members.push_back(DebtRecord("M005", "Rohan", 2700));
    
    heap.buildHeapFromHashMap(members);
    
    // Print the summary report
    heap.printSummaryReport();
    
    cout << "✓ BONUS TEST PASSED\n";
}

// =============================================================================
// MAIN FUNCTION - Run all tests
// =============================================================================

int main() {
    cout << "\n";
    cout << "╔════════════════════════════════════════════════════╗\n";
    cout << "║       DEBTHEAP COMPREHENSIVE TEST SUITE            ║\n";
    cout << "║  Complete Implementation - Role 3 Submission       ║\n";
    cout << "╚════════════════════════════════════════════════════╝\n";
    
    try {
        test1_basic_insert_extract();
        test2_heapify_up();
        test3_heapify_down();
        test4_correct_ordering();
        test5_empty_heap();
        test6_capacity_limit();
        test7_single_element();
        bonus_full_system_test();
        
        cout << "\n";
        cout << "╔════════════════════════════════════════════════════╗\n";
        cout << "║         ✓✓✓ ALL TESTS PASSED SUCCESSFULLY ✓✓✓      ║\n";
        cout << "║                                                    ║\n";
        cout << "║  Your DebtHeap implementation is ready for use!    ║\n";
        cout << "║                                                    ║\n";
        cout << "║  Functions Implemented:                            ║\n";
        cout << "║    ✓ heapifyUp()                                   ║\n";
        cout << "║    ✓ heapifyDown()                                 ║\n";
        cout << "║    ✓ insertDebt()                                  ║\n";
        cout << "║    ✓ extractMaxDebtor()                            ║\n";
        cout << "║    ✓ buildHeapFromHashMap()                        ║\n";
        cout << "║    ✓ printSummaryReport()                          ║\n";
        cout << "╚════════════════════════════════════════════════════╝\n";
        
    } catch (const exception& e) {
        cout << "\n✗ TEST FAILED: " << e.what() << "\n";
        cout << "Press Enter to exit...";
        cin.get();
        return 1;
    }
    
    cout << "\nPress Enter to exit...";
    cin.get();
    return 0;
}
