#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "Vehicle.h"
#include <string>
using namespace std;

// One entry in a bucket's chain (a singly linked list node).
class HashNode {
public:
    string key;        // Vehicle ID
    Vehicle value;
    HashNode* next;
    HashNode(const string& k, const Vehicle& v);
};

// Hash table: Vehicle ID -> Vehicle. Collisions use separate chaining.
class HashTable {
private:
    HashNode** buckets;   // dynamic array; each slot is the head of a chain
    int tableSize;
    int count;            // vehicles stored
    int collisions;       // inserts that landed in an already-used bucket

    int hashFunction(const string& key) const;

public:
    explicit HashTable(int size = 17);
    ~HashTable();
    HashTable(const HashTable&) = delete;                 // copying would double-delete chains
    HashTable& operator=(const HashTable&) = delete;

    bool     insert(const Vehicle& v);          // false if ID already exists
    Vehicle* search(const string& id) const;    // NULL if not found
    bool     remove(const string& id);          // false if ID not found

    void   display() const;                     // show every bucket and chain
    double loadFactor() const;                  // count / tableSize
    int    size() const;
    int    getCollisions() const;
    bool   isEmpty() const;
};
#endif
