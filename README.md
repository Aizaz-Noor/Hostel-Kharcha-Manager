# Hostel Kharcha Manager

A console-based hostel expense tracker built in C++ as a semester DSA project.  
Every module demonstrates a core data structure from scratch — no STL containers used for DSA logic.

---

## Team

| Member | Module | Data Structure |
|:---|:---|:---|
| Basit Shahid | DebtHeap | Max-Heap (Priority Queue) |
| Aizaz Noor | TransactionTimeline | Doubly Linked List + Stack |
| Abdul | MemberHash | Hash Map (Chaining) |

---

## Features

| Option | Feature | DSA Used |
|:---:|:---|:---|
| 1 | Add New Roommate | Hash Map insert |
| 2 | Log a Shared Expense | DLL append + Stack push |
| 3 | Undo Last Transaction | Stack pop + DLL delete |
| 4 | View Transaction Timeline | DLL traversal |
| 5 | View Roommate Balances | Hash Map full scan |
| 6 | Who Owes The Most? | Max-Heap extract |
| 7 | Exit | — |

---

## Project Structure

```
Hostel-kharcha-manager/
├── include/
│   ├── MemberHash.h          ← Hash Map class definition
│   ├── TransactionTimeline.h ← DLL + Stack class definition
│   └── DebtHeap.h            ← Max-Heap class definition
├── src/
│   ├── MemberHash.cpp        ← Hash Map implementation
│   ├── TransactionTimeline.cpp ← DLL + Stack implementation
│   ├── DebtHeap.cpp          ← Max-Heap implementation
│   └── main.cpp              ← Entry point + UI menu
├── DEBTHEAP_FUNCTIONALITY_GUIDE.md   ← Viva guide: DebtHeap
├── TRANSACTION_TIMELINE_VIVA_GUIDE.md ← Viva guide: TransactionTimeline
├── HostelKharchaManager_FormalProposal.docx
└── HKM.exe                   ← Pre-built Windows binary
```

---

## Build & Run

**Requires:** g++ with C++17 support (MinGW on Windows)

```bash
g++ -std=c++17 -o HKM.exe src/main.cpp src/MemberHash.cpp src/TransactionTimeline.cpp src/DebtHeap.cpp
./HKM.exe
```

---

## DSA Concepts Demonstrated

### Hash Map (MemberHash)
- Array of `TABLE_SIZE = 100` buckets
- **Collision resolution:** Separate chaining (linked list at each bucket)
- **Hash function:** Sum of ASCII values mod TABLE_SIZE
- Insert at **HEAD** of chain for O(1) insertion
- Average O(1) lookup, O(n) worst case

### Doubly Linked List (TransactionTimeline)
- `head` and `tail` pointers for O(1) append
- Bidirectional links (`prev`/`next`) enabling middle-node deletion in O(1)
- Full 4-case deletion: only node / head / tail / middle

### Stack (Undo Engine)
- Implemented as a singly linked list (no array, unbounded)
- Every new transaction is pushed; undo pops the top and removes it from the DLL
- LIFO: last transaction is always the first to be undone

### Max-Heap (DebtHeap)
- Dynamic array representing a complete binary tree
- `heapifyUp` on insert, `heapifyDown` on extract
- Always yields the **highest debtor first** via `extractMaxDebtor()`
- Rule of Three applied: copy constructor and assignment operator deleted
