#ifndef PEOPLE_H
#define PEOPLE_H
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

class Person {
protected:
    string id, name;
public:
    Person() {}
    Person(string i, string n) : id(i), name(n) {}
    string getID() const { return id; }
    string getName() const { return name; }
};

class Driver : public Person {
private:
    string vehicleID;                  // links to Vehicle by ID
public:
    Driver() {}
    Driver(string i, string n, string v) : Person(i, n), vehicleID(v) {}
    string getVehicleID() const { return vehicleID; }
    void setVehicleID(string v) { vehicleID = v; }

    void display() const {
        cout << id << " | " << name << " | " << vehicleID << endl;
    }
    string toLine() const { return id + "|" + name + "|" + vehicleID; }
    void fromLine(const string& line) {
        stringstream ss(line);
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, vehicleID);
    }
};

class Officer : public Person {
private:
    string shift;
public:
    Officer() {}
    Officer(string i, string n, string s) : Person(i, n), shift(s) {}
    string getShift() const { return shift; }
    void setShift(string s) { shift = s; }

    void display() const {
        cout << id << " | " << name << " | " << shift << endl;
    }
    string toLine() const { return id + "|" + name + "|" + shift; }
    void fromLine(const string& line) {
        stringstream ss(line);
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, shift);
    }
};
#endif
