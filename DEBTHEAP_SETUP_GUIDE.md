# DEBTHEAP SETUP & INTEGRATION GUIDE

**For Project Completion - How to Use DebtHeap in main.cpp**

---

## 📋 TABLE OF CONTENTS

1. [Quick Setup (3 steps)](#quick-setup)
2. [Step-by-Step Integration Guide](#step-by-step-guide)
3. [Code Examples](#code-examples)
4. [Testing After Integration](#testing)
5. [Troubleshooting](#troubleshooting)

---

## 🚀 QUICK SETUP (3 STEPS)

### **Step 1: Add Global Variable (Top of main.cpp)**
```cpp
#include <iostream>
#include "../include/MemberHash.h"
#include "../include/TransactionTimeline.h"
#include "../include/DebtHeap.h"

using namespace std;

// ← ADD THIS LINE
MemberHash* globalMemberHash = nullptr;
```

### **Step 2: Initialize in main() Function**
```cpp
int main() {
    MemberHash profiles(10);
    TransactionTimeline timeline;
    DebtHeap analytics(100);
    
    // ← SET GLOBAL REFERENCE (CRITICAL!)
    globalMemberHash = &profiles;
    
    // Rest of your code...
}
```

### **Step 3: Call in Menu Option 6**
```cpp
else if (choice == 6) {
    clearScreen();
    cout << MAGENTA << "\n  ╔══════════════════════════════════╗\n";
    cout            << "  ║      TOP DEBTOR (MAX-HEAP)       ║\n";
    cout            << "  ╚══════════════════════════════════╝\n" << RESET;
    
    // ← ADD THESE TWO LINES
    analytics.buildHeapFromHashMap();
    analytics.printSummaryReport();
    
    pause();
}
```

**DONE!** That's all you need! ✅

---

## 📖 STEP-BY-STEP INTEGRATION GUIDE

### **Complete Integration Checklist**

```
☐ STEP 1: Update includes (add DebtHeap.h)
☐ STEP 2: Declare global variable (top of file)
☐ STEP 3: Create analytics object (in main)
☐ STEP 4: Set global reference (in main)
☐ STEP 5: Update choice 6 menu option
☐ STEP 6: Compile and test
☐ STEP 7: Verify output format
```

---

### **STEP 1: Add Include**

**File:** `src/main.cpp`  
**Location:** Top of file with other includes

```cpp
#include <iostream>
#include <string>
#include <vector>
#include "../include/MemberHash.h"
#include "../include/TransactionTimeline.h"
#include "../include/DebtHeap.h"          // ← ADD THIS

using namespace std;
```

---

### **STEP 2: Declare Global Variable**

**File:** `src/main.cpp`  
**Location:** After includes, before any functions

```cpp
using namespace std;

// ── Global Variables ────────────────────────
MemberHash* globalMemberHash = nullptr;    // ← ADD THIS
DebtHeap* globalDebtHeap = nullptr;        // (Optional)

// ── Colors ──────────────────────────────────
const string RESET   = "\033[0m";
// ... rest of colors
```

**Why?** DebtHeap's `buildHeapFromHashMap()` needs access to member data.

---

### **STEP 3: Initialize Objects in main()**

**File:** `src/main.cpp`  
**Location:** Inside main() function

```cpp
int main() {
    // Initialize all components
    MemberHash profiles(10);              // Member hash table
    TransactionTimeline timeline;         // Transaction log
    DebtHeap analytics(100);              // Debt heap (max 100 debtors)
    
    // ← CRITICAL: Set global references
    globalMemberHash = &profiles;
    
    int choice = 0;
    
    while (choice != 7) {
        // ... rest of menu loop
    }
    
    return 0;
}
```

**Why capacity 100?** Max 100 members can be in debt at once. Adjust if needed.

---

### **STEP 4: Update Menu Option 6**

**File:** `src/main.cpp`  
**Location:** Inside the while loop, at `else if (choice == 6):`

```cpp
else if (choice == 6) {
    clearScreen();
    cout << MAGENTA << "\n  ╔══════════════════════════════════╗\n";
    cout            << "  ║      TOP DEBTOR (MAX-HEAP)       ║\n";
    cout            << "  ╚══════════════════════════════════╝\n" << RESET;
    
    // ── REPLACE THE PLACEHOLDER WITH THIS: ──
    
    cout << "\n  Building debt heap from member data...\n";
    analytics.buildHeapFromHashMap();
    
    cout << "\n  Generating debt report...\n";
    analytics.printSummaryReport();
    
    // ────────────────────────────────────────
    
    pause();
}
```

**What this does:**
1. `buildHeapFromHashMap()` - Reads all members from MemberHash, filters those with negative balance (debt), **converts to positive amounts** (removes minus sign), builds max-heap
2. `printSummaryReport()` - Extracts all debtors in order (highest first) and displays report with positive amounts

---

## 💻 CODE EXAMPLES

### **Example 1: Basic Integration**

```cpp
#include <iostream>
#include "../include/DebtHeap.h"
#include "../include/MemberHash.h"

using namespace std;

MemberHash* globalMemberHash = nullptr;

int main() {
    // Create objects
    MemberHash profiles(10);
    DebtHeap analytics(100);
    
    // Set global
    globalMemberHash = &profiles;
    
    // Use DebtHeap
    cout << "Analyzing debts...\n";
    analytics.buildHeapFromHashMap();
    analytics.printSummaryReport();
    
    return 0;
}
```

---

### **Example 2: Full Menu Integration**

```cpp
else if (choice == 6) {
    clearScreen();
    
    // Header
    cout << MAGENTA << "\n  ╔══════════════════════════════════╗\n";
    cout            << "  ║      WHO OWES THE MOST?          ║\n";
    cout            << "  ║      (Max-Heap Debt Ranking)     ║\n";
    cout            << "  ╚══════════════════════════════════╝\n" << RESET;
    
    // Check if anyone exists
    if (profiles.isEmpty()) {
        cout << "\n" << RED << "  No members added yet!\n" << RESET;
    } else if (profiles.noDebts()) {
        cout << "\n" << GREEN << "  Everyone settled! No debts.\n" << RESET;
    } else {
        // Build and display
        cout << "\n" << CYAN << "  Analyzing member balances...\n" << RESET;
        analytics.buildHeapFromHashMap();
        analytics.printSummaryReport();
    }
    
    pause();
}
```

---

## 🧪 TESTING AFTER INTEGRATION

### **Test Case 1: Empty System**
```
Input: Run program, go to option 6
Expected: "No members added yet!" OR "Everyone settled!"
Status: ✓ Should handle gracefully
```

### **Test Case 2: No Debts**
```
Input: Add members, create equal expenses all pay
Expected: "Great news! No one owes any money."
Status: ✓ Should show positive message
```

### **Test Case 3: With Debts**
```
Input: Add 3 members, Member A pays 1000, Member B pays 500
Expected: List showing who owes how much, sorted highest first
Status: ✓ Should display properly formatted table
```

### **Test Case 4: Large Numbers**
```
Input: Test with large amounts (10000+)
Expected: Correct sorting, accurate totals
Status: ✓ Should handle large values
```

---

## 🔧 TROUBLESHOOTING

### **Problem 1: "globalMemberHash not declared"**
```
Cause: Forgot to add global variable declaration
Fix: Add at top of main.cpp:
    MemberHash* globalMemberHash = nullptr;
```

### **Problem 2: "buildHeapFromHashMap not found"**
```
Cause: Missing #include <vector> in DebtHeap.h
Fix: Add to DebtHeap.h:
    #include <vector>
```

### **Problem 2b: "Understanding the conversion"**
```
Question: Why are amounts positive in report if balance is negative?
Answer: 
    - MemberHash stores balance as NEGATIVE (e.g., -500)
    - buildHeapFromHashMap() CONVERTS to POSITIVE (e.g., +500)
    - Conversion formula: amountOwed = -(balance)
    - Example: -(−500) = +500
    
This makes the report user-friendly (show debt as positive numbers)
```

### **Problem 3: Heap shows no debtors**
```
Cause: MemberHash methods not implemented (member1's job)
Fix: Wait for Member 1 to implement:
    - addMember()
    - updateBalance()
    - getAllMembers()
```

### **Problem 4: Segmentation fault**
```
Cause: globalMemberHash not initialized
Fix: Add in main():
    globalMemberHash = &profiles;
    (before using analytics)
```

### **Problem 5: Compilation error "missing methods"**
```
Cause: MemberHash missing getAllMembers()
Fix: Member 1 needs to add:
    vector<MemberNode*> getAllMembers();
```

---

## 📊 EXPECTED OUTPUT

### **When option 6 is selected:**

```
╔══════════════════════════════════╗
║      TOP DEBTOR (MAX-HEAP)       ║
╚══════════════════════════════════╝

Analyzing member balances...
Heap built with 3 debtors.

==================================================
                DEBT SUMMARY REPORT
==================================================

Members sorted by amount owed (highest first):

1. Omar (M005): Rs. 660
2. Hassan (M003): Rs. 560
3. Ayesha (M002): Rs. 360

==================================================
Total Amount Owed: Rs. 1580
==================================================

Press Enter to continue...
```

---

## ✅ FINAL CHECKLIST

Before submitting, verify:

```
☐ DebtHeap.h is in include/ folder
☐ DebtHeap.cpp is in src/ folder
☐ main.cpp includes DebtHeap.h
☐ Global variable declared: MemberHash* globalMemberHash = nullptr;
☐ Global initialized in main(): globalMemberHash = &profiles;
☐ Menu option 6 calls:
    - analytics.buildHeapFromHashMap();
    - analytics.printSummaryReport();
☐ Compiles without errors
☐ Runs without crashes
☐ Displays proper debt ranking
```

---

## 🎯 SUMMARY

| Step | Action | File | Status |
|------|--------|------|--------|
| 1 | Add include | main.cpp | ✅ |
| 2 | Global variable | main.cpp | ✅ |
| 3 | Initialize in main | main.cpp | ✅ |
| 4 | Set global reference | main.cpp | ✅ |
| 5 | Update menu option 6 | main.cpp | ✅ |
| 6 | Compile | Terminal | ✅ |
| 7 | Test | Terminal | ✅ |

**Time to Complete:** 10-15 minutes  
**Difficulty:** Easy  
**Risk:** Low (no changes to existing code except menu)

---

**Ready to integrate?** Follow the 3-step quick setup above! 🚀
