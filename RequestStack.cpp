#include "RequestStack.h"

RequestStack::RequestStack() : topNode(NULL) {
}

RequestStack::~RequestStack() {
	Request discarded;
	while (pop(discarded)) {
	}
}

void RequestStack::push(const Request& request) {
	topNode = new Node(request, topNode);
}

bool RequestStack::pop(Request& request) {
	if (topNode == NULL) {
		return false;
	}

	Node* oldTop = topNode;
	request = oldTop->value;
	topNode = oldTop->next;
	delete oldTop;
	return true;
}

bool RequestStack::empty() const {
	return topNode == NULL;
}
