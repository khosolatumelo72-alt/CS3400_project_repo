#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H
#include "Request.h"
class PriorityQueue {
private:
	Request* heap;
	int count;
	int capacity;
	bool before(const Request& first, const Request& second) const;
	void grow();

public:
	PriorityQueue();
	~PriorityQueue();
	bool insert(const Request& request);
	bool removeTop(Request& request);
	bool removeID(int requestId, Request& removed);
	bool containsID(int requestId) const;
	bool empty() const;
	void display() const;
	void save(ostream& output) const;
};
#endif
