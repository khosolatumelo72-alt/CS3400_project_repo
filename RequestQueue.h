#ifndef REQUEST_QUEUE_H
#define REQUEST_QUEUE_H
#include "Request.h"
class RequestQueue {
private:
	struct Node {
		Request value;
		Node* next;
		Node(const Request& request) : value(request), next(NULL) {}
	};

	Node* frontNode;
	Node* rearNode;
	int count;

public:
	RequestQueue();
	~RequestQueue();
	bool enqueue(const Request& request);
	bool dequeue(Request& request);
	bool removeID(int requestId, Request& removed);
	bool containsID(int requestId) const;
	bool empty() const;
	void display() const;
	void save(ostream& output) const;
};
#endif
