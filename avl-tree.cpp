#include "avl-tree.h";
#include <cmath>
#include <stack>
#include <iostream>

using namespace std;

typedef BinarySearchTree::DataType DataType;
typedef BinarySearchTree::Node Node;

// check height of the node
int AVLTree::depthBelow(Node *n)
{
    if (n == NULL) return 0;
    return std::max(depthBelow(n->left), depthBelow(n->right)) + 1;
}

bool AVLTree::singleLeftRotation(Node *parentNode, bool isLeftChild)
{
    //Case 1: Unbalanced node is root
    if (!isLeftChild && parentNode == NULL) {
        Node* unbalancedNode = getRootNode();

        *getRootNodeAddress() = getRootNode()->right; //change address of root to new root

        // SPECIAL CASE TO CONSIDER
        // This new root might have a left child (let's call it temp).
        // We need to store it since we will later alter the child with getRootNode()->left = unbalancedNode;
        // If it does not have a left child, this value will be NULL
        Node* transferredTree = getRootNode()->left;

        getRootNode()->left = unbalancedNode; //Connect root to new right child
        unbalancedNode->right = transferredTree; //sever tie
    }
    //Case 2: Unbalanced node is left child of parent node
    else if (isLeftChild) {
        Node* childNode = parentNode->left; //initialize childNode, the unbalanced node

        parentNode->left = parentNode->left->right; //Connect parentNode to new left child
        Node* transferredTree = parentNode->left->left;
        parentNode->left->left = childNode; // Connect left child to new right child
        childNode->right = transferredTree; //sever tie
    }
    //Case 3: Unbalanced node is right child of parent node
    else {
        Node* childNode = parentNode->right; // initialize childNode, the unbalanced node

        parentNode->right = parentNode->right->right; //Connect parentNode to new right child
        Node* transferredTree = parentNode->right->left;
        parentNode->right->left = childNode; //Connect right child to new right child
        childNode->right = transferredTree; //sever tie
    }
}

bool AVLTree::singleRightRotation(Node *parentNode, bool isLeftChild)
{
    //Case 1: Unbalanced node is root
    if (!isLeftChild && parentNode == NULL) {
        Node* unbalancedNode = getRootNode(); //original root

        *getRootNodeAddress() = getRootNode()->left; //change address of root to new root
        Node* transferredTree = getRootNode()->right;
        getRootNode()->right = unbalancedNode; //Connect root to new right child
        unbalancedNode->left = transferredTree; //sever tie
    }
    //Case 2: Unbalanced node is left child of parent node
    else if (isLeftChild) {
        Node* childNode = parentNode->left; //initialize childNode, the unbalanced node

        parentNode->left = parentNode->left->left; //Connect parentNode to new left child
        Node* transferredTree = parentNode->left->right;
        parentNode->left->right = childNode; // Connect left child to new right child
        childNode->left = transferredTree; //sever tie
    }
    //Case 3: Unbalanced node is right child of parent node
    else {
        Node* childNode = parentNode->right; // initialize childNode, the unbalanced node

        parentNode->right = parentNode->right->left; //Connect parentNode to new right child
        Node* transferredTree = parentNode->right->right;
        parentNode->right->right = childNode; //Connect right child to new right child
        childNode->left = transferredTree; //sever tie
    }
}

bool AVLTree::leftRightRotation(Node *parentNode, bool isLeftChild)
{
    //Case 1: Unbalanced node is root
    if (!isLeftChild && parentNode == NULL) {

        // Convert to RR case by rotating left
        Node* rootChild = getRootNode()->left; //store rootChild

        getRootNode()->left = getRootNode()->left->right; //Connect root to new left child
        Node* transferredTree = getRootNode()->left->left;
        getRootNode()->left->left = rootChild; //Connect root's left child to new left child
        rootChild->right = transferredTree; //sever tie

        // Perform RR case
        singleRightRotation(parentNode, isLeftChild);
    }
    //Case 2: Unbalanced node is left child of parent node
    else if (isLeftChild) {

        //Convert to RR case by rotating left
        Node* childNode = parentNode->left; //initialize childNode, the unbalanced node
        Node* grandchildNode = parentNode->left->left; //initialize grandchildNode, the unbalanced node's child

        childNode->left = childNode->left->right; //connect childNode's left child to new left child
        Node* transferredTree = childNode->left->left;
        childNode->left->left = grandchildNode; //connect grandchild to new left child
        grandchildNode->right = transferredTree; //sever tie

        // Perform RR case
        singleRightRotation(parentNode, isLeftChild);
    }
    //Case 3: Unbalanced node is right child of parent node
    else {

        //Convert to RR case by rotating left
        Node* childNode = parentNode->right; //initialize childNode, the unbalanced node
        Node* grandchildNode = parentNode->right->left; //initialize grandchildNode, the unbalanced node's child

        childNode->left = childNode->left->right; //connect childNode's left child to new left child
        Node* transferredTree = childNode->left->left;
        childNode->left->left = grandchildNode; //connect grandchild to new left child
        grandchildNode->right = transferredTree; //sever tie

        //Perform RR case
        singleRightRotation(parentNode, isLeftChild);
    }
}

bool AVLTree::rightLeftRotation(Node *parentNode, bool isLeftChild)
{
    //Case 1: Unbalanced node is root
    if (!isLeftChild && parentNode == NULL) {

        // Convert to LL case by rotating right
        Node* rootChild = getRootNode()->right; //store rootChild

        getRootNode()->right = getRootNode()->right->left; //Connect root to new right child
        Node* transferredTree = getRootNode()->right->right;
        getRootNode()->right->right = rootChild; //Connect root's left child to new left child
        rootChild->left = transferredTree; //sever tie

        // Perform LL case
        singleLeftRotation(parentNode, isLeftChild);
    }
    //Case 2: Unbalanced node is left child of parent node
    else if (isLeftChild) {

        // Convert to LL case by rotating right
        Node* childNode = parentNode->left; //initialize childNode, the unbalanced node
        Node* grandchildNode = parentNode->left->right; //initialize grandchildNode, the unbalanced node's child

        childNode->right = childNode->right->left; //connect childNode's right child to new right child
        Node* transferredTree = childNode->right->right;
        childNode->right->right = grandchildNode; //connect grandchild to new left child
        grandchildNode->left = transferredTree; //sever tie

        // Perform LL case
        singleLeftRotation(parentNode, isLeftChild);
    }
    //Case 3: Unbalanced node is right child of parent node
    else {

        // Convert to LL case by rotating right
        Node* childNode = parentNode->right; //initialize childNode, the unbalanced node
        Node* grandchildNode = parentNode->right->right; //initialize grandchildNode, the unbalanced node's child

        childNode->right = childNode->right->left; //connect left child to new left child
        Node* transferredTree = childNode->right->right;
        childNode->right->right = grandchildNode;
        grandchildNode->left = transferredTree;

        // Perform LL case
        singleLeftRotation(parentNode, isLeftChild);
    }
}

std::stack<BinarySearchTree::Node*> *AVLTree::pathToNodeStack(DataType val)
{
    std::stack<BinarySearchTree::Node*>* stack = new std::stack<BinarySearchTree::Node*>;

    Node* currentNode = getRootNode();

    while(currentNode->val != val) {
        // Continue to search on left subtree
        if (val < currentNode->val) {
            stack->push(currentNode);
            currentNode = currentNode->left;
        }
        // Continue to search on right subtree
        else { // (val > currentNode->val)
            stack->push(currentNode);
            currentNode = currentNode->right;
        }
    }
    return stack;
}


bool AVLTree::updateNodeBalance(std::stack<BinarySearchTree::Node*> *pathToNode, DataType val)
{
    // Initialize balance factor
    int balanceFactor = 0;

    while(!pathToNode->empty()) {
        // Find depth of left and right subtree
        int depthLeft = depthBelow(pathToNode->top()->left);
        int depthRight = depthBelow(pathToNode->top()->right);

        balanceFactor = depthLeft - depthRight;

        // If more than 1, the current node is unbalanced
        // We are either in a RR or LR case
        if (balanceFactor > 1) {
            //Find balance factor of left subtree
            int depthLeftOfLeftSubTree = depthBelow(pathToNode->top()->left->left);
            int depthRightOfLeftSubTree = depthBelow(pathToNode->top()->left->right);
            int balanceFactorSubTree = depthLeftOfLeftSubTree - depthRightOfLeftSubTree;

            // Parameters for rotation functions
            Node* unbalancedNodeParent = findParent(pathToNode->top()->val);
            bool isLeftChild;

            //Initializing isLeftChild accordingly
            if (unbalancedNodeParent == NULL)                         isLeftChild = false; //unbalancedNode is the root
            else if (unbalancedNodeParent->left == pathToNode->top()) isLeftChild = true;
            else                                                      isLeftChild = false;

            //Check sign of balance factor of left subtree
            if (balanceFactorSubTree < 0) { // opposite sign: perform LR rotation
                leftRightRotation(unbalancedNodeParent, isLeftChild);
            }
            else { //same sign or 0: perform RR rotation
                singleRightRotation(unbalancedNodeParent, isLeftChild);
            }
        }

        // If less than -1, the current node is unbalanced
        // We are either in a LL or RL case
        if (balanceFactor < -1) {
            int depthLeftOfRightSubTree = depthBelow(pathToNode->top()->right->left);
            int depthRightOfRightSubTree = depthBelow(pathToNode->top()->right->right);
            int balanceFactorSubTree = depthLeftOfRightSubTree - depthRightOfRightSubTree;

            // Parameters for rotation functions
            Node* unbalancedNodeParent = findParent(pathToNode->top()->val);
            bool isLeftChild;

            //Initializing isLeftChild accordingly
            if (unbalancedNodeParent == NULL)                         isLeftChild = false; //unbalancedNode is the root
            else if (unbalancedNodeParent->left == pathToNode->top()) isLeftChild = true;
            else                                                      isLeftChild = false;

            if (balanceFactorSubTree > 0) { //opposite sign: perform RL rotation
                rightLeftRotation(unbalancedNodeParent, isLeftChild);
            }
            else { //same sign or 0: perform LL rotation
                singleLeftRotation(unbalancedNodeParent, isLeftChild);
            }
        }

        //Pop from the stack
        pathToNode->pop();
    }
    return true;

    // General steps
    // Find balance factor of current node
    // Function checks difference, pops if needed, and calls appropriate balancing function
}

bool AVLTree::insert(DataType val)
{
    BinarySearchTree::insert(val);
    return updateNodeBalance(pathToNodeStack(val), val);
}

bool AVLTree::remove(DataType val)
{
    std::stack<BinarySearchTree::Node*> *st = pathToNodeStack(val);
    BinarySearchTree::remove(val);
    return updateNodeBalance(st, val);

    //Node* parent = findParent(val);
    //BinarySearchTree::remove(val);
//
    //std::stack<BinarySearchTree::Node*> *st = pathToNodeStack(parent->val);
    //st->push(parent); //Add parent to stack as the parent could be the unbalanced node

    //return updateNodeBalance(st, parent->val);
    //return updateNodeBalance(pathToNodeStack(val), val);
}