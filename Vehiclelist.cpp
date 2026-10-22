#include "Vehiclelist.h"
using namespace std;
Node::Node(const Vehicle& vehi): data(vehi),next(NULL){
}
Vehiclelist::Vehiclelist(): head(NULL),tail(NULL),size(0){
}

Vehiclelist::~Vehiclelist(){
	while(head){
		Node*temp=head;
		head=head->next;
		delete temp;
	}
}
Node* Vehiclelist::search(const string& id){
	for(Node* current=head;current;current=current->next)
		if(current->data.getID()==id) return current;
	return NULL;	
}
bool Vehiclelist::exists(const string& id) const {
    Node* current = head;
    while (current != NULL) {
        if (current->data.getID() == id) return true;
        current = current->next;
    }
    return false;
}

bool Vehiclelist::insertFront(const Vehicle& vehi){
	if(search(vehi.getID())) return false;
	Node* n=new Node(vehi);
	n->next=head;
	head=n;
	if(!tail) tail=n;
	size++;
	return true;
}

bool Vehiclelist::insertEnd(const Vehicle& vehi){
	if(search(vehi.getID())) return false;
	Node* n=new Node(vehi);
	if(!head) head=tail=n;
	else{
		tail->next=n;
		tail=n;
	}
	size++;
	return true;
}
bool Vehiclelist::remove(const string& ID){
	Node* cur = head;
    Node* prev = NULL;
    while (cur != NULL && cur->data.getID() != ID) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == NULL) return false;
    if (prev != NULL) prev->next = cur->next;
    else head = cur->next;
    if (cur == tail) tail = prev;
    delete cur;
    size--;
    return true;
}


void Vehiclelist::display() const {
    if (head == NULL) { cout << "No vehicles.\n"; return; }
    for (Node* c = head; c != NULL; c = c->next)
        c->data.display();
}

int Vehiclelist::getsize()const { return size; }
bool Vehiclelist::isEmpty() const { return size == 0; }

Vehicle* Vehiclelist::toArray()const {
    Vehicle* arr = new Vehicle[size];
    int i = 0;
    for (Node* current = head; current != NULL; current = current->next)
        arr[i++] = current->data;
    return arr;
}

void Vehiclelist::saveToFile(const string& filename) const {
    ofstream out(filename.c_str());
    for (Node* c = head; c != NULL; c = c->next)
        out << c->data.toLine() << "\n";
    out.close();
}

void Vehiclelist::loadFromFile(const string& filename) {
    ifstream in(filename.c_str());
    if (!in) return;
    string line;
    while (getline(in, line)) {
        if (line == "") continue;
        Vehicle vehi;
        vehi.fromLine(line);
        insertEnd(vehi);
    }
    in.close();
}
