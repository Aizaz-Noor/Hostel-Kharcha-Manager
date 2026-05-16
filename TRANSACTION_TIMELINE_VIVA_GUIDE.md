# Transaction Timeline — Complete Viva Guide
### Hostel Kharcha Manager | DSA Semester Project
### Your Part: Role 2 — Transaction History & Undo Engine

---

## WHAT IS MY PART?

My part is the **Transaction Timeline**. Every time someone in the hostel pays for something (groceries, electricity, rent), that transaction is recorded in a **Doubly Linked List**. If someone makes a mistake, they can **Undo** it using a **Stack**.

**Files I wrote:**
- `include/TransactionTimeline.h` — the class definition and node structures
- `src/TransactionTimeline.cpp` — the full implementation

**DSA structures used:**
1. **Doubly Linked List (DLL)** — stores all transactions in order
2. **Stack (using Linked List)** — remembers the last added transaction so we can undo it

---

## THE NODES (Building Blocks)

### TransactionNode — one record in the DLL

```cpp
struct TransactionNode {
    int transactionId;      // unique number like 1, 2, 3...
    string description;     // "Groceries", "Electricity Bill"
    double amount;          // Rs. 500.00
    string payerId;         // who paid, e.g. "A01"

    TransactionNode* prev;  // points to the PREVIOUS transaction
    TransactionNode* next;  // points to the NEXT transaction
};
```

> **Why prev AND next?**
> Because it is a DOUBLY linked list. With both pointers, we can go forward AND backward through the list without searching.

### StackNode — one entry in the Undo Stack

```cpp
struct StackNode {
    TransactionNode* transactionRef;  // address of the transaction to undo
    StackNode* next;                  // pointer to the stack node below this one
};
```

> The stack does NOT copy the transaction. It only stores the **memory address** (pointer) of it.

---

## THE CLASS

```cpp
class TransactionTimeline {
private:
    TransactionNode* head;        // start of the DLL
    TransactionNode* tail;        // end of the DLL
    StackNode* undoStackTop;      // top of the undo stack
    int transactionCounter;       // auto-incrementing ID

public:
    TransactionTimeline();
    ~TransactionTimeline();
    void logTransaction(string desc, double amount, string payerId);
    void undoLastTransaction();
    void printTimeline();
};
```

---

## FUNCTION 1 — logTransaction()

**What it does:** Adds a new transaction.

**DSA Steps:**
1. Create a new TransactionNode
2. Append it to the **TAIL** of the Doubly Linked List
3. Push its address onto the **Stack**

```cpp
void TransactionTimeline::logTransaction(string desc, double amount, string payerId) {

    // Step 1: Create new node
    TransactionNode* newNode = new TransactionNode(transactionCounter, desc, amount, payerId);
    transactionCounter++;

    // Step 2: Add to DLL tail
    if (head == nullptr) {
        head = newNode;   // first node — it is both head and tail
        tail = newNode;
    } else {
        newNode->prev = tail;   // new node's prev = old tail
        tail->next = newNode;   // old tail's next = new node
        tail = newNode;         // update tail
    }

    // Step 3: Push onto Stack
    StackNode* stackNode = new StackNode(newNode);
    stackNode->next = undoStackTop;
    undoStackTop = stackNode;
}
```

**Visual — DLL after adding 3 transactions:**
```
HEAD
 |
[Txn#1 | Groceries | Rs.200] <--> [Txn#2 | Electricity | Rs.500] <--> [Txn#3 | Rent | Rs.3000]
                                                                          |
                                                                         TAIL

Stack (top to bottom):
  [-> Txn#3]
  [-> Txn#2]
  [-> Txn#1]
```

**Time Complexity:** O(1) — We always append to the tail. We have a direct pointer to tail so no traversal needed.

**Space Complexity:** O(1) per call — one new DLL node + one new Stack node.

---

## FUNCTION 2 — undoLastTransaction()

**What it does:** Removes the most recently added transaction.

**DSA Steps:**
1. **Pop** from the Stack — get which transaction to remove
2. Handle **4 cases** to remove that node from the DLL
3. Free the memory

```cpp
void TransactionTimeline::undoLastTransaction() {

    if (undoStackTop == nullptr) {
        cout << "Nothing to undo!\n";
        return;
    }

    // Step 1: Pop from stack
    StackNode* topStack = undoStackTop;
    TransactionNode* toRemove = topStack->transactionRef;
    undoStackTop = undoStackTop->next;
    delete topStack;

    // Step 2: Remove from DLL (4 cases)
    if (toRemove->prev == nullptr && toRemove->next == nullptr) {
        // Only node
        head = nullptr;
        tail = nullptr;
    }
    else if (toRemove->prev == nullptr) {
        // It is the head
        head = toRemove->next;
        head->prev = nullptr;
    }
    else if (toRemove->next == nullptr) {
        // It is the tail
        tail = toRemove->prev;
        tail->next = nullptr;
    }
    else {
        // It is in the middle
        toRemove->prev->next = toRemove->next;
        toRemove->next->prev = toRemove->prev;
    }

    // Step 3: Free memory
    delete toRemove;
}
```

**The 4 DLL Deletion Cases (draw this in viva):**

```
CASE 1 — Only node:
  [Txn#1]   →  head = null, tail = null

CASE 2 — Head node:
  [Txn#1] <-> [Txn#2]   →   head now points to Txn#2, Txn#2->prev = null

CASE 3 — Tail node:
  [Txn#1] <-> [Txn#2]   →   tail now points to Txn#1, Txn#1->next = null

CASE 4 — Middle node:
  [Txn#1] <-> [Txn#2] <-> [Txn#3]
  Remove Txn#2:
  Txn#1->next = Txn#3
  Txn#3->prev = Txn#1
  Result: [Txn#1] <-> [Txn#3]
```

**Time Complexity:** O(1) — Because each node already has its own prev and next pointers, we can unlink it instantly without searching the list.

> This is the KEY advantage of DLL over a singly linked list. In a singly linked list you would need O(n) to find the previous node.

---

## FUNCTION 3 — printTimeline()

**What it does:** Traverses the DLL from head to tail and prints all transactions as a table.

```cpp
void TransactionTimeline::printTimeline() {

    if (head == nullptr) {
        cout << "No transactions recorded yet.\n";
        return;
    }

    // Print header row
    cout << "  +----+----------------------------+-------------+--------------+\n";
    cout << "  | #  | Description                | Amount      | Paid By      |\n";
    cout << "  +----+----------------------------+-------------+--------------+\n";

    // Traverse DLL using next pointers
    TransactionNode* current = head;
    while (current != nullptr) {
        // print the row...
        current = current->next;   // move to next node
    }

    cout << "  Direction: HEAD --> [next] --> [next] --> TAIL\n";
}
```

**Time Complexity:** O(n) — We visit every node once.

---

## DESTRUCTOR — ~TransactionTimeline()

**Why we need it:** We used `new` to allocate memory. If we do not `delete` it, the program will have a **memory leak** — the RAM stays used even after the program ends.

```cpp
TransactionTimeline::~TransactionTimeline() {
    // Free all DLL nodes
    TransactionNode* current = head;
    while (current != nullptr) {
        TransactionNode* temp = current->next;
        delete current;
        current = temp;
    }

    // Free all Stack nodes
    StackNode* stackCurrent = undoStackTop;
    while (stackCurrent != nullptr) {
        StackNode* temp = stackCurrent->next;
        delete stackCurrent;
        stackCurrent = temp;
    }
}
```

> **Important:** We save `current->next` into `temp` BEFORE deleting, because after `delete` the pointer is invalid and we cannot access `->next` anymore.

---

## EXPECTED VIVA QUESTIONS & ANSWERS

---

### SECTION A — Basic Concepts

**Q: What is a Doubly Linked List?**
> A doubly linked list is a linear data structure where each node has three parts: data, a pointer to the next node, and a pointer to the previous node. This allows traversal in both directions — forward and backward.

**Q: What is the difference between a Singly Linked List and a Doubly Linked List?**
| Feature | Singly Linked List | Doubly Linked List |
|---|---|---|
| Pointers per node | 1 (next only) | 2 (next + prev) |
| Traversal | Forward only | Both directions |
| Deletion without search | Not possible | Possible in O(1) |
| Memory per node | Less | More |

**Q: What is a Stack?**
> A stack is a linear data structure that follows the LIFO principle — Last In, First Out. The last element added is the first one removed. Main operations are Push (add) and Pop (remove).

**Q: What is LIFO?**
> Last In, First Out. Like a stack of plates — you put the newest plate on top, and you always take from the top first.

**Q: Why did you use a Stack for undo instead of a Queue?**
> Because undo always removes the MOST RECENT action. A Stack (LIFO) does exactly this. A Queue is FIFO — it would remove the OLDEST transaction first, which is wrong for undo.

---

### SECTION B — Implementation Questions

**Q: Why did you append to the TAIL and not the HEAD in logTransaction?**
> Because transactions should appear in chronological order. If I add to the head, the newest transaction would always appear first (reverse order). Adding to the tail keeps them in the order they happened.

**Q: Why do you keep a separate `tail` pointer?**
> To make appending O(1). Without a tail pointer, I would have to traverse the entire list every time just to find the last node, making it O(n). With `tail`, I go directly to the end in O(1).

**Q: Why does the Stack store a POINTER to the TransactionNode instead of a copy?**
> To save memory and keep things consistent. If we stored a copy, there would be two copies of the same data — one in the DLL and one in the Stack. Using a pointer, both the DLL and Stack refer to the same single node in memory.

**Q: How many cases are there when deleting a node from a DLL?**
> Four cases:
> 1. The list has only one node
> 2. The node to delete is the head
> 3. The node to delete is the tail
> 4. The node to delete is in the middle

**Q: Why do we need all 4 cases? Can we combine them?**
> Cases 2, 3, and 4 can sometimes be partially combined, but each has different pointer adjustments. For a viva, it is cleaner to handle them separately so the logic is clear and easy to understand.

**Q: What happens if I call undoLastTransaction on an empty list?**
> We check `if (undoStackTop == nullptr)` first and print "Nothing to undo!" then return. This is called a guard clause — it prevents the program from crashing when trying to access a null pointer.

**Q: Why do you `delete topStack` before removing the DLL node?**
> The StackNode and the TransactionNode are separate objects in memory. We get the address of the transaction from the stack node, then we no longer need the stack node itself, so we free it. The TransactionNode is freed later after unlinking it from the DLL.

---

### SECTION C — Time & Space Complexity

**Q: What is the time complexity of logTransaction?**
> O(1) — We always append to the tail using the tail pointer, and push onto the stack top. Both are direct operations with no loops.

**Q: What is the time complexity of undoLastTransaction?**
> O(1) — We pop from the stack top (direct), then unlink the node from the DLL using its own prev/next pointers (direct). No traversal needed.

**Q: What is the time complexity of printTimeline?**
> O(n) — We must visit every node in the list to print it. n is the number of transactions.

**Q: What is the space complexity of the whole TransactionTimeline?**
> O(n) — We store n TransactionNodes (one per transaction) and n StackNodes (one per transaction). So it is 2n = O(n).

**Q: How is DLL deletion O(1) if we do not search for the node?**
> Because the Stack already gave us the exact memory address (pointer) of the node to delete. We do not need to search. The `prev` and `next` pointers inside the node itself tell us its neighbors, so we can fix the links immediately.

---

### SECTION D — Why This Design?

**Q: Why did you combine a DLL and a Stack? Why not just use a Stack alone?**
> The DLL stores ALL transactions in order so we can display the full history. The Stack only remembers the most recent transaction for undo. They serve different purposes. A Stack alone cannot display history in order.

**Q: Why not use an array instead of a DLL?**
> Arrays have a fixed size and deleting from the middle requires shifting all elements — O(n). DLL can insert and delete in O(1) without shifting. Also, we do not know in advance how many transactions there will be, so a dynamic structure like DLL is better.

**Q: Could you use a `std::list` or `std::stack` from the C++ standard library instead?**
> Yes, but this is a DSA project. The point is to implement the data structures manually to understand how they work internally. Using built-in containers would hide the logic.

**Q: Why is the Stack implemented as a linked list and not an array?**
> Because we do not know how many transactions the user will make. A linked-list based stack grows dynamically. An array-based stack needs a fixed capacity.

---

### SECTION E — Memory Management

**Q: What is a memory leak?**
> When you allocate memory with `new` but never free it with `delete`. The memory stays reserved even after the program ends. Over time this can slow down or crash the system.

**Q: Where could a memory leak happen in your code if the destructor was missing?**
> Every `new TransactionNode(...)` and every `new StackNode(...)` would leak. If a user logs 100 transactions, 200 objects would stay in memory after the program exits.

**Q: Why do you save `current->next` into `temp` before deleting in the destructor?**
> Because after `delete current`, the memory that `current` pointed to is freed and invalid. If we tried to access `current->next` after deletion, it would be undefined behaviour (crash or garbage value). We save `next` first while the memory is still valid.

---

### SECTION F — "Add This Feature" Questions

**Q: How would you add a "Redo" feature?**
> Add a second Stack called `redoStack`. When the user undoes, instead of deleting the node, pop it from the undoStack, remove it from the DLL, and push it onto the redoStack. When redo is called, pop from redoStack, re-insert into the DLL, and push back onto undoStack.

**Q: How would you search for a transaction by ID?**
> Traverse the DLL from head to tail using a while loop. Compare each node's `transactionId` with the target. Return the node if found, nullptr if not.
```cpp
TransactionNode* current = head;
while (current != nullptr) {
    if (current->transactionId == targetId) return current;
    current = current->next;
}
return nullptr;
```
> Time complexity: O(n)

**Q: How would you delete a specific transaction by ID (not just the last one)?**
> Search for the node by ID (O(n)), then apply the same 4-case DLL deletion logic. The Stack would also need to be updated to remove the corresponding StackNode.

**Q: How would you print the timeline in REVERSE order?**
> Start from `tail` and traverse backward using `prev` pointers.
```cpp
TransactionNode* current = tail;
while (current != nullptr) {
    // print current
    current = current->prev;
}
```

**Q: How would you find the total amount spent?**
> Traverse the DLL from head to tail, add up all `amount` values.
```cpp
double total = 0;
TransactionNode* current = head;
while (current != nullptr) {
    total += current->amount;
    current = current->next;
}
```

**Q: How would you find the most expensive transaction?**
> Traverse the DLL, keep track of the maximum amount seen so far.
```cpp
TransactionNode* maxNode = head;
TransactionNode* current = head;
while (current != nullptr) {
    if (current->amount > maxNode->amount) maxNode = current;
    current = current->next;
}
```

**Q: How would you sort transactions by amount?**
> You could implement Bubble Sort on the DLL. Swap data between nodes (not the nodes themselves) while traversing.
> Time complexity: O(n²) — acceptable for small datasets in a hostel app.

---

### SECTION G — Drawing / Diagram Questions

**Q: Draw the DLL after logging 3 transactions.**
```
head                                                       tail
 |                                                          |
[NULL | Txn#1 "Groceries" Rs.200 | -->] <--> [<-- | Txn#2 "Rent" Rs.3000 | -->] <--> [<-- | Txn#3 "Water" Rs.100 | NULL]
```

**Q: Draw the Stack after logging 3 transactions.**
```
undoStackTop
     |
  [-> Txn#3]  <- top (most recent)
     |
  [-> Txn#2]
     |
  [-> Txn#1]
     |
   nullptr
```

**Q: Draw what happens when we call undoLastTransaction once.**
```
BEFORE:
  Stack top -> Txn#3
  DLL: Txn#1 <-> Txn#2 <-> Txn#3 (tail)

AFTER undo:
  Stack top -> Txn#2
  DLL: Txn#1 <-> Txn#2 (tail)
  Txn#3 node: deleted from memory
```

---

### SECTION H — Tricky / Advanced Questions

**Q: Is your undo Stack always in sync with the DLL?**
> Yes, as long as the user only removes transactions using undoLastTransaction. Every `logTransaction` call does both: append to DLL AND push to stack. Every `undoLastTransaction` does both: pop from stack AND remove from DLL.

**Q: What if the stack has a node pointing to a DLL node that was already manually deleted?**
> That would be a dangling pointer bug. In our implementation this cannot happen because the only way to remove a DLL node is through `undoLastTransaction`, which always pops the stack first. We never delete DLL nodes independently.

**Q: What is the difference between `delete topStack` and `delete toRemove` in undoLastTransaction?**
> `topStack` is the StackNode wrapper — it holds the pointer but is not the actual transaction data. We delete it after extracting the transaction address. `toRemove` is the actual TransactionNode with all the transaction data — we delete it after unlinking it from the DLL.

**Q: Can two stack nodes point to the same transaction node?**
> In our implementation, no. Each transaction is pushed exactly once (in logTransaction) and popped exactly once (in undoLastTransaction). There is a 1-to-1 relationship between stack nodes and DLL nodes.

---

## QUICK REVISION SUMMARY

| Topic | Answer |
|---|---|
| Data structures used | Doubly Linked List + Stack |
| logTransaction complexity | O(1) |
| undoLastTransaction complexity | O(1) |
| printTimeline complexity | O(n) |
| Why DLL over array | No shifting, O(1) delete with pointers |
| Why Stack for undo | LIFO — last added is first undone |
| DLL deletion cases | 4 (only, head, tail, middle) |
| Why keep tail pointer | O(1) append instead of O(n) traversal |
| Memory management | new to create, delete in destructor |
| Why save next before delete | After delete, pointer is invalid |

---

## DEMO SCRIPT FOR VIVA

1. Run the program → show the menu
2. **Option 1** → Add 2-3 roommates (A01-Ali, A02-Sara, A03-Umar)
3. **Option 2** → Log 3 expenses (Groceries Rs.500 by A01, Electricity Rs.1200 by A02, Water Rs.300 by A03)
4. **Option 4** → Show the timeline — explain: "This traverses my Doubly Linked List from head to tail"
5. **Option 3** → Undo last transaction — explain: "This pops from the Stack and removes that node from the DLL"
6. **Option 4** → Show timeline again — Water bill is gone, proves undo worked
7. Point to the code: show head/tail pointers, show the 4 deletion cases, show the stack push/pop

---

*This guide covers your complete implementation of the Transaction Timeline for the Hostel Kharcha Manager DSA project.*
*DSA concepts: Doubly Linked List, Stack, Dynamic Memory Allocation, Pointer Manipulation*
