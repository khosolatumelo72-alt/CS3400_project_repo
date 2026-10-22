#include <iostream>
#include <sstream>

#include "Vehiclelist.h"
#include "Sorting.h"
#include "Tree.h"
#include "HashTable.h"
#include "Graph.h"
#include "MenuC.h"
#include "Student B/RequestManager.h"

using namespace std;

static string readLine(const string& prompt) {
	cout << prompt;
	string value;
	getline(cin, value);
	return value;
}

static int readNumber(const string& prompt) {
	while (true) {
		string input = readLine(prompt);
		stringstream parser(input);
		int value;
		char extra;

		if (parser >> value && !(parser >> extra)) {
			return value;
		}

		cout << "Enter a whole number.\n";
	}
}

static Vehicle promptVehicle() {
	string id = readLine("Vehicle ID: ");
	string plate = readLine("Plate: ");
	string category = readLine("Category: ");
	string type = readLine("Type: ");
	string route = readLine("Route: ");
	int capacity = readNumber("Capacity: ");
	string status = readLine("Status: ");

	return Vehicle(id, plate, category, type, route, capacity, status);
}

static void addVehicle(Vehiclelist& vehicles, VehicleTree& tree, HashTable& table) {
	Vehicle vehicle = promptVehicle();
	if (vehicle.getID().empty() || vehicles.exists(vehicle.getID())) {
		cout << "ID is empty or already registered.\n";
		return;
	}

	if (vehicles.insertEnd(vehicle)) {
		tree.insert(vehicle);
		table.insert(vehicle);
		cout << "Vehicle added.\n";
	}
}

static void removeVehicle(Vehiclelist& vehicles, VehicleTree& tree, HashTable& table) {
	string id = readLine("ID to remove: ");
	if (!vehicles.remove(id)) {
		cout << "Vehicle not found.\n";
		return;
	}

	tree.remove(id);
	table.remove(id);
	cout << "Vehicle removed.\n";
}

static void updateVehicle(Vehiclelist& vehicles, VehicleTree& tree, HashTable& table) {
	string id = readLine("ID to update: ");
	Node* node = vehicles.search(id);
	if (node == NULL) {
		cout << "Vehicle not found.\n";
		return;
	}

	Vehicle replacement = promptVehicle();
	if (replacement.getID() != id) {
		cout << "Changing IDs is not supported; keep the existing ID.\n";
		return;
	}

	tree.remove(id);
	table.remove(id);
	node->data = replacement;
	tree.insert(replacement);
	table.insert(replacement);
	cout << "Vehicle updated.\n";
}

static void resourceMenu(Vehiclelist& vehicles, VehicleTree& tree, HashTable& table) {
	int choice = -1;
	while (choice != 0) {
		cout << "\n=== Resource Management ===\n"
			 << "1. Add vehicle\n"
			 << "2. Remove vehicle\n"
			 << "3. Update vehicle\n"
			 << "4. Display vehicles\n"
			 << "0. Back\n";
		choice = readNumber("Choice: ");

		if (choice == 1) {
			addVehicle(vehicles, tree, table);
		} else if (choice == 2) {
			removeVehicle(vehicles, tree, table);
		} else if (choice == 3) {
			updateVehicle(vehicles, tree, table);
		} else if (choice == 4) {
			vehicles.display();
		} else if (choice != 0) {
			cout << "Invalid choice.\n";
		}
	}
}

static void searchResources(Vehiclelist& vehicles) {
	string id = readLine("Vehicle ID to search: ");
	int method = readNumber("1. Linear search, 2. Binary search by ID: ");

	if (method == 1) {
		Node* current = vehicles.search(id);
		if (current != NULL) {
			current->data.display();
		} else {
			cout << "Vehicle not found.\n";
		}
	} else if (method == 2) {
		int count = vehicles.getsize();
		Vehicle* records = vehicles.toArray();
		mergeSort(records, count, 1);

		int index = binarysearch(records, count, id);
		if (index >= 0) {
			records[index].display();
		} else {
			cout << "Vehicle not found.\n";
		}

		delete[] records;
	} else {
		cout << "Invalid search method.\n";
	}
}

static void sortResources(Vehiclelist& vehicles) {
	int count = vehicles.getsize();
	if (count == 0) {
		cout << "No vehicles to sort.\n";
		return;
	}

	Vehicle* records = vehicles.toArray();
	int key = readNumber("Sort by 1. ID, 2. capacity, 3. type, 4. plate: ");
	if (key < 1 || key > 4) {
		delete[] records;
		cout << "Invalid sort key.\n";
		return;
	}

	int algorithm = readNumber("1. Insertion sort, 2. Merge sort: ");
	if (algorithm == 1) {
		insertionSort(records, count, key);
	} else if (algorithm == 2) {
		mergeSort(records, count, key);
	} else {
		delete[] records;
		cout << "Invalid sorting algorithm.\n";
		return;
	}

	for (int i = 0; i < count; ++i) {
		records[i].display();
	}
	delete[] records;
}

int main() {
	Vehiclelist vehicles;
	vehicles.loadFromFile("vehicles.txt");

	VehicleTree tree;
	HashTable table(101);
	int vehicleCount = vehicles.getsize();
	Vehicle* initialVehicles = vehicles.toArray();
	loadVehiclesIntoStructures(initialVehicles, vehicleCount, tree, table);
	delete[] initialVehicles;

	Graph graph;
	graph.loadFromFile("data/locations.txt");

	RequestManager requests;
	requests.load("requests.txt");

	int choice = -1;
	while (choice != 0) {
		cout << "\n===== CAMPUS TRANSPORT RESOURCE MANAGEMENT =====\n"
			 << "1. Resource management\n"
			 << "2. Search resources by ID\n"
			 << "3. Sort resources\n"
			 << "4. Requests and activities\n"
			 << "5. Priority request management\n"
			 << "6. Undo last request cancellation\n"
			 << "7. Tree operations\n"
			 << "8. Hash table lookup\n"
			 << "9. Campus navigation\n"
			 << "10. System analysis\n"
			 << "0. Save and exit\n";
		choice = readNumber("Choice: ");

		if (choice == 1) {
			resourceMenu(vehicles, tree, table);
		} else if (choice == 2) {
			searchResources(vehicles);
		} else if (choice == 3) {
			sortResources(vehicles);
		} else if (choice == 4) {
			requests.menu(vehicles);
		} else if (choice == 5) {
			requests.priorityMenu(vehicles);
		} else if (choice == 6) {
			cout << (requests.undoLastCancellation()
				? "Cancellation undone.\n"
				: "Nothing to undo.\n");
		} else if (choice == 7) {
			treeMenu(tree);
		} else if (choice == 8) {
			hashMenu(table);
		} else if (choice == 9) {
			navigationMenu(graph);
		} else if (choice == 10) {
			systemAnalysisC();
		} else if (choice != 0) {
			cout << "Invalid choice.\n";
		}
	}

	vehicles.saveToFile("vehicles.txt");
	requests.save("requests.txt");
	graph.saveToFile("data/locations.txt");
	cout << "Data saved. Goodbye.\n";
	return 0;
}
