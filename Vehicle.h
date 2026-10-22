#ifndef VEHICLE_H
#define VEHICLE_H
#include <iostream>
#include <string>
using namespace std;

class Vehicle{
	private:
		string ID, plate,category, type, route, status;
		int Capacity;
	public:
		Vehicle();
		Vehicle(string,string,string,string,int,string); // compatibility overload: category defaults to Transport
		Vehicle(string,string,string,string,string,int,string);
		
		string getID()const;
		string getplate()const;
		string getcategory()const;
		string gettype()const;
		string getroute()const;
		string getstatus()const;
		int getCapacity()const;
		
		void setID(string);
		void setplate(string);
		void setcategory(string);
		void settype(string);
		void setroute(string);
		void setstatus(string);
		void setCapacity(int);
		
		void display()const;
		string toLine()const;
		void fromLine(const string&);
		 
};
#endif
