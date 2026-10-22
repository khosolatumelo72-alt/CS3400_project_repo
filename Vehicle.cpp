#include "Vehicle.h"
#include <sstream>
#include <cstdlib>

Vehicle::Vehicle():Capacity(0){}

Vehicle::Vehicle(string ID,string plate,string type,string route,int Capacity,string status):ID(ID),plate(plate),category("Transport"),type(type),route(route),status(status),Capacity(Capacity){}

Vehicle::Vehicle(string ID,string plate,string category,string type,string route,int Capacity,string status):ID(ID),plate(plate),category(category),type(type),route(route),status(status),Capacity(Capacity){}

string Vehicle::getID()const{
	return ID;
}
string Vehicle::getplate()const{
	return plate;
}
string Vehicle::getcategory()const{
	return category;
}
string Vehicle::gettype()const{
	return type;
}
string Vehicle::getroute()const{
	return route;
}
string Vehicle::getstatus()const{
	return status;
}
int Vehicle::getCapacity()const{
	return Capacity;
}

void Vehicle::setID(string id){
	ID=id;
}
void Vehicle::setplate(string plt){
	plate=plt;
}
void Vehicle::setcategory(string cate){
	category=cate;
}
void Vehicle::settype(string typ){
	type=typ;
}
void Vehicle::setroute(string rte){
	route=rte;
}
void Vehicle::setstatus(string sts){
	status=sts;
}
void Vehicle::setCapacity(int Cap){
	Capacity=Cap;
}

void Vehicle::display()const{
	cout<<ID<<"|"<<plate<<"|"<<category<<"|"<<type<<"|"<<route<<"|"<<Capacity<<"|"<<status<<endl;
}
string Vehicle::toLine()const{
	ostringstream out;
	out<<ID<<"|"<<plate<<"|"<<category<<"|"<<type<<"|"<<route<<"|"<<Capacity<<"|"<<status;
	return out.str();
}
void Vehicle::fromLine(const string& line){
	stringstream ss(line);
	string cap;
	getline(ss,ID,'|');
	getline(ss,plate,'|');
	getline(ss,category,'|');
	getline(ss,type,'|');
	getline(ss,route,'|');
	getline(ss,cap,'|');
	getline(ss,status);
	Capacity= atoi(cap.c_str());
}
