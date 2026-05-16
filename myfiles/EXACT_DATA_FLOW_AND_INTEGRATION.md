# Exact Data Flow: How Components Should Work Together

## Real-World Scenario: Alice Pays 3000 for Pizza

### **The CORRECT Flow (What Should Happen)**

```
USER INPUT: "Alice pays 3000 for Pizza (3 people total)"
      │
      ▼
┌─────────────────────────────────────────────────────┐
│          main.cpp - Choice 2 Processing             │
├─────────────────────────────────────────────────────┤
│                                                     │
│  cin >> payerId       ← "A01" (Alice)               │
│  cin >> desc         ← "Pizza"                      │
│  cin >> amount       ← 3000                         │
│                                                     │
│  // Step 1: Record in Timeline [Member 2]           │
│  timeline.logTransaction(desc, amount, payerId);    │
│  └─→ Creates TransactionNode in DLL                 │
│      Transaction: {id:1, desc:"Pizza", amount:3000}│
│                                                     │
│  // Step 2: Update MemberHash [Member 1]            │
│  // Alice paid 3000, but also gets share:           │
│  //   She gets back: 3000/3 = 1000                  │
│  //   Net: She owes back: 3000 - 1000 = 2000       │
│  //   So: -2000 (she paid forward)                  │
│                                                     │
│  profiles.updateBalance("A01", -2000);              │
│  └─→ Alice.balance = 0 + (-2000) = -2000            │
│                                                     │
│  // Each other person owes their share:             │
│  profiles.updateBalance("B01", +1000);              │
│  └─→ Bob.balance = 0 + 1000 = +1000                 │
│                                                     │
│  profiles.updateBalance("C01", +1000);              │
│  └─→ Charlie.balance = 0 + 1000 = +1000             │
│                                                     │
└─────────────────────────────────────────────────────┘
      │
      │ (Results stored in two places:)
      │
      ├──────────────────────────────┬──────────────────────────────┐
      │                              │                              │
      ▼                              ▼                              │
┌──────────────────────────┐    ┌─────────────────────┐           │
│  TransactionTimeline DLL │    │    MemberHash       │           │
├──────────────────────────┤    ├─────────────────────┤           │
│ head ←→ [Txn 1] ←→ tail  │    │ table[...]:         │           │
│         Pizza            │    │ A01: Alice(-2000)   │           │
│         3000             │    │ B01: Bob(+1000)     │           │
│         by Alice         │    │ C01: Charlie(+1000) │           │
└──────────────────────────┘    └──────────┬──────────┘           │
                                           │                      │
                          Later (User asks: "Who owes most?")      │
                                           │                      │
                                           ▼                      │
                            ┌─────────────────────────┐           │
                            │  DebtHeap [Member 3]    │           │
                            ├─────────────────────────┤           │
                            │                         │           │
                            │ buildHeapFromHashMap(): │           │
                            │  ↓ Read from MemberHash │ ◄─────────┘
                            │  ↓ Find all with < 0    │
                            │  ↓ Only find Alice(-2k) │
                            │  ↓ Insert into heap     │
                            │                         │
                            │  Heap contains:         │
                            │  [Alice: 2000]          │
                            │                         │
                            │ extractMaxDebtor():     │
                            │  → Alice (owes 2000)    │
                            │                         │
                            └─────────────────────────┘
                                    │
                                    ▼
                            DISPLAY: "Alice owes 2000"
```

---

## Component Dependencies

```
┌──────────────────────────┐
│  TransactionTimeline     │
│  (Member 2)              │
│                          │
│  ✓ Self-contained        │
│  ✓ Just records events   │
│  ✓ No dependencies       │
└──────────┬───────────────┘
           │
           │ (needs main.cpp to call:)
           │
           ▼
┌──────────────────────────┐
│  MemberHash              │
│  (Member 1)              │
│                          │
│  ✓ Stores all balances   │
│  ✓ Main source of truth  │
│  ✓ No dependencies       │
└──────────┬───────────────┘
           │
           │ (DebtHeap reads from here:)
           │
           ▼
┌──────────────────────────┐
│  DebtHeap                │
│  (Member 3)              │
│                          │
│  ✗ Depends on MemberHash │
│  ✗ Must wait for data    │
│  ✓ Processes it          │
└──────────────────────────┘
```

---

## Current vs Required Code in main.cpp

### **Currently (BROKEN):**

```cpp
else if (choice == 2) {
    string payerId, desc;
    double amount;
    
    cout << "Who paid? : ";
    cin >> payerId;
    cout << "What for? : ";
    cin >> desc;
    cout << "Amount    : ";
    cin >> amount;
    
    timeline.logTransaction(desc, amount, payerId);  // ✓ Does this
    
    // ❌ MISSING: Balance updates!
    // profiles.updateBalance(???, ???);
    
    pause();
}
```

### **Should Be (CORRECT):**

```cpp
else if (choice == 2) {
    string payerId, desc;
    double amount;
    
    cout << "Who paid? : ";
    cin >> payerId;
    cout << "What for? : ";
    cin >> desc;
    cout << "Amount    : ";
    cin >> amount;
    
    // Step 1: Record transaction
    timeline.logTransaction(desc, amount, payerId);
    
    // Step 2: Update balances in MemberHash
    // Assume 3-person split for now (or make it dynamic)
    int numPeople = 3;
    double sharePerPerson = amount / numPeople;
    
    // Payer gets back their share amount
    profiles.updateBalance(payerId, -amount + sharePerPerson);
    
    // For each other person, add their share
    // NOTE: This is simplified - you'd need to loop through actual members
    profiles.updateBalance("B01", +sharePerPerson);
    profiles.updateBalance("C01", +sharePerPerson);
    
    cout << "Expense recorded and balances updated!\n";
    pause();
}
```

---

## What DebtHeap Should NEVER Do

```cpp
// ❌ WRONG - Don't read from Timeline:
void DebtHeap::buildHeapFromHashMap() {
    // Try to read transactions from timeline
    // Calculate balances manually
    // This is WRONG because:
    //   1. Timeline data is not meant for this
    //   2. Balance calculation logic is duplicated
    //   3. Defeats purpose of MemberHash
}

// ✓ CORRECT - Read from MemberHash:
void DebtHeap::buildHeapFromHashMap(MemberHash& memberData) {
    // Get current balances from MemberHash
    // Find those with negative balance
    // Insert into heap
    // This is RIGHT because:
    //   1. Balances already calculated correctly
    //   2. Single source of truth
    //   3. No duplication of logic
}
```

---

## The Three Roles - CLARITY

### **Member 1: MemberHash (Storage Layer)**
```
Job: Store member data and their CURRENT balances
├─ addMember(id, name)
├─ updateBalance(id, amount)
├─ getMember(id)
└─ printAllMembers()

Data stored: 
├─ Member ID
├─ Member Name
└─ Current Balance (sum of all their transactions)
```

### **Member 2: TransactionTimeline (History Layer)**
```
Job: Record WHAT transactions happened (audit trail)
├─ logTransaction(desc, amount, payerId)
├─ undoLastTransaction()
└─ printTimeline()

Data stored:
├─ Transaction ID
├─ Description
├─ Amount
├─ Payer ID
└─ DLL + Undo Stack for reversing

NOTE: Does NOT calculate balances!
```

### **Member 3: DebtHeap (Analytics Layer)**
```
Job: Find who owes the most money QUICKLY
├─ heapifyUp()
├─ heapifyDown()
├─ insertDebt()
├─ extractMaxDebtor()
├─ buildHeapFromHashMap(MemberHash& data) ← READ FROM MEMBER1
└─ printSummaryReport()

Data source: MemberHash (reads current balances)
NOT: TransactionTimeline

Data stored:
├─ Max-Heap of debtors only
└─ Only those with negative balance
```

---

## The Complete Flow: Step By Step

```
1. User selects "Add Roommate"
   └─→ profiles.addMember("A01", "Alice")
   └─→ Alice added to MemberHash with balance = 0

2. User selects "Add Roommate"
   └─→ profiles.addMember("B01", "Bob")
   └─→ Bob added to MemberHash with balance = 0

3. User selects "Log Expense" (Alice pays 3000)
   └─→ timeline.logTransaction("Pizza", 3000, "A01")
   │    └─ Transaction recorded in DLL
   │
   └─→ profiles.updateBalance("A01", -2000)
   │    └─ Alice.balance = -2000
   │
   └─→ profiles.updateBalance("B01", +1000)
        └─ Bob.balance = +1000

4. User selects "View Balances"
   └─→ profiles.printAllMembers()
   │    Display: Alice(-2000), Bob(+1000)

5. User selects "Who Owes Most?"
   └─→ analytics.buildHeapFromHashMap(profiles)
   │    ├─ Read from MemberHash
   │    ├─ Find Alice with -2000
   │    ├─ Insert into heap
   │    └─ Now Alice is in heap
   │
   └─→ analytics.extractMaxDebtor()
   │    └─ Returns Alice (2000)
   │
   └─→ Display: "Alice owes 2000"

6. User selects "Undo Last"
   └─→ timeline.undoLastTransaction()
   │    └─ Remove transaction from DLL
   │
   └─→ profiles.updateBalance("A01", +2000)
   │    └─ Alice.balance = 0 (reverted)
   │
   └─→ profiles.updateBalance("B01", -1000)
        └─ Bob.balance = 0 (reverted)
```

---

## The Missing Link in main.cpp

Currently when you choose option 6 ("Who Owes Most?"):

```cpp
else if (choice == 6) {
    cout << "This feature is handled by Role 3 (Debt Heap).\n";
    // ❌ Nothing is actually called!
}
```

**Should be:**

```cpp
else if (choice == 6) {
    cout << "\n  Building debt analysis...\n";
    
    // Build heap from current balances
    analytics.buildHeapFromHashMap(profiles);
    
    // Extract and display
    if (/* heap is not empty */) {
        DebtRecord topDebtor = analytics.extractMaxDebtor();
        cout << GREEN << "\n  Top Debtor:\n";
        cout << "  Name: " << topDebtor.memberName << "\n";
        cout << "  Owes: Rs. " << topDebtor.amountOwed << "\n" << RESET;
    } else {
        cout << GREEN << "\n  Great! Nobody owes money.\n" << RESET;
    }
}
```

---

## Your DebtHeap Implementation Path

```
Wait For:
├─ Member 1 to implement MemberHash completely
└─ main.cpp to call profiles.updateBalance()

Then:
├─ Your buildHeapFromHashMap() can read actual data
├─ Test with real balances
└─ Verify max-heap works correctly

Your Code Will Read:
├─ MemberHash.getAllMembers() [assuming this getter exists]
├─ Check member.balance < 0
└─ Insert into heap: insertDebt(id, name, -balance)
```

---

## TL;DR - Direct Answer to Your Question

### **Q: Does balance update in MemberHash based on transactions?**
✓ **YES** - Main.cpp should call `profiles.updateBalance()` when logging a transaction

### **Q: Where should my DebtHeap read from?**
✓ **MemberHash** - Read the current member balances
❌ **NOT TransactionTimeline** - That's just for audit trail

### **Q: Should I work on transactions or balances?**
✓ **Work on BALANCES** from MemberHash
❌ **Don't process transactions** - that's calculated by Member 1

### **What's Broken Now?**
❌ **main.cpp** doesn't call `profiles.updateBalance()` after logging expense
❌ So MemberHash has no real data
❌ So your DebtHeap has nothing to work with

**Your Next Step:**
1. Wait for Member 1 to implement MemberHash
2. Implement your DebtHeap using [DEBTHEAD_CODE_SKELETON.cpp](DEBTHEAD_CODE_SKELETON.cpp)
3. Test with `buildHeapFromHashMap(profiles)` reading actual balance data
