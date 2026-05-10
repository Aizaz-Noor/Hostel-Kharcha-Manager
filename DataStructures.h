#ifndef DS_H
#define DS_H

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace std;

// ANSI Escape Codes for Styling - Available globally via header
static const string RESET = "\033[0m";
static const string BOLD = "\033[1m";
static const string GREEN = "\033[32m";
static const string RED = "\033[31m";
static const string YELLOW = "\033[33m";
static const string BLUE = "\033[34m";
static const string CYAN = "\033[36m";
static const string MAGENTA = "\033[35m";

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
    int id;
    string name;
    double totalPaid;
    double totalOwed;
    Member* next; // For Hash Map Separate Chaining
    
    Member(int _id, string n) {
        id = _id;
        name = n;
        totalPaid = 0.0;
        totalOwed = 0.0;
        next = NULL;
    }

    double getBalance() {
        return totalPaid - totalOwed;
    }
};

// ==========================================
// ROLE 1: Hash Map (Member Profiles)
// ==========================================
class MemberMap {
private:
    static const int TABLE_SIZE = 10;
    Member* table[TABLE_SIZE];
    int memberCount;

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
        memberCount = 0;
    }

    void addMember(string name) {
        int index = hashFunction(name);
        Member* newMember = new Member(++memberCount, name);
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
    
    void updateBalances(string payerName, double amount) {
        if (memberCount == 0) return;
        
        Member* payer = getMember(payerName);
        if (!payer) return;
        
        payer->totalPaid += amount;
        double share = amount / memberCount;
        
        // Update everyone's Owed amount
        for (int i = 0; i < TABLE_SIZE; i++) {
            Member* temp = table[i];
            while (temp) {
                temp->totalOwed += share;
                temp = temp->next;
            }
        }
    }

    void printAllMembers() {
        bool found = false;
        for (int i = 0; i < TABLE_SIZE; i++) {
            Member* temp = table[i];
            while (temp) {
                cout << " - [ID: " << temp->id << "] " << temp->name 
                     << " (Net: " << (temp->getBalance() >= 0 ? "+" : "") << temp->getBalance() << ")\n";
                temp = temp->next;
                found = true;
            }
        }
        if (!found) cout << "No members in the group yet.\n";
    }

    void printSummaryReport() {
        cout << "-------------------------------------------------------------\n";
        cout << "ID   NAME          PAID        OWED        NET BALANCE\n";
        cout << "-------------------------------------------------------------\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            Member* temp = table[i];
            while (temp) {
                printf("%-4d %-13s %-11.2f %-11.2f %-11.2f\n", 
                       temp->id, temp->name.c_str(), temp->totalPaid, temp->totalOwed, temp->getBalance());
                temp = temp->next;
            }
        }
    }

    bool hasMembers() {
        return memberCount > 0;
    }

    int getMemberCount() {
        return memberCount;
    }

    // Needed for Heap updates
    Member* getMemberAt(int n) {
        int count = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
            Member* temp = table[i];
            while (temp) {
                if (count == n) return temp;
                count++;
                temp = temp->next;
            }
        }
        return NULL;
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
    double debt; // We'll store absolute value of negative balance
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

    void update(MemberMap& m) {
        size = 0; // Rebuild heap for accuracy
        int count = m.getMemberCount();
        for (int i = 0; i < count; i++) {
            Member* mem = m.getMemberAt(i);
            if (mem && mem->getBalance() < 0) {
                heap[size].name = mem->name;
                heap[size].debt = -mem->getBalance();
                heapifyUp(size);
                size++;
            }
        }
    }

    void printTopDebtor() {
        if (size > 0) {
            cout << BOLD << RED << heap[0].name << RESET << " owes the most: " << RED << "$" << heap[0].debt << RESET << "\n";
        } else {
            cout << GREEN << "No one is in debt!" << RESET << "\n";
        }
    }

    ~DebtMaxHeap() {
        delete[] heap;
    }
};

#endif
