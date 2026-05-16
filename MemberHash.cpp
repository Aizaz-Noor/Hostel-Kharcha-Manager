#include "../include/MemberHash.h"

// Role 1: Implement the Hash Map logic here!
// Note: You must not use any STL containers.

MemberHash::MemberHash() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table[i] = nullptr;
    }
    autoID == 1000;
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
    for(int i = 0; i < TABLE_SIZE; i++){
        cout<<i<<" :";
        
        MemberNode *p = table[i];
        while(p != nullptr){
            cout<<"ID: "<<p ->id<<"Username: "<<p->userName<<endl;
            p = p ->next;
        }
    }
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
