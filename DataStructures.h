#ifndef DS_H
#define DS_H

#include <iostream>
#include <string>

using namespace std;

// --- Common Structures ---

struct Transaction {
    int id;
    string payer;
    string description;
    double amount;
    Transaction* next; // For Stack and DLL
    Transaction* prev; // For DLL
    
    Transaction(int _id, string p, string desc, double amt) {
        id = _id;
        payer = p;
        description = desc;
        amount = amt;
        next = NULL;
        prev = NULL;
    }
};

struct Member {
    string name;
    double balance;
    Member* next; // For Hash Map Separate Chaining
    
    Member(string n) {
        name = n;
        balance = 0.0;
        next = NULL;
    }
};

// ==========================================
// ROLE 1: Hash Map (Member Profiles)
// ==========================================
class MemberMap {
private:
    static const int TABLE_SIZE = 10;
    Member* table[TABLE_SIZE];

    int hashFunction(string name) {
        int hash = 0;
        for (int i = 0; i < name.length(); i++) {
            hash = (hash + name[i]) % TABLE_SIZE;
        }
        return hash;
    }

public:
    MemberMap() {
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = NULL;
    }

    void addMember(string name) {
        int index = hashFunction(name);
        Member* newMember = new Member(name);
        if (!table[index]) {
            table[index] = newMember;
        } else {
            Member* temp = table[index];
            while (temp->next) temp = temp->next;
            temp->next = newMember;
        }
    }

    Member* getMember(string name) {
        int index = hashFunction(name);
        Member* temp = table[index];
        while (temp) {
            if (temp->name == name) return temp;
            temp = temp->next;
        }
        return NULL;
    }
    
    void updateBalance(string name, double amount) {
        Member* m = getMember(name);
        if (m) m->balance += amount;
    }

    ~MemberMap() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Member* temp = table[i];
            while (temp) {
                Member* toDelete = temp;
                temp = temp->next;
                delete toDelete;
            }
        }
    }
};

// ==========================================
// ROLE 2: Doubly Linked List (Timeline) & Stack (Undo)
// ==========================================
class HistoryDLL {
public:
    Transaction* head;
    Transaction* tail;

    HistoryDLL() {
        head = NULL;
        tail = NULL;
    }

    void append(Transaction* t) {
        Transaction* newNode = new Transaction(t->id, t->payer, t->description, t->amount);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void removeTail() {
        if (!tail) return;
        Transaction* toDelete = tail;
        if (head == tail) {
            head = tail = NULL;
        } else {
            tail = tail->prev;
            tail->next = NULL;
        }
        delete toDelete;
    }

    ~HistoryDLL() {
        Transaction* temp = head;
        while (temp) {
            Transaction* toDelete = temp;
            temp = temp->next;
            delete toDelete;
        }
    }
};

class UndoStack {
private:
    Transaction* topNode;
public:
    UndoStack() {
        topNode = NULL;
    }

    void push(Transaction* t) {
        Transaction* newNode = new Transaction(t->id, t->payer, t->description, t->amount);
        newNode->next = topNode;
        topNode = newNode;
    }

    Transaction* pop() {
        if (!topNode) return NULL;
        Transaction* temp = topNode;
        topNode = topNode->next;
        return temp; // Caller assumes responsibility for deletion
    }

    ~UndoStack() {
        Transaction* temp = topNode;
        while (temp) {
            Transaction* toDelete = temp;
            temp = temp->next;
            delete toDelete;
        }
    }
};

// ==========================================
// ROLE 3: Max-Heap (Debt Prioritization)
// ==========================================
struct DebtNode {
    string name;
    double debt;
};

class DebtMaxHeap {
private:
    DebtNode* heap;
    int capacity;
    int size;

    void heapifyUp(int index) {
        if (index && heap[parent(index)].debt < heap[index].debt) {
            swap(heap[index], heap[parent(index)]);
            heapifyUp(parent(index));
        }
    }

    void heapifyDown(int index) {
        int left = leftChild(index);
        int right = rightChild(index);
        int largest = index;

        if (left < size && heap[left].debt > heap[largest].debt) largest = left;
        if (right < size && heap[right].debt > heap[largest].debt) largest = right;

        if (largest != index) {
            swap(heap[index], heap[largest]);
            heapifyDown(largest);
        }
    }

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return (2 * i + 1); }
    int rightChild(int i) { return (2 * i + 2); }
    
    void swap(DebtNode& a, DebtNode& b) {
        DebtNode temp = a;
        a = b;
        b = temp;
    }

public:
    DebtMaxHeap(int cap = 100) {
        capacity = cap;
        size = 0;
        heap = new DebtNode[capacity];
    }

    void insertOrUpdate(string n, double d) {
        // Simple linear search to update if exists. In O(N), but acceptable for small constraints
        for (int i = 0; i < size; i++) {
            if (heap[i].name == n) {
                heap[i].debt = d;
                heapifyUp(i);
                heapifyDown(i);
                return;
            }
        }
        if (size == capacity) return; // Full
        heap[size].name = n;
        heap[size].debt = d;
        heapifyUp(size);
        size++;
    }

    void printTopDebtor() {
        if (size > 0) {
            cout << heap[0].name << " owes the most: " << heap[0].debt << "\n";
        }
    }

    ~DebtMaxHeap() {
        delete[] heap;
    }
};

#endif
