#ifndef REQUEST_STACK_H
#define REQUEST_STACK_H
#include "Request.h"
class RequestStack {
private:
	struct Node {
		Request value;
		Node* next;
		Node(const Request& request, Node* previousTop)
			: value(request), next(previousTop) {}
	};

	Node* topNode;

public:
	RequestStack();
	~RequestStack();
	void push(const Request& request);
	bool pop(Request& request);
	bool empty() const;
};
#endif
