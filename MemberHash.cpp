#include "../include/MemberHash.h"

// Role 1: Implement the Hash Map logic here!
// Note: You must not use any STL containers.

MemberHash::MemberHash() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table[i] = nullptr;
    }
    autoID = 1000;

    loadFromFile();
}


int MemberHash::hashFunction(const string& username) {
    int key = 0;
    for (char c : username) {
        key += c;
    }
    return key % TABLE_SIZE;
}

bool MemberHash::addMember(const string& username, const string& password) {
    if(getMember(username) != nullptr){
        return false;
    }
    int id = autoID++;
    MemberNode *newNode = new MemberNode(id, username, password);
    
    int index = hashFunction(username);

    if(table[index] == nullptr){
        table[index] = newNode;
    }
    else{
        MemberNode *p = table[index];
        while(p ->next != nullptr){
            p = p ->next;
        }

        p ->next = newNode;
    }

    saveToFile();
    return true;
    
}

MemberNode* MemberHash::getMember(const string& username) {
    int index = hashFunction(username);
    MemberNode* p = table[index];

    while(p != nullptr) {
        if(p ->userName == username) {
            return p;
        }

        p = p ->next;
    }

    return nullptr;
}

bool MemberHash::login(const string &username, const string &password){
    MemberNode *user = getMember(username);

    if(user != nullptr && user ->password == password){
        return true;
    }

    return false;
}

void MemberHash::printAllMembers() {

    for(int i = 0; i < TABLE_SIZE; i++) {

        MemberNode* p = table[i];

        if(p == nullptr) {
            continue;
        }

        cout << "\nBucket "
             << i
             << ":\n";

        while(p != nullptr) {

            cout << "--------------------------------\n";

            cout << "ID: "
                 << p->id
                 << endl;

            cout << "Username: "
                 << p->userName
                 << endl;

            cout << "Balance: "
                 << p->balance
                 << endl;

            cout << "--------------------------------\n";

            p = p->next;
        }
    }
}

void MemberHash::saveToFile() {
    ofstream file("members.txt");

    if(!file) {
        cout << "File could not open." << endl;
        return;
    }

    for(int i = 0; i < TABLE_SIZE; i++) {

        MemberNode* p = table[i];

        while(p != nullptr) {

            file << p->id << " "
                 << p->userName << " "
                 << p->password << " "
                 << p->balance
                 << endl;

            p = p->next;
        }
    }

    file.close();
}

void MemberHash::loadFromFile() {
    ifstream file("members.txt");

    if(!file) {
        return;
    }

    int id;
    string username;
    string password;
    double balance;

    while(file >>id >>username >>password >>balance)
    {
        MemberNode* newNode =
            new MemberNode(id, username, password);

            newNode->balance = balance;

        int index = hashFunction(username);

        if(table[index] == nullptr) {
            table[index] = newNode;
        }
        else {
            MemberNode* p = table[index];

            while(p->next != nullptr) {
                p = p->next;
            }

            p->next = newNode;
        }

        if(id >= autoID) {
            autoID = id + 1;
        }
    }

    file.close();
}

MemberHash::~MemberHash()
{
    for(int i = 0; i < TABLE_SIZE; i++){
        MemberNode* p = table[i];

        while(p != nullptr){
            MemberNode* temp = p;
            p = p->next;
            delete temp;
        }
    }
}
