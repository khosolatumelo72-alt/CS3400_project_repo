#include "PriorityQueue.h"

PriorityQueue::PriorityQueue() : heap(new Request[8]), count(0), capacity(8) {
}

PriorityQueue::~PriorityQueue() {
	delete[] heap;
}

bool PriorityQueue::before(const Request& first, const Request& second) const {
	if (first.getPriority() != second.getPriority()) {
		return first.getPriority() < second.getPriority();
	}
	return first.getID() < second.getID();
}

void PriorityQueue::grow() {
	int newCapacity = capacity * 2;
	Request* newHeap = new Request[newCapacity];
	for (int i = 0; i < count; ++i) {
		newHeap[i] = heap[i];
	}
	delete[] heap;
	heap = newHeap;
	capacity = newCapacity;
}

bool PriorityQueue::insert(const Request& request) {
	if (request.getPriority() < 1 || request.getPriority() > 4) {
		return false;
	}
	if (count == capacity) {
		grow();
	}

	int child = count++;
	while (child > 0) {
		int parent = (child - 1) / 2;
		if (!before(request, heap[parent])) {
			break;
		}
		heap[child] = heap[parent];
		child = parent;
	}
	heap[child] = request;
	return true;
}

bool PriorityQueue::removeTop(Request& request) {
	if (count == 0) {
		return false;
	}

	request = heap[0];
	Request last = heap[--count];
	if (count == 0) {
		return true;
	}

	int parent = 0;
	while (2 * parent + 1 < count) {
		int bestChild = 2 * parent + 1;
		int rightChild = bestChild + 1;
		if (rightChild < count && before(heap[rightChild], heap[bestChild])) {
			bestChild = rightChild;
		}
		if (!before(heap[bestChild], last)) {
			break;
		}
		heap[parent] = heap[bestChild];
		parent = bestChild;
	}
	heap[parent] = last;
	return true;
}

bool PriorityQueue::removeID(int requestId, Request& removed) {
	int index = 0;
	while (index < count && heap[index].getID() != requestId) {
		++index;
	}
	if (index == count) {
		return false;
	}

	removed = heap[index];
	Request last = heap[--count];
	if (index == count) {
		return true;
	}
	heap[index] = last;

	if (index > 0 && before(heap[index], heap[(index - 1) / 2])) {
		while (index > 0) {
			int parent = (index - 1) / 2;
			if (!before(heap[index], heap[parent])) {
				break;
			}
			Request temp = heap[index];
			heap[index] = heap[parent];
			heap[parent] = temp;
			index = parent;
		}
	} else {
		while (2 * index + 1 < count) {
			int bestChild = 2 * index + 1;
			int rightChild = bestChild + 1;
			if (rightChild < count && before(heap[rightChild], heap[bestChild])) {
				bestChild = rightChild;
			}
			if (!before(heap[bestChild], heap[index])) {
				break;
			}
			Request temp = heap[index];
			heap[index] = heap[bestChild];
			heap[bestChild] = temp;
			index = bestChild;
		}
	}
	return true;
}

bool PriorityQueue::containsID(int requestId) const {
	for (int i = 0; i < count; ++i) {
		if (heap[i].getID() == requestId) {
			return true;
		}
	}
	return false;
}

bool PriorityQueue::empty() const {
	return count == 0;
}

void PriorityQueue::display() const {
	if (count == 0) {
		cout << "No priority requests pending.\n";
		return;
	}

	PriorityQueue orderedCopy;
	for (int i = 0; i < count; ++i) {
		orderedCopy.insert(heap[i]);
	}
	Request request;
	while (orderedCopy.removeTop(request)) {
		request.display();
	}
}

void PriorityQueue::save(ostream& output) const {
	for (int i = 0; i < count; ++i) {
		const Request& request = heap[i];
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
