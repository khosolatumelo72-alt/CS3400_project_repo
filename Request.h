#ifndef REQUEST_H
#define REQUEST_H
#include <iostream>
#include <string>
using namespace std;

class Request {
private:
	int id;
	string user;
	string vehicleID;
	string pickup;
	string destination;
	string time;
	string status;
	int priority;

public:
	Request();
	Request(int requestId, const string& requestUser, const string& vehicle,
		const string& pickupLocation, const string& destinationLocation,
		const string& requestTime, const string& requestStatus, int requestPriority);

	int getID() const;
	string getUser() const;
	string getVehicleID() const;
	string getPickup() const;
	string getDestination() const;
	string getTime() const;
	string getStatus() const;
	int getPriority() const;
	void setStatus(const string&);
	void display() const;
};
#endif
