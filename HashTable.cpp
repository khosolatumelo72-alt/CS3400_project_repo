#include "HashTable.h"
#include <iostream>
using namespace std;

HashNode::HashNode(const string& k, const Vehicle& v) : key(k), value(v), next(NULL) {}

HashTable::HashTable(int size) : tableSize(size), count(0), collisions(0) {
    if (tableSize < 1) tableSize = 17;          // guard against bad sizes
    buckets = new HashNode*[tableSize];
    for (int i = 0; i < tableSize; i++)
        buckets[i] = NULL;
}

HashTable::~HashTable() {
    for (int i = 0; i < tableSize; i++) {
        HashNode* cur = buckets[i];
        while (cur != NULL) {
            HashNode* nextNode = cur->next;     // save before deleting
            delete cur;
            cur = nextNode;
        }
    }
    delete[] buckets;
}

// Polynomial rolling hash: h = h * 31 + character, then take the remainder.
int HashTable::hashFunction(const string& key) const {
    unsigned long h = 0;
    for (size_t i = 0; i < key.length(); i++)
        h = h * 31 + (unsigned char)key[i];
    return (int)(h % tableSize);
}

bool HashTable::insert(const Vehicle& v) {
    string id = v.getID();
    int index = hashFunction(id);

    for (HashNode* cur = buckets[index]; cur != NULL; cur = cur->next)
        if (cur->key == id) return false;       // duplicate ID

    if (buckets[index] != NULL) collisions++;   // bucket already in use

    HashNode* node = new HashNode(id, v);
    node->next = buckets[index];                // insert at head of chain: O(1)
    buckets[index] = node;
    count++;
    return true;
}

Vehicle* HashTable::search(const string& id) const {
    int index = hashFunction(id);
    for (HashNode* cur = buckets[index]; cur != NULL; cur = cur->next)
        if (cur->key == id) return &cur->value;
    return NULL;
}

bool HashTable::remove(const string& id) {
    int index = hashFunction(id);
    HashNode* prev = NULL;
    HashNode* cur = buckets[index];
    while (cur != NULL) {
        if (cur->key == id) {
            if (prev == NULL) buckets[index] = cur->next;   // removing the head
            else              prev->next = cur->next;       // bypass the node
            delete cur;
            count--;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

void HashTable::display() const {
    for (int i = 0; i < tableSize; i++) {
        cout << "[" << i << "] ";
        for (HashNode* cur = buckets[i]; cur != NULL; cur = cur->next)
            cout << "-> " << cur->key << " ";
        cout << "-> NULL" << endl;
    }
}

double HashTable::loadFactor() const   { return (double)count / tableSize; }
int    HashTable::size() const         { return count; }
int    HashTable::getCollisions() const{ return collisions; }
bool   HashTable::isEmpty() const      { return count == 0; }
