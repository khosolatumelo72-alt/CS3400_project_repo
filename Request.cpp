#include "Request.h"

Request::Request() : id(0), priority(3) {
}

Request::Request(int requestId, const string& requestUser, const string& vehicle,
		const string& pickupLocation, const string& destinationLocation,
		const string& requestTime, const string& requestStatus, int requestPriority)
	: id(requestId),
	  user(requestUser),
	  vehicleID(vehicle),
	  pickup(pickupLocation),
	  destination(destinationLocation),
	  time(requestTime),
	  status(requestStatus),
	  priority(requestPriority) {
}

int Request::getID() const { return id; }
string Request::getUser() const { return user; }
string Request::getVehicleID() const { return vehicleID; }
string Request::getPickup() const { return pickup; }
string Request::getDestination() const { return destination; }
string Request::getTime() const { return time; }
string Request::getStatus() const { return status; }
int Request::getPriority() const { return priority; }

void Request::setStatus(const string& newStatus) {
	status = newStatus;
}

void Request::display() const {
	static const char* priorityLabels[] = {
		"", "Emergency", "High", "Normal", "Low"
	};

	const char* priorityLabel = "Invalid";
	if (priority >= 1 && priority <= 4) {
		priorityLabel = priorityLabels[priority];
	}

	cout << "Request " << id
		 << " | " << user
		 << " | Vehicle: " << vehicleID
		 << " | " << pickup << " -> " << destination
		 << " | " << time
		 << " | " << status
		 << " | Priority: " << priorityLabel << endl;
}
