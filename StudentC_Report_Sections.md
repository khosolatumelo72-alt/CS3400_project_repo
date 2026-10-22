# Student C: Report Sections (Trees, Hashing, Graphs)

Group Type E: Campus Transport. Draft text for the group report. Sections 3 to 6 cover Student C's modules. Edit the wording so it is yours, and replace the sample timing numbers with the ones from your own machine (menu option 10).

---

## 3. Data Structure Selection

### 3.1 Binary Search Tree (`Tree.h`, `Tree.cpp`)
- **Problem solved:** keep vehicles in an ordered, searchable structure keyed by Vehicle ID, so that vehicles can be listed in sorted order and found without scanning everything.
- **Why selected:** a BST gives O(log n) average insert, search and delete, and an inorder traversal gives the IDs in sorted order without a separate sort. It is built from dynamically allocated nodes (`new`), so it grows as vehicles are added.
- **Alternatives considered:**
  - *Sorted array + binary search:* fast search, but insertion and deletion are O(n) because elements must be shifted.
  - *Linked list:* easy insertion, but search is O(n).
  - *AVL or Red-Black tree:* guarantees O(log n) in the worst case. We did not implement it because of its added complexity, but it is the fix for the degenerate case shown in Section 5.

### 3.2 Hash Table (`HashTable.h`, `HashTable.cpp`)
- **Problem solved:** instant retrieval of a vehicle's full record from its ID (Vehicle ID to Vehicle).
- **Why selected:** average O(1) lookup, which is faster than the tree and far faster than the linked list.
- **Collision handling: separate chaining.** Each bucket holds a linked list of the vehicles that hash to it. Justification: the table never becomes full; deletion is a normal linked list deletion (no "deleted" markers as in open addressing); and performance degrades gradually as the load factor rises instead of collapsing.
- **Hash function:** polynomial rolling hash, `h = h * 31 + character`, followed by `h % tableSize`. The multiplier 31 is an odd prime, so IDs that differ in a single character (V001, V002) are spread across buckets. The default table size is 17, a prime, which also helps spread keys evenly.
- **Alternatives considered:** linear probing (suffers from clustering and needs tombstones for deletion), quadratic probing (avoids primary clustering but can fail to find a free slot when the table is more than half full), double hashing (best spread for open addressing but needs a second hash function and is harder to explain).

### 3.3 Graph (`Graph.h`, `Graph.cpp`)
- **Problem solved:** model the bus stops and the routes between them, so that we can traverse the network and answer reachability and shortest-route questions.
- **Why selected:** stops are joined in loops, which a tree cannot represent. The graph is undirected (a route works both ways) and weighted (distance in km).
- **Representation: adjacency list.** Space is O(V + E), and BFS and DFS run in O(V + E). A campus network is sparse (each stop connects to only a few others), so an adjacency matrix (O(V²) space) would be mostly empty.
- **Alternative considered:** adjacency matrix. Its one advantage is an O(1) check of whether two given stops are directly connected.

---

## 4. Algorithms

### 4.1 BST operations
- **Insert:** compare the new ID with the current node; go left if smaller, right if larger; reject if equal (duplicate). Insert at the first empty (`NULL`) position.
- **Search:** same comparisons; return the vehicle when the ID matches or `NULL` when a `NULL` link is reached.
- **Delete:** three cases. (1) Leaf: remove it. (2) One child: replace the node by its child. (3) Two children: copy the inorder successor (smallest node of the right subtree) into the node, then delete the successor.
- **Traversals:** inorder (left, node, right; gives sorted IDs), preorder (node, left, right), postorder (left, right, node), level-order (queue; row by row).

### 4.2 Hashing
- **Insert:** hash the ID to a bucket; walk the chain to reject duplicates; add the new node at the head of the chain (O(1)). A collision is counted when the bucket was already in use.
- **Search / remove:** hash the ID, then walk only that bucket's chain.

### 4.3 Graph algorithms
- **BFS:** a queue and a visited array. Visit the start stop, then all stops one connection away, then two, and so on. Stops are marked visited when they are enqueued, so no stop is queued twice.
- **DFS:** recursion. Follow one path as deep as possible, then backtrack. The call stack acts as the stack.
- **Difference between BFS and DFS:** BFS explores level by level and uses a queue (it finds the fewest-hops route); DFS goes deep first and uses a stack (it is simpler for exploring everything reachable). Both are O(V + E).
- **Reachability and components:** a BFS from stop A marks everything reachable; B is reachable if marked. Counting components means starting a new BFS from each unvisited stop.
- **Shortest path (Dijkstra):** keep the best-known distance to every stop; repeatedly finalise the closest unfinished stop and relax its edges (update neighbours if a shorter route is found); rebuild the route by following the `prev` array backwards.

---

## 5. Complexity Analysis

n = vehicles, V = stops, E = connections, m = number of hash buckets.

| Operation | Structure / algorithm | Best | Average | Worst | Notes |
|---|---|---|---|---|---|
| Insert vehicle | BST | O(1) | O(log n) | O(n) | Worst case: IDs inserted in sorted order, the tree becomes a linked list |
| Search vehicle | BST | O(1) | O(log n) | O(n) | Same degenerate case |
| Delete vehicle | BST | O(1) | O(log n) | O(n) | Two-children case adds a `findMin` walk |
| Inorder / any traversal | BST | O(n) | O(n) | O(n) | Every node must be visited |
| Lookup by ID | Hash table (chaining) | O(1) | O(1 + n/m) = O(1) while load factor is small | O(n) | Worst case: all keys in one bucket |
| Insert / remove | Hash table | O(1) | O(1) | O(n) | Duplicate check walks the chain |
| Search (for comparison) | Linear search on the linked list / array | O(1) | O(n) | O(n) | |
| BFS / DFS | Graph, adjacency list | O(V + E) | O(V + E) | O(V + E) | Each stop and connection handled a constant number of times |
| Reachability / components | BFS | O(V + E) | O(V + E) | O(V + E) | |
| Shortest path | Dijkstra (array scan) | O(V²) | O(V²) | O(V²) | A heap would give O(E log V) |
| Add stop | Dynamic array | O(V) | O(V) | O(V) | The cost is the duplicate-name check; array doubling is O(1) amortised |

**Space:** BST O(n); hash table O(n + m); graph O(V + E).

### Measured results (option 10, System Analysis)
Sample run, to be replaced with your own numbers. Average time per successful search in nanoseconds:

| n | Linear | BST | Hash | BST height (shuffled IDs) | BST height (sorted IDs) | Load factor | Collisions |
|---|---|---|---|---|---|---|---|
| 100 | 1246 | 430 | 60 | 14 | 100 | 0.99 | 14 |
| 1000 | 12483 | 661 | 75 | 28 | 1000 | 0.99 | 412 |
| 5000 | 64205 | 894 | 84 | 29 | 5000 | 1.00 | 1748 |

**Discussion:**
- Linear search time grows roughly in proportion to n (O(n)).
- BST time grows slowly (O(log n)) when IDs arrive in a scrambled order.
- Hash time stays almost flat (O(1) average) because the load factor is kept near 1.
- When IDs are inserted in sorted order, the BST height equals n: the tree has degenerated into a linked list and its operations become O(n). This is the worst case in the table above, and a balanced tree (AVL) would prevent it.

---

## 6. Testing (Student C modules)

`test_studentC.cpp` runs 61 automated checks. All 61 passed, including under AddressSanitizer (no memory errors or leaks).

| Category | Test | Result |
|---|---|---|
| Normal | Insert, search, four traversals, delete (BST); insert, lookup, remove (hash); build map, BFS, DFS, shortest path (graph) | Pass |
| Boundary | Empty tree, empty hash table, empty graph; one-bucket hash table; deleting leaf, one-child, two-child and root nodes | Pass |
| Invalid | Duplicate ID (tree and hash); search/remove missing ID; unknown stop, self-loop, duplicate and negative-distance connections; BFS from unknown stop; missing data file; invalid hash table size | Pass |
| Large data | 5000 vehicles in tree and hash table (inserted, all found, half deleted); chain of 2000 stops (components, reachability, shortest path = 1999 km, DFS recursion) | Pass |
| Menus | Letters typed in place of numbers, empty IDs, out-of-range choices | Handled with error messages |

---

## 7. Challenges and Lessons Learned (technical, Student C)
- **Deleting a BST node with two children** is the hardest case: the replacement must keep the ordering rule, which is why the inorder successor is used.
- **Memory management:** every `new` (tree nodes, hash chain nodes, graph edges, dynamic arrays) needs a matching `delete` in a destructor; this was verified with a memory checker.
- **Undirected graph:** each connection must be stored as two edges, and the file writer must write each connection only once.
- **BFS and DFS visit order:** marking a stop as visited when it is queued (not when it is processed) prevents duplicates.
- **Degenerate trees:** the System Analysis showed that sorted input destroys the BST's O(log n) behaviour. This motivated the discussion of balanced trees.
- *Team collaboration and design challenges: write these yourself from your group's experience (for example agreeing on the `Vehicle` class interface with Student A, and who edits `main.cpp`).*

---

## Defence Questions (viva practice)

1. **Why a BST and not a sorted array?** Sorted arrays search fast but insertion and deletion shift elements, O(n). A BST inserts and deletes in O(log n) on average.
2. **What is the worst case of your BST?** O(n), when keys are inserted in sorted order. The height equals n. An AVL tree fixes it by rebalancing.
3. **Explain deleting a node with two children.** Copy the smallest value of the right subtree (inorder successor) into the node, then delete that successor from the right subtree; ordering is preserved.
4. **Which traversal gives sorted output, and why?** Inorder: it visits the left subtree (smaller IDs), then the node, then the right subtree (larger IDs).
5. **Why does level-order use a queue?** It must visit nodes in the order they were discovered, which is first-in-first-out.
6. **What is a collision and how do you handle it?** Two IDs hash to the same bucket. Each bucket holds a linked list (separate chaining).
7. **Why chaining instead of linear probing?** The table never fills up, delete is simple, and it degrades gradually. Probing needs tombstones and clusters.
8. **Why 31 and a prime table size?** An odd prime multiplier mixes characters well, and a prime modulus reduces patterns in how keys map to buckets.
9. **What is the load factor?** count / tableSize: the average chain length. Near 1 or below gives O(1) lookups.
10. **Adjacency list or matrix, and why?** List: the network is sparse, so space is O(V + E) instead of O(V²), and traversal is O(V + E).
11. **BFS vs DFS?** BFS uses a queue, goes level by level and finds the fewest hops. DFS uses recursion (a stack), goes deep first. Both O(V + E).
12. **How does Dijkstra work and what is its complexity?** It repeatedly finalises the closest unfinished stop and relaxes its edges. O(V²) with an array scan; O(E log V) with a heap. It needs non-negative weights, which is why `addConnection` rejects negative distances.
13. **What would you change to make it faster?** Use a heap (priority queue) in Dijkstra; use an AVL tree to guarantee O(log n); resize the hash table when the load factor passes 1.
