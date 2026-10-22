#include "Graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;

Edge::Edge(int t, double w) : to(t), weight(w), next(NULL) {}

// ---------- construction / destruction ----------
Graph::Graph(int initialCapacity) : numStops(0), capacity(initialCapacity) {
    if (capacity < 1) capacity = 10;
    stops = new string[capacity];
    adj = new Edge*[capacity];
    for (int i = 0; i < capacity; i++)
        adj[i] = NULL;
}

Graph::~Graph() {
    for (int i = 0; i < numStops; i++) {
        Edge* cur = adj[i];
        while (cur != NULL) {
            Edge* nextEdge = cur->next;
            delete cur;
            cur = nextEdge;
        }
    }
    delete[] adj;
    delete[] stops;
}

// Double the capacity: allocate bigger arrays, copy, free the old ones.
void Graph::grow() {
    int newCap = capacity * 2;
    string* newStops = new string[newCap];
    Edge** newAdj = new Edge*[newCap];
    for (int i = 0; i < newCap; i++) newAdj[i] = NULL;
    for (int i = 0; i < numStops; i++) {
        newStops[i] = stops[i];
        newAdj[i] = adj[i];
    }
    delete[] stops;
    delete[] adj;
    stops = newStops;
    adj = newAdj;
    capacity = newCap;
}

// ---------- building the map ----------
int Graph::indexOf(const string& name) const {
    for (int i = 0; i < numStops; i++)
        if (stops[i] == name) return i;
    return -1;
}

bool Graph::addStop(const string& name) {
    if (name.empty() || indexOf(name) != -1) return false;
    if (numStops == capacity) grow();
    stops[numStops] = name;
    adj[numStops] = NULL;
    numStops++;
    return true;
}

bool Graph::addConnection(const string& a, const string& b, double dist) {
    int ia = indexOf(a), ib = indexOf(b);
    if (ia == -1 || ib == -1 || ia == ib || dist < 0) return false;

    for (Edge* e = adj[ia]; e != NULL; e = e->next)
        if (e->to == ib) return false;              // already connected

    Edge* ab = new Edge(ib, dist);                  // undirected: two edges
    ab->next = adj[ia];
    adj[ia] = ab;

    Edge* ba = new Edge(ia, dist);
    ba->next = adj[ib];
    adj[ib] = ba;
    return true;
}

void Graph::display() const {
    if (numStops == 0) {
        cout << "No locations in the network." << endl;
        return;
    }
    for (int i = 0; i < numStops; i++) {
        cout << stops[i] << " ->";
        if (adj[i] == NULL) cout << " (no connections)";
        for (Edge* e = adj[i]; e != NULL; e = e->next)
            cout << " " << stops[e->to] << " (" << e->weight << " km)";
        cout << endl;
    }
}

int  Graph::getNumStops() const { return numStops; }
bool Graph::isEmpty() const     { return numStops == 0; }

// ---------- BFS (queue) ----------
void Graph::bfs(const string& start) const {
    int s = indexOf(start);
    if (s == -1) {
        cout << "Location not found: " << start << endl;
        return;
    }
    bool* visited = new bool[numStops];
    int* queue = new int[numStops];                 // each stop enqueued once
    for (int i = 0; i < numStops; i++) visited[i] = false;

    int front = 0, rear = 0;
    queue[rear++] = s;
    visited[s] = true;
    bool first = true;
    while (front < rear) {
        int v = queue[front++];
        cout << (first ? "" : " -> ") << stops[v];
        first = false;
        for (Edge* e = adj[v]; e != NULL; e = e->next) {
            if (!visited[e->to]) {
                visited[e->to] = true;              // mark when enqueued
                queue[rear++] = e->to;
            }
        }
    }
    cout << endl;
    delete[] queue;
    delete[] visited;
}

// ---------- DFS (recursion = call stack) ----------
void Graph::dfsRec(int v, bool* visited) const {
    visited[v] = true;
    cout << stops[v] << "  ";
    for (Edge* e = adj[v]; e != NULL; e = e->next)
        if (!visited[e->to])
            dfsRec(e->to, visited);
}

void Graph::dfs(const string& start) const {
    int s = indexOf(start);
    if (s == -1) {
        cout << "Location not found: " << start << endl;
        return;
    }
    bool* visited = new bool[numStops];
    for (int i = 0; i < numStops; i++) visited[i] = false;
    dfsRec(s, visited);
    cout << endl;
    delete[] visited;
}

// ---------- reachability and components ----------
// BFS that marks everything reachable from start; returns how many.
int Graph::componentSize(int start, bool* visited) const {
    int* queue = new int[numStops];
    int front = 0, rear = 0;
    queue[rear++] = start;
    visited[start] = true;
    while (front < rear) {
        int v = queue[front++];
        for (Edge* e = adj[v]; e != NULL; e = e->next)
            if (!visited[e->to]) {
                visited[e->to] = true;
                queue[rear++] = e->to;
            }
    }
    delete[] queue;
    return rear;
}

bool Graph::isReachable(const string& a, const string& b) const {
    int ia = indexOf(a), ib = indexOf(b);
    if (ia == -1 || ib == -1) return false;
    bool* visited = new bool[numStops];
    for (int i = 0; i < numStops; i++) visited[i] = false;
    componentSize(ia, visited);
    bool result = visited[ib];
    delete[] visited;
    return result;
}

int Graph::countComponents() const {
    if (numStops == 0) return 0;
    bool* visited = new bool[numStops];
    for (int i = 0; i < numStops; i++) visited[i] = false;
    int components = 0;
    for (int i = 0; i < numStops; i++) {
        if (!visited[i]) {
            components++;
            componentSize(i, visited);
        }
    }
    delete[] visited;
    return components;
}

// ---------- Dijkstra's shortest path ----------
void Graph::shortestPath(const string& a, const string& b) const {
    int s = indexOf(a), t = indexOf(b);
    if (s == -1 || t == -1) {
        cout << "One or both locations not found." << endl;
        return;
    }
    const double INF = 1e18;
    double* dist = new double[numStops];
    int* prev = new int[numStops];
    bool* done = new bool[numStops];
    for (int i = 0; i < numStops; i++) {
        dist[i] = INF;
        prev[i] = -1;
        done[i] = false;
    }
    dist[s] = 0;

    for (int round = 0; round < numStops; round++) {
        int u = -1;                                  // closest unfinished stop
        for (int i = 0; i < numStops; i++)
            if (!done[i] && (u == -1 || dist[i] < dist[u])) u = i;
        if (u == -1 || dist[u] >= INF) break;        // rest unreachable
        done[u] = true;
        for (Edge* e = adj[u]; e != NULL; e = e->next) {
            if (dist[u] + e->weight < dist[e->to]) { // found a shorter route
                dist[e->to] = dist[u] + e->weight;
                prev[e->to] = u;
            }
        }
    }

    if (dist[t] >= INF) {
        cout << "No route from " << a << " to " << b << "." << endl;
    } else {
        int* path = new int[numStops];
        int len = 0;
        for (int v = t; v != -1; v = prev[v]) path[len++] = v;  // built backwards
        cout << "Shortest route: ";
        for (int i = len - 1; i >= 0; i--) {
            cout << stops[path[i]];
            if (i > 0) cout << " -> ";
        }
        cout << "\nTotal distance: " << dist[t] << " km" << endl;
        delete[] path;
    }
    delete[] dist;
    delete[] prev;
    delete[] done;
}

// ---------- file persistence ----------
void Graph::saveToFile(const string& filename) const {
    ofstream out(filename.c_str());
    if (!out) {
        cout << "Could not open " << filename << " for writing." << endl;
        return;
    }
    for (int i = 0; i < numStops; i++)
        out << "STOP|" << stops[i] << "\n";
    for (int i = 0; i < numStops; i++)
        for (Edge* e = adj[i]; e != NULL; e = e->next)
            if (i < e->to)                           // write each edge once
                out << "EDGE|" << stops[i] << "|" << stops[e->to] << "|" << e->weight << "\n";
}

void Graph::loadFromFile(const string& filename) {
    ifstream in(filename.c_str());
    if (!in) {
        cout << "Could not open " << filename << " (starting with an empty network)." << endl;
        return;
    }
    string line;
    while (getline(in, line)) {
        if (!line.empty() && line[line.length() - 1] == '\r')
            line.erase(line.length() - 1);           // Windows line endings
        if (line.empty()) continue;

        stringstream ss(line);
        string type, f1, f2, f3;
        getline(ss, type, '|');
        getline(ss, f1, '|');
        getline(ss, f2, '|');
        getline(ss, f3, '|');

        if (type == "STOP")
            addStop(f1);
        else if (type == "EDGE")
            addConnection(f1, f2, atof(f3.c_str()));
    }
}
