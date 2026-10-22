#include "MenuC.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <chrono>
using namespace std;

// ---------------------------------------------------------------
// Input helpers: every menu reads a whole line and validates it, so
// typing letters where a number is expected cannot crash the program.
// ---------------------------------------------------------------
static string readText(const string& prompt) {
    string s;
    cout << prompt;
    if (!getline(cin, s)) {                 // input stream closed
        cout << "\nInput closed. Exiting." << endl;
        exit(0);
    }
    return s;
}

static bool isWholeNumber(const string& s) {
    if (s.empty() || s.length() > 6) return false;
    for (size_t i = 0; i < s.length(); i++)
        if (s[i] < '0' || s[i] > '9') return false;
    return true;
}

static bool isDecimal(const string& s) {
    if (s.empty() || s.length() > 8) return false;
    int dots = 0;
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] == '.') dots++;
        else if (s[i] < '0' || s[i] > '9') return false;
    }
    return dots <= 1 && s != ".";
}

static int readInt(const string& prompt) {
    string s = readText(prompt);
    while (!isWholeNumber(s)) {
        cout << "Invalid number. Please try again." << endl;
        s = readText(prompt);
    }
    return atoi(s.c_str());
}

static double readDistance(const string& prompt) {
    string s = readText(prompt);
    while (!isDecimal(s)) {
        cout << "Invalid distance. Please try again." << endl;
        s = readText(prompt);
    }
    return atof(s.c_str());
}

static Vehicle readVehicle() {
    string id     = readText("Vehicle ID: ");
    string plate  = readText("Plate number: ");
    string category = readText("Category: ");
    string type   = readText("Type (Bus/Minibus/Van): ");
    string route  = readText("Route: ");
    int capacity  = readInt("Capacity: ");
    string status = readText("Status (Active/Maintenance): ");
    return Vehicle(id, plate, category, type, route, capacity, status);
}

void loadVehiclesIntoStructures(const Vehicle* vehicles, int n,
                                VehicleTree& tree, HashTable& table) {
    for (int i = 0; i < n; i++) {
        tree.insert(vehicles[i]);
        table.insert(vehicles[i]);
    }
}

// ---------------------------------------------------------------
// Menu 7: Tree operations
// ---------------------------------------------------------------
void treeMenu(VehicleTree& tree) {
    int choice = -1;
    while (choice != 0) {
        cout << "\n===== TREE OPERATIONS (BST ordered by Vehicle ID) =====\n"
             << "1. Insert vehicle\n"
             << "2. Search vehicle by ID\n"
             << "3. Delete vehicle\n"
             << "4. Inorder traversal (sorted by ID)\n"
             << "5. Preorder traversal\n"
             << "6. Postorder traversal\n"
             << "7. Level-order traversal\n"
             << "8. Tree information (size and height)\n"
             << "0. Back\n";
        choice = readInt("Enter your choice: ");

        if (choice == 1) {
            Vehicle v = readVehicle();
            if (v.getID().empty())      cout << "Vehicle ID cannot be empty." << endl;
            else if (tree.insert(v))    cout << "Vehicle inserted." << endl;
            else                        cout << "A vehicle with this ID already exists." << endl;
        } else if (choice == 2) {
            Vehicle* v = tree.search(readText("Vehicle ID to search: "));
            if (v != NULL) v->display();
            else           cout << "Vehicle not found." << endl;
        } else if (choice == 3) {
            if (tree.remove(readText("Vehicle ID to delete: "))) cout << "Vehicle deleted." << endl;
            else                                                  cout << "Vehicle not found." << endl;
        } else if (choice == 4) {
            cout << "--- Inorder ---" << endl;
            if (tree.isEmpty()) cout << "Tree is empty." << endl; else tree.inorder();
        } else if (choice == 5) {
            cout << "--- Preorder ---" << endl;
            if (tree.isEmpty()) cout << "Tree is empty." << endl; else tree.preorder();
        } else if (choice == 6) {
            cout << "--- Postorder ---" << endl;
            if (tree.isEmpty()) cout << "Tree is empty." << endl; else tree.postorder();
        } else if (choice == 7) {
            cout << "--- Level-order ---" << endl;
            tree.levelOrder();
        } else if (choice == 8) {
            cout << "Vehicles in tree: " << tree.size() << "\nTree height: " << tree.height() << endl;
        } else if (choice != 0) {
            cout << "Invalid choice." << endl;
        }
    }
}

// ---------------------------------------------------------------
// Menu 8: Hash table lookup
// ---------------------------------------------------------------
void hashMenu(HashTable& table) {
    int choice = -1;
    while (choice != 0) {
        cout << "\n===== HASH TABLE LOOKUP (Vehicle ID -> Vehicle) =====\n"
             << "1. Insert vehicle\n"
             << "2. Look up vehicle by ID\n"
             << "3. Remove vehicle\n"
             << "4. Display table (buckets and chains)\n"
             << "5. Table statistics\n"
             << "0. Back\n";
        choice = readInt("Enter your choice: ");

        if (choice == 1) {
            Vehicle v = readVehicle();
            if (v.getID().empty())      cout << "Vehicle ID cannot be empty." << endl;
            else if (table.insert(v))   cout << "Vehicle inserted." << endl;
            else                        cout << "A vehicle with this ID already exists." << endl;
        } else if (choice == 2) {
            Vehicle* v = table.search(readText("Vehicle ID to look up: "));
            if (v != NULL) v->display();
            else           cout << "Vehicle not found." << endl;
        } else if (choice == 3) {
            if (table.remove(readText("Vehicle ID to remove: "))) cout << "Vehicle removed." << endl;
            else                                                   cout << "Vehicle not found." << endl;
        } else if (choice == 4) {
            table.display();
        } else if (choice == 5) {
            cout << "Vehicles stored: " << table.size()
                 << "\nLoad factor:     " << table.loadFactor()
                 << "\nCollisions:      " << table.getCollisions() << endl;
        } else if (choice != 0) {
            cout << "Invalid choice." << endl;
        }
    }
}

// ---------------------------------------------------------------
// Menu 9: Campus navigation (graph)
// ---------------------------------------------------------------
void navigationMenu(Graph& graph) {
    const string file = "data/locations.txt";
    int choice = -1;
    while (choice != 0) {
        cout << "\n===== CAMPUS NAVIGATION (Bus Stop Network) =====\n"
             << "1. Add location (bus stop)\n"
             << "2. Add connection between locations\n"
             << "3. Display network\n"
             << "4. Breadth-First Search (BFS)\n"
             << "5. Depth-First Search (DFS)\n"
             << "6. Is a location reachable from another?\n"
             << "7. Shortest path (Dijkstra)\n"
             << "8. Count connected components\n"
             << "9. Save network to file\n"
             << "10. Load network from file\n"
             << "0. Back\n";
        choice = readInt("Enter your choice: ");

        if (choice == 1) {
            if (graph.addStop(readText("Location name: "))) cout << "Location added." << endl;
            else cout << "Location is empty or already exists." << endl;
        } else if (choice == 2) {
            string a = readText("First location: ");
            string b = readText("Second location: ");
            double d = readDistance("Distance in km: ");
            if (graph.addConnection(a, b, d)) cout << "Connection added." << endl;
            else cout << "Failed: unknown location, same location twice, or already connected." << endl;
        } else if (choice == 3) {
            graph.display();
        } else if (choice == 4) {
            cout << "BFS order: ";
            graph.bfs(readText("Start location: "));
        } else if (choice == 5) {
            cout << "DFS order: ";
            graph.dfs(readText("Start location: "));
        } else if (choice == 6) {
            string a = readText("From: ");
            string b = readText("To: ");
            if (graph.isReachable(a, b)) cout << b << " is reachable from " << a << "." << endl;
            else cout << b << " is NOT reachable from " << a << " (or a location does not exist)." << endl;
        } else if (choice == 7) {
            string a = readText("From: ");
            string b = readText("To: ");
            graph.shortestPath(a, b);
        } else if (choice == 8) {
            cout << "Connected components: " << graph.countComponents() << endl;
        } else if (choice == 9) {
            graph.saveToFile(file);
            cout << "Saved to " << file << endl;
        } else if (choice == 10) {
            graph.loadFromFile(file);
        } else if (choice != 0) {
            cout << "Invalid choice." << endl;
        }
    }
}

// ---------------------------------------------------------------
// System analysis: measures linear search, BST search and hash search
// on growing data sets, and shows how the BST degrades on sorted input.
// ---------------------------------------------------------------
static string makeID(int i) {
    char buffer[16];
    sprintf(buffer, "V%05d", i);
    return string(buffer);
}

void systemAnalysisC() {
    const int sizes[3]       = {100, 1000, 5000};
    const int tableSizes[3]  = {101, 1009, 5003};   // primes, load factor about 1
    const int REPEATS = 20;                         // repeat fast searches to measure them

    cout << "\n===== SYSTEM ANALYSIS (Student C) =====\n"
         << "Time per successful search, in nanoseconds (average).\n\n"
         << left << setw(7) << "n"
         << setw(14) << "Linear(ns)" << setw(12) << "BST(ns)" << setw(12) << "Hash(ns)"
         << setw(14) << "Height(rand)" << setw(16) << "Height(sorted)"
         << setw(8) << "Load" << "Collisions" << endl;

    for (int s = 0; s < 3; s++) {
        int n = sizes[s];

        // build the vehicles, then shuffle them (Fisher-Yates, fixed seed)
        Vehicle* arr = new Vehicle[n];
        for (int i = 0; i < n; i++)
            arr[i] = Vehicle(makeID(i), "PLATE", "Transport", "Bus", "R1", 40, "Active");
        srand(42);
        for (int i = n - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            Vehicle temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }

        VehicleTree randomTree, sortedTree;
        HashTable table(tableSizes[s]);
        for (int i = 0; i < n; i++) {
            randomTree.insert(arr[i]);
            table.insert(arr[i]);
        }
        for (int i = 0; i < n; i++)                  // sorted insertion order
            sortedTree.insert(Vehicle(makeID(i), "PLATE", "Transport", "Bus", "R1", 40, "Active"));

        int found = 0;

        // linear search through the array
        chrono::steady_clock::time_point t0 = chrono::steady_clock::now();
        for (int q = 0; q < n; q++) {
            string target = arr[q].getID();
            for (int i = 0; i < n; i++)
                if (arr[i].getID() == target) { found++; break; }
        }
        chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
        long long linearNs = chrono::duration_cast<chrono::nanoseconds>(t1 - t0).count() / n;

        // BST search
        t0 = chrono::steady_clock::now();
        for (int r = 0; r < REPEATS; r++)
            for (int q = 0; q < n; q++)
                if (randomTree.search(arr[q].getID()) != NULL) found++;
        t1 = chrono::steady_clock::now();
        long long treeNs = chrono::duration_cast<chrono::nanoseconds>(t1 - t0).count() / ((long long)n * REPEATS);

        // hash table search
        t0 = chrono::steady_clock::now();
        for (int r = 0; r < REPEATS; r++)
            for (int q = 0; q < n; q++)
                if (table.search(arr[q].getID()) != NULL) found++;
        t1 = chrono::steady_clock::now();
        long long hashNs = chrono::duration_cast<chrono::nanoseconds>(t1 - t0).count() / ((long long)n * REPEATS);

        if (found != n + 2 * n * REPEATS)
            cout << "WARNING: some searches failed!" << endl;

        cout << left << setw(7) << n
             << setw(14) << linearNs << setw(12) << treeNs << setw(12) << hashNs
             << setw(14) << randomTree.height() << setw(16) << sortedTree.height()
             << setw(8) << fixed << setprecision(2) << table.loadFactor()
             << table.getCollisions() << endl;

        delete[] arr;
    }
    cout.unsetf(ios::floatfield);                    // restore normal number printing
    cout.precision(6);
    cout << right;
    cout << "\nReading the table:\n"
         << "- Linear search grows with n (O(n)); the BST grows slowly (O(log n));\n"
         << "  the hash table stays almost flat (O(1) on average).\n"
         << "- A BST built from shuffled IDs stays fairly short (a small multiple of\n"
         << "  log2(n)); built from sorted IDs its height equals n, so it degrades\n"
         << "  to a linked list and searching becomes O(n).\n"
         << "- The hash table stays near O(1) while the load factor stays near 1.\n" << endl;
}
