#ifndef REQUEST_MANAGER_H
#define REQUEST_MANAGER_H
#include "RequestQueue.h"
#include "RequestStack.h"
#include "PriorityQueue.h"
#include "../Vehiclelist.h"
#include <string>
class RequestManager {
private:
	RequestQueue normal;
	PriorityQueue urgent;
	RequestStack cancelled;
	int nextID;

	bool exists(int requestId) const;
	void submitRequest(const Vehiclelist& vehicles, bool priorityRequest);
	void processNextNormal();
	void processNextPriority();
	void cancelRequest();
	void scheduleActivity();
public:
	RequestManager();
	void load(const string& filename);
	void save(const string& filename) const;
	void menu(const Vehiclelist& vehicles);
	void priorityMenu(const Vehiclelist& vehicles);
	bool undoLastCancellation();
};
#endif
