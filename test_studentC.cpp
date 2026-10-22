// Test driver for Student C's modules (Tree, HashTable, Graph).
// It is separate from main.cpp. Build with:
//   g++ test_studentC.cpp Tree.cpp HashTable.cpp Graph.cpp MenuC.cpp Vehicle.cpp -o testC
#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
#include "Tree.h"
#include "HashTable.h"
#include "Graph.h"
#include "MenuC.h"
using namespace std;

static int passed = 0;
static int failed = 0;

static void check(bool condition, const string& name) {
    if (condition) { passed++; cout << "[PASS] " << name << endl; }
    else           { failed++; cout << "[FAIL] " << name << endl; }
}

// Redirects cout into a string while it exists, so printed output can be checked.
class OutputCapture {
private:
    ostringstream buffer;
    streambuf* oldBuffer;
public:
    OutputCapture()  { oldBuffer = cout.rdbuf(buffer.rdbuf()); }
    ~OutputCapture() { cout.rdbuf(oldBuffer); }
    string text() const { return buffer.str(); }
};

static Vehicle makeVehicle(const string& id) {
    return Vehicle(id, "PLATE-" + id, "Bus", "Route1", 40, "Active");
}

static string makeID(int i) {
    char buffer[16];
    sprintf(buffer, "V%05d", i);
    return string(buffer);
}

// ------------------------------------------------------------ TREE
static void testTree() {
    cout << "\n--- TREE TESTS ---" << endl;

    // boundary: empty tree
    VehicleTree empty;
    check(empty.isEmpty() && empty.size() == 0 && empty.height() == 0, "Tree: empty tree has size 0 and height 0");
    check(empty.search("V001") == NULL, "Tree: search in empty tree returns NULL");
    check(!empty.remove("V001"), "Tree: remove from empty tree returns false");
    { OutputCapture c; empty.levelOrder(); check(c.text().find("empty") != string::npos, "Tree: level-order on empty tree prints a message"); }

    // normal: insert 7 vehicles (tree shape: V005 root, V003/V008, V001/V004/V007/V009)
    VehicleTree tree;
    const char* ids[7] = {"V005", "V003", "V008", "V001", "V004", "V007", "V009"};
    bool allInserted = true;
    for (int i = 0; i < 7; i++) allInserted = allInserted && tree.insert(makeVehicle(ids[i]));
    check(allInserted && tree.size() == 7, "Tree: 7 vehicles inserted");
    check(tree.height() == 3, "Tree: height is 3");

    // invalid: duplicate
    check(!tree.insert(makeVehicle("V005")) && tree.size() == 7, "Tree: duplicate ID rejected");

    // search
    Vehicle* found = tree.search("V007");
    check(found != NULL && found->getplate() == "PLATE-V007", "Tree: search finds correct vehicle");
    check(tree.search("V999") == NULL, "Tree: search for missing ID returns NULL");

    // traversals
    { OutputCapture c; tree.inorder(); string o = c.text();
      check(o.find("V001") < o.find("V003") && o.find("V003") < o.find("V004") && o.find("V004") < o.find("V005") &&
            o.find("V005") < o.find("V007") && o.find("V007") < o.find("V008") && o.find("V008") < o.find("V009"),
            "Tree: inorder prints IDs in sorted order"); }
    { OutputCapture c; tree.preorder(); string o = c.text();
      check(o.find("V005") == 0 && o.find("V003") < o.find("V008"), "Tree: preorder starts at root, left before right"); }
    { OutputCapture c; tree.postorder(); string o = c.text();
      check(o.find("V009") < o.find("V005") && o.find("V001") < o.find("V003"), "Tree: postorder visits root last"); }
    { OutputCapture c; tree.levelOrder(); string o = c.text();
      check(o.find("V005") == 0 && o.find("V008") < o.find("V001"), "Tree: level-order visits row by row"); }

    // delete: leaf, one child, two children, root, missing
    check(tree.remove("V001") && tree.search("V001") == NULL && tree.size() == 6, "Tree: delete leaf");
    check(tree.remove("V003") && tree.search("V004") != NULL && tree.size() == 5, "Tree: delete node with one child");
    check(tree.remove("V008") && tree.search("V007") != NULL && tree.search("V009") != NULL && tree.size() == 4, "Tree: delete node with two children");
    check(tree.remove("V005") && tree.search("V005") == NULL && tree.size() == 3, "Tree: delete root");
    check(tree.search("V004") != NULL && tree.search("V007") != NULL && tree.search("V009") != NULL, "Tree: remaining vehicles still found");
    check(!tree.remove("V555"), "Tree: delete missing ID returns false");

    // large data
    VehicleTree big;
    bool ok = true;
    for (int i = 0; i < 5000; i++) ok = ok && big.insert(makeVehicle(makeID((i * 7919) % 5000)));
    check(ok && big.size() == 5000, "Tree: 5000 vehicles inserted (scrambled order)");
    ok = true;
    for (int i = 0; i < 5000; i++) ok = ok && big.search(makeID(i)) != NULL;
    check(ok, "Tree: all 5000 vehicles found");
    for (int i = 0; i < 5000; i += 2) big.remove(makeID(i));
    check(big.size() == 2500 && big.search(makeID(2)) == NULL && big.search(makeID(3)) != NULL, "Tree: 2500 vehicles deleted correctly");
}

// ------------------------------------------------------------ HASH TABLE
static void testHashTable() {
    cout << "\n--- HASH TABLE TESTS ---" << endl;

    HashTable empty(5);
    check(empty.isEmpty() && empty.size() == 0, "Hash: new table is empty");
    check(empty.search("V001") == NULL && !empty.remove("V001"), "Hash: search/remove on empty table fail safely");

    // normal + collisions (12 vehicles in 5 buckets must collide)
    HashTable table(5);
    bool ok = true;
    for (int i = 1; i <= 12; i++) ok = ok && table.insert(makeVehicle(makeID(i)));
    check(ok && table.size() == 12, "Hash: 12 vehicles inserted");
    check(table.getCollisions() >= 7, "Hash: collisions detected (12 items, 5 buckets)");
    ok = true;
    for (int i = 1; i <= 12; i++) ok = ok && table.search(makeID(i)) != NULL;
    check(ok, "Hash: all vehicles found despite collisions");
    check(!table.insert(makeVehicle("V00003")) && table.size() == 12, "Hash: duplicate ID rejected");
    check(table.search("V99999") == NULL, "Hash: search for missing ID returns NULL");
    check(table.search("V00007")->getplate() == "PLATE-V00007", "Hash: lookup returns correct vehicle");

    check(table.remove("V00001") && table.search("V00001") == NULL && table.size() == 11, "Hash: remove vehicle");
    check(!table.remove("V00001"), "Hash: remove missing ID returns false");
    ok = true;
    for (int i = 2; i <= 12; i++) ok = ok && table.search(makeID(i)) != NULL;
    check(ok, "Hash: other vehicles unaffected by removal");

    // boundary: one bucket, everything collides
    HashTable oneBucket(1);
    for (int i = 1; i <= 10; i++) oneBucket.insert(makeVehicle(makeID(i)));
    check(oneBucket.getCollisions() == 9 && oneBucket.search("V00010") != NULL && oneBucket.remove("V00005"),
          "Hash: table with one bucket still works (long chain)");

    // invalid size falls back to default
    HashTable bad(0);
    check(bad.insert(makeVehicle("V1")) && bad.search("V1") != NULL, "Hash: invalid size handled");

    // large data, small table: long chains but still correct
    HashTable big(101);
    ok = true;
    for (int i = 0; i < 5000; i++) ok = ok && big.insert(makeVehicle(makeID(i)));
    check(ok && big.size() == 5000, "Hash: 5000 vehicles inserted");
    ok = true;
    for (int i = 0; i < 5000; i++) ok = ok && big.search(makeID(i)) != NULL;
    check(ok, "Hash: all 5000 vehicles found");
}

// ------------------------------------------------------------ GRAPH
static void testGraph() {
    cout << "\n--- GRAPH TESTS ---" << endl;

    // boundary: empty graph
    Graph empty;
    check(empty.isEmpty() && empty.getNumStops() == 0 && empty.countComponents() == 0, "Graph: empty graph");
    check(!empty.isReachable("A", "B"), "Graph: reachability on empty graph is false");
    { OutputCapture c; empty.bfs("A"); empty.dfs("A"); empty.shortestPath("A", "B"); empty.display();
      check(c.text().find("not found") != string::npos, "Graph: traversals on empty graph do not crash"); }

    // build:  A--B(1)  A--C(5)  B--D(1)  C--D(1)   E is isolated
    Graph g(2);   // small capacity forces the arrays to grow
    check(g.addStop("A") && g.addStop("B") && g.addStop("C") && g.addStop("D") && g.addStop("E") && g.getNumStops() == 5,
          "Graph: 5 stops added (array grows past capacity 2)");
    check(!g.addStop("A") && !g.addStop(""), "Graph: duplicate and empty stop names rejected");
    check(g.addConnection("A", "B", 1) && g.addConnection("A", "C", 5) && g.addConnection("B", "D", 1) && g.addConnection("C", "D", 1),
          "Graph: 4 connections added");
    check(!g.addConnection("A", "Z", 1), "Graph: connection to unknown stop rejected");
    check(!g.addConnection("A", "A", 1), "Graph: connection from a stop to itself rejected");
    check(!g.addConnection("A", "B", 2), "Graph: duplicate connection rejected");
    check(!g.addConnection("A", "E", -3), "Graph: negative distance rejected");

    // traversals
    { OutputCapture c; g.bfs("A"); string o = c.text();
      check(o.find("A") == 0 && o.find("D") != string::npos && o.find("E") == string::npos, "Graph: BFS starts at A, reaches D, skips isolated E"); }
    { OutputCapture c; g.dfs("A"); string o = c.text();
      check(o.find("A") == 0 && o.find("B") != string::npos && o.find("C") != string::npos &&
            o.find("D") != string::npos && o.find("E") == string::npos, "Graph: DFS visits the connected stops only"); }
    { OutputCapture c; g.bfs("Nowhere"); check(c.text().find("not found") != string::npos, "Graph: BFS from unknown stop handled"); }

    // advanced features
    check(g.isReachable("A", "D"), "Graph: D is reachable from A");
    check(!g.isReachable("A", "E"), "Graph: isolated E is not reachable");
    check(g.countComponents() == 2, "Graph: 2 connected components");
    { OutputCapture c; g.shortestPath("A", "D"); string o = c.text();
      check(o.find("A -> B -> D") != string::npos && o.find("Total distance: 2") != string::npos,
            "Graph: shortest path A to D is A -> B -> D (2 km), not via C"); }
    { OutputCapture c; g.shortestPath("A", "E"); check(c.text().find("No route") != string::npos, "Graph: no route to isolated stop reported"); }

    // persistence round trip
    g.saveToFile("test_locations_tmp.txt");
    Graph loaded;
    loaded.loadFromFile("test_locations_tmp.txt");
    check(loaded.getNumStops() == 5 && loaded.countComponents() == 2 && loaded.isReachable("A", "D"), "Graph: save and load round trip");
    remove("test_locations_tmp.txt");
    { OutputCapture c; Graph missing; missing.loadFromFile("no_such_file.txt"); check(c.text().find("Could not open") != string::npos, "Graph: loading a missing file handled"); }

    // large data: a chain of 2000 stops
    Graph chain;
    bool ok = true;
    for (int i = 0; i < 2000; i++) ok = ok && chain.addStop("S" + makeID(i));
    for (int i = 0; i < 1999; i++) ok = ok && chain.addConnection("S" + makeID(i), "S" + makeID(i + 1), 1);
    check(ok && chain.getNumStops() == 2000, "Graph: chain of 2000 stops built");
    check(chain.countComponents() == 1 && chain.isReachable("S" + makeID(0), "S" + makeID(1999)), "Graph: chain is one component, end reachable");
    { OutputCapture c; chain.shortestPath("S" + makeID(0), "S" + makeID(1999));
      check(c.text().find("Total distance: 1999") != string::npos, "Graph: shortest path across 2000 stops is 1999 km"); }
    { OutputCapture c; chain.dfs("S" + makeID(0)); check(c.text().find("S" + makeID(1999)) != string::npos, "Graph: DFS recursion handles 2000 stops"); }
}

int main() {
    testTree();
    testHashTable();
    testGraph();
    cout << "\n==============================\n"
         << "Passed: " << passed << "   Failed: " << failed << endl;
    systemAnalysisC();
    return failed == 0 ? 0 : 1;
}
