#include "RequestQueue.h"

RequestQueue::RequestQueue() : frontNode(NULL), rearNode(NULL), count(0) {
}

RequestQueue::~RequestQueue() {
	Request discarded;
	while (dequeue(discarded)) {
	}
}

bool RequestQueue::enqueue(const Request& request) {
	Node* newNode = new Node(request);
	if (rearNode == NULL) {
		frontNode = newNode;
		rearNode = newNode;
	} else {
		rearNode->next = newNode;
		rearNode = newNode;
	}
	++count;
	return true;
}

bool RequestQueue::dequeue(Request& request) {
	if (frontNode == NULL) {
		return false;
	}

	Node* oldFront = frontNode;
	request = oldFront->value;
	frontNode = oldFront->next;
	if (frontNode == NULL) {
		rearNode = NULL;
	}
	delete oldFront;
	--count;
	return true;
}

bool RequestQueue::removeID(int requestId, Request& removed) {
	Node* previous = NULL;
	Node* current = frontNode;
	while (current != NULL && current->value.getID() != requestId) {
		previous = current;
		current = current->next;
	}
	if (current == NULL) {
		return false;
	}

	removed = current->value;
	if (previous != NULL) {
		previous->next = current->next;
	} else {
		frontNode = current->next;
	}
	if (rearNode == current) {
		rearNode = previous;
	}
	delete current;
	--count;
	return true;
}

bool RequestQueue::containsID(int requestId) const {
	for (Node* current = frontNode; current != NULL; current = current->next) {
		if (current->value.getID() == requestId) {
			return true;
		}
	}
	return false;
}

bool RequestQueue::empty() const {
	return frontNode == NULL;
}

void RequestQueue::display() const {
	if (frontNode == NULL) {
		cout << "No normal requests pending.\n";
		return;
	}
	for (Node* current = frontNode; current != NULL; current = current->next) {
		current->value.display();
	}
}

void RequestQueue::save(ostream& output) const {
	for (Node* current = frontNode; current != NULL; current = current->next) {
		const Request& request = current->value;
		output << request.getID() << '|'
			   << request.getUser() << '|'
			   << request.getVehicleID() << '|'
			   << request.getPickup() << '|'
			   << request.getDestination() << '|'
			   << request.getTime() << '|'
			   << request.getStatus() << '|'
			   << request.getPriority() << '\n';
	}
}
