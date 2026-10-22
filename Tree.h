#ifndef TREE_H
#define TREE_H
#include "Vehicle.h"
#include <string>
using namespace std;

// One node of the tree: holds a Vehicle and two child pointers.
class TreeNode {
public:
    Vehicle data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(const Vehicle& v);
};

// Binary Search Tree of Vehicles, ordered by Vehicle ID.
class VehicleTree {
private:
    TreeNode* root;
    int count;

    // recursive helpers - the public functions call these
    TreeNode* insertRec(TreeNode* node, const Vehicle& v, bool& inserted);
    TreeNode* searchRec(TreeNode* node, const string& id) const;
    TreeNode* removeRec(TreeNode* node, const string& id, bool& removed);
    TreeNode* findMin(TreeNode* node) const;
    int  heightRec(TreeNode* node) const;
    void inorderRec(TreeNode* node) const;
    void preorderRec(TreeNode* node) const;
    void postorderRec(TreeNode* node) const;
    void destroy(TreeNode* node);

public:
    VehicleTree();
    ~VehicleTree();
    VehicleTree(const VehicleTree&) = delete;             // copying would double-delete nodes
    VehicleTree& operator=(const VehicleTree&) = delete;

    bool     insert(const Vehicle& v);          // false if ID already exists
    Vehicle* search(const string& id) const;    // NULL if not found
    bool     remove(const string& id);          // false if ID not found

    void inorder() const;
    void preorder() const;
    void postorder() const;
    void levelOrder() const;

    int  height() const;
    int  size() const;
    bool isEmpty() const;
};
#endif
