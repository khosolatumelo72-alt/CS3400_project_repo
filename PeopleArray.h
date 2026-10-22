#ifndef PEOPLEARRAY_H
#define PEOPLEARRAY_H
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

template <typename T>
class PeopleArray {
private:
    T* data;
    int size;
    int capacity;

    PeopleArray(const PeopleArray&);              // no copying
    PeopleArray& operator=(const PeopleArray&);

    void grow() {                                 // double the capacity
        capacity = capacity * 2;
        T* bigger = new T[capacity];
        for (int i = 0; i < size; i++) bigger[i] = data[i];
        delete[] data;
        data = bigger;
    }

public:
    PeopleArray() : size(0), capacity(10) { data = new T[capacity]; }
    ~PeopleArray() { delete[] data; }

    int indexOf(const string& id) const {         // linear search, O(n)
        for (int i = 0; i < size; i++)
            if (data[i].getID() == id) return i;
        return -1;
    }

    bool add(const T& item) {                     // O(1) on average
        if (indexOf(item.getID()) != -1) return false;   // duplicate ID
        if (size == capacity) grow();
        data[size++] = item;
        return true;
    }

    bool remove(const string& id) {               // O(n): shifts items left
        int pos = indexOf(id);
        if (pos == -1) return false;
        for (int i = pos; i < size - 1; i++) data[i] = data[i + 1];
        size--;
        return true;
    }

    T* find(const string& id) {                   
        int pos = indexOf(id);
        if (pos == -1) return NULL;
        return &data[pos];
    }

    T& at(int i) { return data[i]; }              
    int getSize() const { return size; }
    bool isEmpty() const { return size == 0; }

    void display() const {
        if (size == 0) { cout << "Nothing to show.\n"; return; }
        for (int i = 0; i < size; i++) data[i].display();
    }

    void saveToFile(const string& filename) const {
        ofstream out(filename.c_str());
        for (int i = 0; i < size; i++) out << data[i].toLine() << "\n";
        out.close();
    }

    void loadFromFile(const string& filename) {
        ifstream in(filename.c_str());
        if (!in) return;
        string line;
        while (getline(in, line)) {
            if (line == "") continue;
            T item;
            item.fromLine(line);
            add(item);
        }
        in.close();
    }
};
#endif
