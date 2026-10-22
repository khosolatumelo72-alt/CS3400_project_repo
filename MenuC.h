#ifndef MENUC_H
#define MENUC_H
// Menu functions for Student C's modules (menu options 7, 8, 9 and the
// Student C part of option 10). main.cpp only needs to call these.
#include "Tree.h"
#include "HashTable.h"
#include "Graph.h"

// Copies vehicles (for example from Student A's Vehiclelist::toArray())
// into the tree and the hash table.
void loadVehiclesIntoStructures(const Vehicle* vehicles, int n,
                                VehicleTree& tree, HashTable& table);

void treeMenu(VehicleTree& tree);       // menu option 7
void hashMenu(HashTable& table);        // menu option 8
void navigationMenu(Graph& graph);      // menu option 9
void systemAnalysisC();                 // Student C part of menu option 10
#endif
