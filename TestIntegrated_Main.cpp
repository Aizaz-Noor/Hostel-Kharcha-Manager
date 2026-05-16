

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// =============================================================================
// DebtRecord - For storing member debt information
// =============================================================================
struct DebtRecord {
    int memberId;
    string memberName;
    double amountOwed;
    
    DebtRecord() : memberId(0), memberName(""), amountOwed(0.0) {}
    DebtRecord(int id, string name, double amt) 
        : memberId(id), memberName(name), amountOwed(amt) {}
};

// =============================================================================
// MemberNode - For storing member information
// =============================================================================
struct MemberNode {
    int id;
    string userName;
    double balance;
    
    MemberNode(int _id, string name, double bal = 0.0) 
        : id(_id), userName(name), balance(bal) {}
};

// =============================================================================
// Transaction - For storing transaction details
// =============================================================================
struct Transaction {
    string description;
    double amount;
    string payerId;
    string timestamp;
    
    Transaction(string desc, double amt, string payer, string time)
        : description(desc), amount(amt), payerId(payer), timestamp(time) {}
};

// =============================================================================
// TransactionTimeline - Logs and displays transactions
// =============================================================================
class TransactionTimeline {
private:
    vector<Transaction> transactions;

public:
    void logTransaction(string desc, double amount, string payerId) {
        string timestamp = "2026-05-16 10:00";  // Mock timestamp
        transactions.push_back(Transaction(desc, amount, payerId, timestamp));
        cout << "  ✔ Transaction logged: " << desc << " (Rs. " << amount << ")\n";
    }
    
    void printTimeline() {
        cout << "\n" << string(70, '=') << "\n";
        cout << "                    TRANSACTION TIMELINE\n";
        cout << string(70, '=') << "\n\n";
        
        if (transactions.empty()) {
            cout << "No transactions recorded yet.\n";
            return;
        }
        
        cout << setw(5) << "No." << " | " 
             << setw(25) << "Description" << " | "
             << setw(10) << "Amount" << " | "
             << setw(10) << "Payer ID" << "\n";
        cout << string(70, '-') << "\n";
        
        for (size_t i = 0; i < transactions.size(); i++) {
            cout << setw(5) << (i + 1) << " | "
                 << setw(25) << transactions[i].description.substr(0, 25) << " | "
                 << setw(10) << "Rs. " << transactions[i].amount << " | "
                 << setw(10) << transactions[i].payerId << "\n";
        }
        
        cout << "\n" << string(70, '=') << "\n";
        cout << "Total Transactions: " << transactions.size() << "\n";
        cout << string(70, '=') << "\n\n";
    }
    
    int getTransactionCount() {
        return transactions.size();
    }
};

// =============================================================================
// DebtHeap - Max-Heap Priority Queue for debtors
// =============================================================================
class DebtHeap {
private:
    DebtRecord* heapArray;
    int capacity;
    int size;
    
    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return (2 * i) + 1; }
    int rightChild(int i) { return (2 * i) + 2; }
    
    void heapifyUp(int index) {
        while (index > 0) {
            int parentIdx = parent(index);
            if (heapArray[index].amountOwed > heapArray[parentIdx].amountOwed) {
                DebtRecord temp = heapArray[index];
                heapArray[index] = heapArray[parentIdx];
                heapArray[parentIdx] = temp;
                index = parentIdx;
            } else {
                break;
            }
        }
    }
    
    void heapifyDown(int index) {
        while (true) {
            int leftIdx = leftChild(index);
            int rightIdx = rightChild(index);
            int largestIdx = index;
            
            if (leftIdx < size && 
                heapArray[leftIdx].amountOwed > heapArray[largestIdx].amountOwed) {
                largestIdx = leftIdx;
            }
            
            if (rightIdx < size && 
                heapArray[rightIdx].amountOwed > heapArray[largestIdx].amountOwed) {
                largestIdx = rightIdx;
            }
            
            if (largestIdx != index) {
                DebtRecord temp = heapArray[index];
                heapArray[index] = heapArray[largestIdx];
                heapArray[largestIdx] = temp;
                index = largestIdx;
            } else {
                break;
            }
        }
    }

public:
    DebtHeap(int cap) {
        capacity = cap;
        size = 0;
        heapArray = new DebtRecord[capacity];
    }

    ~DebtHeap() {
        delete[] heapArray;
    }

    void insertDebt(int id, string name, double amount) {
        if (size >= capacity) {
            cout << "  ERROR: Heap is full!\n";
            return;
        }
        
        heapArray[size] = DebtRecord(id, name, amount);
        heapifyUp(size);
        size++;
    }

    DebtRecord extractMaxDebtor() {
        if (size <= 0) {
            return DebtRecord();
        }
        
        DebtRecord maxDebtor = heapArray[0];
        heapArray[0] = heapArray[size - 1];
        size--;
        
        if (size > 0) {
            heapifyDown(0);
        }
        
        return maxDebtor;
    }
    
    void buildHeapFromMembers(vector<MemberNode*> members) {
        size = 0;
        
        for (size_t i = 0; i < members.size(); i++) {
            if (members[i]->balance < 0) {
                double amountOwed = -(members[i]->balance);
                insertDebt(members[i]->id, members[i]->userName, amountOwed);
            }
        }
    }
    
    void printSummaryReport() {
        cout << "\n" << string(70, '=') << "\n";
        cout << "                    DEBT SUMMARY REPORT\n";
        cout << string(70, '=') << "\n\n";
        
        if (size == 0) {
            cout << "Great news! No one owes any money.\n";
            cout << string(70, '=') << "\n\n";
            return;
        }
        
        cout << "Members sorted by amount owed (highest first):\n\n";
        cout << setw(5) << "Rank" << " | "
             << setw(20) << "Member Name" << " | "
             << setw(15) << "Amount Owed" << "\n";
        cout << string(70, '-') << "\n";
        
        int rank = 1;
        double totalDebt = 0;
        vector<DebtRecord> debtors;
        
        // Extract all debtors and store them
        while (size > 0) {
            DebtRecord debtor = extractMaxDebtor();
            debtors.push_back(debtor);
            totalDebt += debtor.amountOwed;
        }
        
        // Print all debtors
        for (size_t i = 0; i < debtors.size(); i++) {
            cout << setw(5) << (i + 1) << " | "
                 << setw(20) << debtors[i].memberName << " | "
                 << setw(15) << "Rs. " << debtors[i].amountOwed << "\n";
        }
        
        cout << string(70, '-') << "\n";
        cout << "Total Amount Owed: Rs. " << totalDebt << "\n";
        cout << string(70, '=') << "\n\n";
    }
    
    int getSize() { return size; }
};

// =============================================================================
// TEST SUITE - Integrated Testing
// =============================================================================

void printHeader() {
    cout << "\n";
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║  INTEGRATED TEST: TRANSACTION TIMELINE + DEBT HEAP SORTING ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";
}

void printSectionHeader(string title) {
    cout << "\n";
    cout << "┌────────────────────────────────────────────────────────────┐\n";
    cout << "│  " << title << "\n";
    cout << "└────────────────────────────────────────────────────────────┘\n";
}

void test_integrated() {
    printHeader();
    
    // Initialize systems
    TransactionTimeline timeline;
    DebtHeap heap(20);
    vector<MemberNode*> members;
    
    // =================================================================
    // STEP 1: Create Members
    // =================================================================
    printSectionHeader("STEP 1: Creating Members");
    
    MemberNode* member1 = new MemberNode(1, "ali", 0.0);
    MemberNode* member2 = new MemberNode(2, "amjad", 0.0);
    MemberNode* member3 = new MemberNode(3, "Adil", 0.0);
    MemberNode* member4 = new MemberNode(4, "fatima", 0.0);
    MemberNode* member5 = new MemberNode(5, "Razaq", 0.0);
    
    members.push_back(member1);
    members.push_back(member2);
    members.push_back(member3);
    members.push_back(member4);
    members.push_back(member5);
    
    cout << "  ✔ Created 5 members\n";
    for (size_t i = 0; i < members.size(); i++) {
        cout << "    - " << members[i]->userName << " (ID: " << members[i]->id << ")\n";
    }
    
    // =================================================================
    // STEP 2: Log Transactions
    // =================================================================
    printSectionHeader("STEP 2: Logging Transactions");
    
    timeline.logTransaction("Pizza for dinner", 1500, "Rahul");
    timeline.logTransaction("Coffee and snacks", 800, "Priya");
    timeline.logTransaction("Movie tickets", 600, "Aditya");
    timeline.logTransaction("Groceries for week", 2400, "Neha");
    timeline.logTransaction("Internet bill", 500, "Rohan");
    
    // =================================================================
    // STEP 3: Calculate Balances (Simulating Member 1's updateBalance)
    // =================================================================
    printSectionHeader("STEP 3: Calculating Balances");
    
    cout << "  Simulating balance updates:\n";
    
    // Transaction 1: Rahul paid 1500, shared by 5 people
    double perPerson1 = 1500.0 / 5;
    member1->balance += (1500 - perPerson1);  // Rahul is owed
    member2->balance -= perPerson1;           // Priya owes
    member3->balance -= perPerson1;           // Aditya owes
    member4->balance -= perPerson1;           // Neha owes
    member5->balance -= perPerson1;           // Rohan owes
    cout << "    - Pizza (Rs. 1500) split 5 ways\n";
    
    // Transaction 2: Priya paid 800, shared by 5 people
    double perPerson2 = 800.0 / 5;
    member2->balance += (800 - perPerson2);
    member1->balance -= perPerson2;
    member3->balance -= perPerson2;
    member4->balance -= perPerson2;
    member5->balance -= perPerson2;
    cout << "    - Coffee (Rs. 800) split 5 ways\n";
    
    // Transaction 3: Aditya paid 600, shared by 5 people
    double perPerson3 = 600.0 / 5;
    member3->balance += (600 - perPerson3);
    member1->balance -= perPerson3;
    member2->balance -= perPerson3;
    member4->balance -= perPerson3;
    member5->balance -= perPerson3;
    cout << "    - Movie (Rs. 600) split 5 ways\n";
    
    // Transaction 4: Neha paid 2400, shared by 5 people
    double perPerson4 = 2400.0 / 5;
    member4->balance += (2400 - perPerson4);
    member1->balance -= perPerson4;
    member2->balance -= perPerson4;
    member3->balance -= perPerson4;
    member5->balance -= perPerson4;
    cout << "    - Groceries (Rs. 2400) split 5 ways\n";
    
    // Transaction 5: Rohan paid 500, shared by 5 people
    double perPerson5 = 500.0 / 5;
    member5->balance += (500 - perPerson5);
    member1->balance -= perPerson5;
    member2->balance -= perPerson5;
    member3->balance -= perPerson5;
    member4->balance -= perPerson5;
    cout << "    - Internet (Rs. 500) split 5 ways\n";
    
    // =================================================================
    // STEP 4: Display Transaction Timeline
    // =================================================================
    printSectionHeader("STEP 4: Transaction Timeline");
    
    timeline.printTimeline();
    
    // =================================================================
    // STEP 5: Display Current Balances
    // =================================================================
    printSectionHeader("STEP 5: Current Member Balances");
    
    cout << "\n" << setw(20) << "Member Name" << " | "
         << setw(15) << "Balance" << " | "
         << setw(20) << "Status\n";
    cout << string(60, '-') << "\n";
    
    for (size_t i = 0; i < members.size(); i++) {
        string status;
        if (members[i]->balance > 0) {
            status = "OWED MONEY";
        } else if (members[i]->balance < 0) {
            status = "OWES MONEY";
        } else {
            status = "SETTLED";
        }
        
        cout << setw(20) << members[i]->userName << " | "
             << setw(15) << "Rs. " << members[i]->balance << " | "
             << setw(20) << status << "\n";
    }
    cout << "\n";
    
    // =================================================================
    // STEP 6: Build and Display Debt Heap
    // =================================================================
    printSectionHeader("STEP 6: Building Debt Heap (Max-Heap Sorting)");
    
    heap.buildHeapFromMembers(members);
    cout << "  ✔ Heap built with " << heap.getSize() << " debtors\n";
    
    heap.printSummaryReport();
    
    // =================================================================
    // CLEANUP
    // =================================================================
    for (size_t i = 0; i < members.size(); i++) {
        delete members[i];
    }
    
    cout << "\n";
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║          ✓ INTEGRATED TEST COMPLETED SUCCESSFULLY ✓        ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";
}

// =============================================================================
// MAIN
// =============================================================================

int main() {
    try {
        test_integrated();
        
        cout << "\nPress Enter to exit...";
        cin.get();
        
    } catch (const exception& e) {
        cout << "\n✗ TEST FAILED: " << e.what() << "\n";
        cin.get();
        return 1;
    }
    
    return 0;
}
