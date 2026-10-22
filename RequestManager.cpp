#include "RequestManager.h"

#include <cstdlib>
#include <fstream>
#include <sstream>

using namespace std;

static string readText(const string& prompt) {
	cout << prompt;
	string value;
	getline(cin, value);
	return value;
}

static int readInteger(const string& prompt) {
	while (true) {
		string input = readText(prompt);
		stringstream parser(input);
		int value;
		char extra;

		if (parser >> value && !(parser >> extra)) {
			return value;
		}
		cout << "Enter a whole number.\n";
	}
}

RequestManager::RequestManager() : nextID(1) {
}

bool RequestManager::exists(int requestId) const {
	return normal.containsID(requestId) || urgent.containsID(requestId);
}

bool RequestManager::undoLastCancellation() {
	Request request;
	if (!cancelled.pop(request)) {
		return false;
	}

	if (request.getPriority() <= 2) {
		urgent.insert(request);
	} else {
		normal.enqueue(request);
	}
	return true;
}

void RequestManager::load(const string& filename) {
	ifstream input(filename.c_str());
	string line;

	while (getline(input, line)) {
		stringstream row(line);
		string idText, user, vehicleId, pickup, destination;
		string time, status, priorityText;

		if (!getline(row, idText, '|') ||
			!getline(row, user, '|') ||
			!getline(row, vehicleId, '|') ||
			!getline(row, pickup, '|') ||
			!getline(row, destination, '|') ||
			!getline(row, time, '|') ||
			!getline(row, status, '|') ||
			!getline(row, priorityText, '|')) {
			continue;
		}

		int id = atoi(idText.c_str());
		int priority = atoi(priorityText.c_str());
		if (id <= 0 || priority < 1 || priority > 4) {
			continue;
		}

		Request request(id, user, vehicleId, pickup, destination, time, status, priority);
		if (priority <= 2) {
			urgent.insert(request);
		} else {
			normal.enqueue(request);
		}
		if (id >= nextID) {
			nextID = id + 1;
		}
	}
}

void RequestManager::save(const string& filename) const {
	ofstream output(filename.c_str());
	if (!output) {
		cerr << "Could not save " << filename << ".\n";
		return;
	}

	normal.save(output);
	urgent.save(output);
}

void RequestManager::menu(const Vehiclelist& vehicles) {
	int choice = -1;
	while (choice != 0) {
		cout << "\n=== Student B: Requests and Activities ===\n"
			 << "1. Submit normal request\n"
			 << "2. Submit priority request\n"
			 << "3. View normal FIFO queue\n"
			 << "4. Process next normal request\n"
			 << "5. View priority queue\n"
			 << "6. Process highest-priority request\n"
			 << "7. Cancel pending request\n"
			 << "8. Undo cancellation\n"
			 << "9. Schedule activity\n"
			 << "0. Return\n";
		choice = readInteger("Choice: ");

		if (choice == 1 || choice == 2) {
			submitRequest(vehicles, choice == 2);
		} else if (choice == 3) {
			normal.display();
		} else if (choice == 4) {
			processNextNormal();
		} else if (choice == 5) {
			urgent.display();
		} else if (choice == 6) {
			processNextPriority();
		} else if (choice == 7) {
			cancelRequest();
		} else if (choice == 8) {
			cout << (undoLastCancellation()
				? "Cancellation undone.\n"
				: "Nothing to undo.\n");
		} else if (choice == 9) {
			scheduleActivity();
		} else if (choice != 0) {
			cout << "Invalid choice.\n";
		}
	}
}

void RequestManager::submitRequest(const Vehiclelist& vehicles, bool priorityRequest) {
	string user = readText("User: ");
	string vehicleId = readText("Registered vehicle ID: ");
	if (!vehicles.exists(vehicleId)) {
		cout << "Vehicle ID not found. Register it in Resource Management first.\n";
		return;
	}

	string pickup = readText("Pickup: ");
	string destination = readText("Destination: ");
	string time = readText("Request time: ");
	int priority = 3;
	if (priorityRequest) {
		priority = readInteger("Priority (1 Emergency, 2 High, 3 Normal, 4 Low): ");
	}
	if (user.empty() || pickup.empty() || destination.empty() || time.empty() ||
		priority < 1 || priority > 4) {
		cout << "Invalid request details.\n";
		return;
	}

	Request request(nextID, user, vehicleId, pickup, destination, time, "Pending", priority);
	if (exists(nextID)) {
		cout << "Request ID conflict.\n";
		return;
	}
	if (priority <= 2) {
		urgent.insert(request);
	} else {
		normal.enqueue(request);
	}
	cout << "Submitted request " << nextID << ".\n";
	++nextID;
}

void RequestManager::processNextNormal() {
	Request request;
	if (!normal.dequeue(request)) {
		cout << "No normal requests pending.\n";
		return;
	}
	request.setStatus("Processed");
	cout << "Processed FIFO request: ";
	request.display();
}

void RequestManager::processNextPriority() {
	Request request;
	if (!urgent.removeTop(request)) {
		cout << "No priority requests pending.\n";
		return;
	}
	request.setStatus("Processed");
	cout << "Processed most urgent request: ";
	request.display();
}

void RequestManager::cancelRequest() {
	int requestId = readInteger("Request ID to cancel: ");
	Request request;
	if (normal.removeID(requestId, request) || urgent.removeID(requestId, request)) {
		cancelled.push(request);
		cout << "Cancelled; undo is available.\n";
	} else {
		cout << "Pending request not found.\n";
	}
}

void RequestManager::scheduleActivity() {
	string title = readText("Activity title: ");
	string dateTime = readText("Date and time: ");
	string venue = readText("Venue/location: ");
	int priority = readInteger("Priority (1 Emergency, 2 High, 3 Normal, 4 Low): ");
	if (title.empty() || dateTime.empty() || venue.empty() || priority < 1 || priority > 4) {
		cout << "Invalid activity details.\n";
		return;
	}

	Request activity(nextID, "Activity", "ACTIVITY", title, venue, dateTime, "Scheduled", priority);
	if (priority <= 2) {
		urgent.insert(activity);
	} else {
		normal.enqueue(activity);
	}
	cout << "Activity scheduled as request " << nextID << ".\n";
	++nextID;
}

void RequestManager::priorityMenu(const Vehiclelist& vehicles) {
	int choice = -1;
	while (choice != 0) {
		cout << "\n=== Priority Request Management ===\n"
			 << "1. Submit priority request\n"
			 << "2. View priority queue\n"
			 << "3. Process highest priority\n"
			 << "0. Return\n";
		choice = readInteger("Choice: ");

		if (choice == 1) {
			submitRequest(vehicles, true);
		} else if (choice == 2) {
			urgent.display();
		} else if (choice == 3) {
			processNextPriority();
		} else if (choice != 0) {
			cout << "Invalid choice.\n";
		}
	}
}
