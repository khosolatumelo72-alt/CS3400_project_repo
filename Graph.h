#ifndef GRAPH_H
#define GRAPH_H
#include <string>
using namespace std;

// One connection leaving a stop: where it goes and how far it is.
class Edge {
public:
    int to;          // index of the destination stop
    double weight;   // distance in km
    Edge* next;      // next connection from the same stop
    Edge(int t, double w);
};

// Campus bus network: stops are vertices, routes between stops are edges.
// Undirected and weighted, stored as an adjacency list.
class Graph {
private:
    string* stops;     // stops[i] = name of stop i
    Edge** adj;        // adj[i] = head of the edge list of stop i
    int numStops;
    int capacity;

    void grow();                                   // double the arrays when full
    void dfsRec(int v, bool* visited) const;
    int  componentSize(int start, bool* visited) const;

public:
    Graph(int initialCapacity = 10);
    ~Graph();
    Graph(const Graph&) = delete;                         // copying would double-delete edges
    Graph& operator=(const Graph&) = delete;

    bool addStop(const string& name);                                   // false if duplicate
    bool addConnection(const string& a, const string& b, double dist);  // false if unknown stop or already connected
    int  indexOf(const string& name) const;                             // -1 if not found
    void display() const;
    int  getNumStops() const;
    bool isEmpty() const;

    void bfs(const string& start) const;
    void dfs(const string& start) const;

    bool isReachable(const string& a, const string& b) const;
    int  countComponents() const;
    void shortestPath(const string& a, const string& b) const;

    void saveToFile(const string& filename) const;
    void loadFromFile(const string& filename);
};
#endif
