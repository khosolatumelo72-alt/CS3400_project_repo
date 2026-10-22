#include "Tree.h"
#include <iostream>
using namespace std;

TreeNode::TreeNode(const Vehicle& v) : data(v), left(NULL), right(NULL) {}

VehicleTree::VehicleTree() : root(NULL), count(0) {}

VehicleTree::~VehicleTree() {
    destroy(root);
}

// Postorder delete: children first, then the node itself.
void VehicleTree::destroy(TreeNode* node) {
    if (node == NULL) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

// ---------- INSERT ----------
TreeNode* VehicleTree::insertRec(TreeNode* node, const Vehicle& v, bool& inserted) {
    if (node == NULL) {                 // found the empty spot
        inserted = true;
        return new TreeNode(v);
    }
    if (v.getID() < node->data.getID())
        node->left = insertRec(node->left, v, inserted);
    else if (v.getID() > node->data.getID())
        node->right = insertRec(node->right, v, inserted);
    else
        inserted = false;               // duplicate ID
    return node;
}

bool VehicleTree::insert(const Vehicle& v) {
    bool inserted = false;
    root = insertRec(root, v, inserted);
    if (inserted) count++;
    return inserted;
}

// ---------- SEARCH ----------
TreeNode* VehicleTree::searchRec(TreeNode* node, const string& id) const {
    if (node == NULL || node->data.getID() == id)
        return node;
    if (id < node->data.getID())
        return searchRec(node->left, id);
    return searchRec(node->right, id);
}

Vehicle* VehicleTree::search(const string& id) const {
    TreeNode* found = searchRec(root, id);
    if (found == NULL) return NULL;
    return &found->data;
}

// ---------- TRAVERSALS ----------
void VehicleTree::inorderRec(TreeNode* node) const {
    if (node == NULL) return;
    inorderRec(node->left);
    node->data.display();
    inorderRec(node->right);
}

void VehicleTree::preorderRec(TreeNode* node) const {
    if (node == NULL) return;
    node->data.display();
    preorderRec(node->left);
    preorderRec(node->right);
}

void VehicleTree::postorderRec(TreeNode* node) const {
    if (node == NULL) return;
    postorderRec(node->left);
    postorderRec(node->right);
    node->data.display();
}

void VehicleTree::inorder() const   { inorderRec(root); }
void VehicleTree::preorder() const  { preorderRec(root); }
void VehicleTree::postorder() const { postorderRec(root); }

// Level-order: visit row by row using a simple array-based queue.
void VehicleTree::levelOrder() const {
    if (root == NULL) {
        cout << "Tree is empty." << endl;
        return;
    }
    TreeNode** queue = new TreeNode*[count];   // each node is enqueued once
    int front = 0, rear = 0;
    queue[rear++] = root;
    while (front < rear) {
        TreeNode* cur = queue[front++];
        cur->data.display();
        if (cur->left  != NULL) queue[rear++] = cur->left;
        if (cur->right != NULL) queue[rear++] = cur->right;
    }
    delete[] queue;
}

// ---------- REMOVE ----------
TreeNode* VehicleTree::findMin(TreeNode* node) const {
    while (node->left != NULL)
        node = node->left;
    return node;
}

TreeNode* VehicleTree::removeRec(TreeNode* node, const string& id, bool& removed) {
    if (node == NULL) return NULL;              // ID not in tree

    if (id < node->data.getID()) {
        node->left = removeRec(node->left, id, removed);
    } else if (id > node->data.getID()) {
        node->right = removeRec(node->right, id, removed);
    } else {                                    // this is the node to delete
        removed = true;
        if (node->left == NULL) {               // 0 or 1 child (right)
            TreeNode* child = node->right;
            delete node;
            return child;
        }
        if (node->right == NULL) {              // 1 child (left)
            TreeNode* child = node->left;
            delete node;
            return child;
        }
        // 2 children: copy inorder successor here, then delete the successor
        TreeNode* successor = findMin(node->right);
        node->data = successor->data;
        bool dummy = false;
        node->right = removeRec(node->right, successor->data.getID(), dummy);
    }
    return node;
}

bool VehicleTree::remove(const string& id) {
    bool removed = false;
    root = removeRec(root, id, removed);
    if (removed) count--;
    return removed;
}

// ---------- INFO ----------
int VehicleTree::heightRec(TreeNode* node) const {
    if (node == NULL) return 0;
    int l = heightRec(node->left);
    int r = heightRec(node->right);
    return (l > r ? l : r) + 1;
}

int  VehicleTree::height() const  { return heightRec(root); }
int  VehicleTree::size() const    { return count; }
bool VehicleTree::isEmpty() const { return root == NULL; }
