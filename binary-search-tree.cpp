#include "binary-search-tree.h"
#include <iostream>
#include <queue>

typedef BinarySearchTree::DataType DataType;
typedef BinarySearchTree::Node Node;

using namespace std;

BinarySearchTree::Node::Node(DataType newval)
{
    val = newval;
    left = NULL;
    right = NULL;
}

int BinarySearchTree::getNodeDepth(Node* n) const
{
    
}

BinarySearchTree::BinarySearchTree()
{
    root_ = NULL;
    size_ = 0;
}

BinarySearchTree::~BinarySearchTree()
{
    removeNode(root_);
}

void BinarySearchTree::removeNode(Node* node) {
    if (node == NULL) return; // base casee
    removeNode(node->left);
    removeNode(node->right);
    node = NULL;
    delete node;
}

unsigned int BinarySearchTree::size() const
{
    return size_;
}

DataType BinarySearchTree::max() const
{
    Node *currentNode = root_;
    Node *previousNode = NULL;

    while (currentNode != NULL) {
        previousNode = currentNode;
        currentNode = currentNode->right;
    }
    return previousNode->val;
}

DataType BinarySearchTree::min() const
{
    Node *currentNode = root_;
    Node *previousNode = NULL;

    while (currentNode != NULL) {
        previousNode = currentNode;
        currentNode = currentNode->left;
    }
    return previousNode->val;
}

int BinarySearchTree::depthHelper(Node* node) const
{
    if (node == NULL) return -1;
    return std::max(depthHelper(node->left), depthHelper(node->right)) + 1;
}

unsigned int BinarySearchTree::depth() const
{
    return depthHelper(root_);
}

void BinarySearchTree::print() const
{

}

bool BinarySearchTree::exists(DataType val) const
{
    if (size_ == 0) return false;

    Node* currentNode = root_;

    while(currentNode != NULL) {
        if (val < currentNode->val)      currentNode = currentNode->left;
        else if (val > currentNode->val) currentNode = currentNode->right;
        else                             return true; // val == currentNode->val
    }
    return false; // currentNode is now NULL and val was never found
}

Node* BinarySearchTree::getRootNode()
{
    return root_;
}

Node** BinarySearchTree::getRootNodeAddress()
{
    return &root_;
}

bool BinarySearchTree::insert(DataType val)
{
    if (exists(val)) return false;

    //Create new node
    //Has to be Node not Node* because Node() doesn't return an address
    Node* newNode = new Node(val);

    if (size_ == 0) root_ = newNode;
    else { // Must traverse tree to find place for node

        Node *currentNode = root_;
        Node *previousNode = NULL;

        while (currentNode != NULL) {

            // Continue to perform insert operation on left subtree
            if (val < currentNode->val) {
                previousNode = currentNode;
                currentNode = currentNode->left;
            }
            // Continue to perform insert operation on right subtree
            else { // (val > currentNode->val)
                previousNode = currentNode;
                currentNode = currentNode->right;
            }
        }

        // Insert as left or right child of previousNode
        if (val < previousNode->val) previousNode->left = newNode;
        else                         previousNode->right = newNode;

    }

    size_++;
    return true;
}

// Returns parent of a val. If val is not found, returns a NULL pointer
Node* BinarySearchTree::findParent(DataType val) {

    Node* currentNode = root_;
    Node* parentNode = NULL;

    if (!exists(val)) return parentNode;

    while (currentNode->val != val) {

        // Continue to search on left subtree
        if (val < currentNode->val) {
            parentNode = currentNode;
            currentNode = currentNode->left;
        }
            // Continue to search on right subtree
        else { // (val > currentNode->val)
            parentNode = currentNode;
            currentNode = currentNode->right;
        }
    }
    return parentNode;
}

Node* findPredecessor(Node* ptr) {

    // ptr can be thought of as previousNode
    Node* ptrNext = ptr->left; // i.e. currentNode

    while (ptrNext != NULL) {
        ptr = ptrNext;
        ptrNext = ptrNext->right;
    }
    return ptr;
}

bool BinarySearchTree::remove(DataType val)
{
    if (!exists(val)) return false;

    Node* currentNode = root_;
    Node* previousNode = NULL;

    // Find node at which the val exists
    while (currentNode->val != val) {

        // Continue to search on left subtree
        if (val < currentNode->val) {
            previousNode = currentNode;
            currentNode = currentNode->left;
        }
        // Continue to search on right subtree
        else { // (val > currentNode->val)
            previousNode = currentNode;
            currentNode = currentNode->right;
        }
    }

    // CurrentNode is a leaf node

    if (currentNode->left == NULL && currentNode->right == NULL) {

        if (previousNode != NULL) {
            // currentNode is left child of previousNode
            if (previousNode->left == currentNode)  previousNode->left = NULL;
            // currentNode is right child of previousNode
            if (previousNode->right == currentNode) previousNode->right = NULL;
        }
        // Special case of root_
        if (currentNode == root_) root_ = NULL;

        delete currentNode;
    }
    // CurrentNode has one child node
    else if (currentNode->left == NULL ^ currentNode->right == NULL) {

        //Child node on the right
        if (currentNode->left == NULL && currentNode->right != NULL) {

            // Check if there are grandchildren
            // No grandchildren - do what was done before
            if (currentNode->right->left == NULL && currentNode->right->right == NULL) {
                currentNode->val = currentNode->right->val; //Copy contents in child into currentNode
                delete currentNode->right; // Delete child
                //Delete connection between currentNode and child
                currentNode->right = NULL;
            }
            // Only one grandchild, who is on the right
            else if (currentNode->right->left == NULL && currentNode->right->right != NULL) {
                Node* childNode = currentNode->right;

                currentNode->val = childNode->val; //Copy contents in child into currentNode
                currentNode->right = childNode->right; //connect to grandchild
                childNode->right = NULL; // sever tie with grandchild
                delete childNode;
            }
            //Only one grandchild, who is on the left
            else if (currentNode->right->right == NULL && currentNode->right->left != NULL) {
                Node* childNode = currentNode->right;

                currentNode->val = childNode->val; //Copy contents into currentNode
                currentNode->right = childNode->left; //Connect to grandchild
                childNode->left = NULL;
                delete childNode;
            }
            // Two grandchildren
            // Do not look for predecessor here, as there is no left subtree
            else {
                Node* parentNode = findParent(currentNode->val);

                //currentNode is parentNode's left child
                if (parentNode->left == currentNode) {
                    parentNode->left = currentNode->right;
                    currentNode->right = NULL;
                    delete currentNode;
                }
                else { //currentNode is parentNode's right child
                    parentNode->right = currentNode->right;
                    currentNode->right = NULL;
                    delete currentNode;
                }
            }
        }
        //Child node on the left
        else { //(currentNode->right == NULL)

            // Check if there are grandchildren
            // No grandchildren - do what was done before
            if (currentNode->left->left == NULL && currentNode->left->right == NULL) {
                currentNode->val = currentNode->left->val; //Copy contents in child into currentNode
                delete currentNode->left; // Delete child
                currentNode->left = NULL;
            }
            // Only one grandchild, who is on the right
            else if (currentNode->left->left == NULL && currentNode->left->right != NULL) {
                Node* childNode = currentNode->left;

                currentNode->val = childNode->val; //Copy contents in child into currentNode
                currentNode->left = childNode->right; //connect to grandchild
                childNode->right = NULL; // sever tie with grandchild
                delete childNode;
            }
            //Only one grandchild, who is on the left
            else if (currentNode->left->right == NULL && currentNode->left->left != NULL) {
                Node* childNode = currentNode->left;

                currentNode->val = childNode->val; //Copy contents into currentNode
                currentNode->left = childNode->left; //Connect to grandchild
                childNode->left = NULL;
                delete childNode;
            }
            // Two grandchildren
            else {
                Node* predecessor = findPredecessor(currentNode);
                Node* predecessorParent = findParent(predecessor->val);

                currentNode->val = predecessor->val; //copy predecessor into currentNode
                predecessorParent->right = NULL;
                delete predecessor;
            }
        }
    }
    // CurrentNode has two child nodes, meaning neither pointer is NULL
    else {
        Node* predecessor = findPredecessor(currentNode);

        // If no parent, i.e. value does not exist or the node that you want to find the
        // parent for is the root_, NULL will be returned. But we know that predecessor->val
        // will always exist, and a predecessor will never be a root
        Node* predecessorParent = findParent(predecessor->val);

        //Copy contents of predecessor into currentNode
        currentNode->val = predecessor->val;

        // Remove connection between parent and predecessor
        // by connecting parent to predecessor's child
        if (predecessorParent->right == predecessor) predecessorParent->right = predecessor->left;
        else                                         predecessorParent->left = predecessor->left;

        //No need to make right NULL as predecessor does not have right child by definition
        predecessor->left = NULL;
        delete predecessor;
    }

    size_--;
    return true;
}