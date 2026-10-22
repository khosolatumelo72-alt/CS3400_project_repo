#ifndef VEHICLELIST_H
#define VEHICLELIST_H
#include "Vehicle.h"
#include <fstream>
#include <sstream>
#include <cstdLib>
using namespace std;

class Node{
	public:
		Vehicle data;
		Node*next;
		Node(const Vehicle&);
		
};

class Vehiclelist{
	private:
		Node* head;
		Node* tail;
		int size;
		
		
	public:
		Vehiclelist();
		~Vehiclelist();
		Vehiclelist(const Vehiclelist&)=delete;
		Vehiclelist& operator=(const Vehiclelist&)=delete;
		bool exists(const string& id) const;
		bool insertFront(const Vehicle&);
		bool insertEnd(const Vehicle&);
		bool remove(const string&);
		Node* search(const string&);
		void display()const;
		int getsize()const;
		bool isEmpty()const;
		Vehicle* toArray()const;
		void saveToFile(const string&)const;
		void loadFromFile(const string&);
			
};
#endif
