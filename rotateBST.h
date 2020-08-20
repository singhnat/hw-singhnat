//
// Created by Natasha Singh on 8/15/20.
//

#ifndef ROTATEBST_H
#define ROTATEBST_H

#include "bst.h"

template <typename Key, typename Value>
class rotateBST : public BinarySearchTree <Key, Value> {

    public:
        rotateBST();
        ~rotateBST();
        bool sameKeys(const rotateBST& t2);
        void transform(rotateBST& t2);

    protected:
        void leftRotate(Node<Key, Value>* r);
        void rightRotate(Node <Key, Value>* r);


    private:
        bool checkEqual(const rotateBST& t2, Node<Key, Value>* r);
        bool isEqual(const rotateBST<Key,Value>& t2, Node<Key, Value>* r);
        void transformRightRecursive(rotateBST& t2, Node<Key, Value>* r);
        void transformLeftRecursive(rotateBST<Key,Value>& t2, Node<Key, Value>* r1, Node<Key, Value>* r2);
        void transformRightRecursive(rotateBST<Key,Value>& t2, Node<Key, Value>* r1, Node<Key, Value>* r2);


};

/**
* Constructor
*/
template<typename Key, typename Value>
rotateBST<Key,Value>::rotateBST() {
    BinarySearchTree<Key, Value>();
}

/**
* Deconstructor
*/
template<typename Key, typename Value>
rotateBST<Key,Value>::~rotateBST() {
    this->clear();
}

/**
* Rotates the tree to the right from a root Node.
*/
template<typename Key, typename Value>
void rotateBST<Key, Value>::rightRotate(Node<Key, Value>* r) {
    // If the left side is empty, no rotation
    if (r->getLeft() == NULL) {
        return;
    }

    // Finding the three key players in the rotation
    auto leftChild = r->getLeft();
    auto leftRightChild = leftChild->getRight();
    auto parent = r->getParent();

    // Changing the Pointers accordingly
    r->setLeft(leftRightChild);         // leftRightChild becomes the child of the root instead of leftChild
    r->setParent(leftChild);            // leftChild becomes the parent of the root instead of being the left child
    leftChild->setRight(r);             // root becomes the right child of the leftChild
    leftChild->setParent(parent);       // root's parent becomes the parent of the leftChild instead of root

    // Changing the main root if necessary
    if (r == this->mRoot) {
        this->mRoot = leftChild;
    }
    // If the leftRightChild exists, set its parent to the root
    if (leftRightChild != NULL) {
        leftRightChild->setParent(r);
    }
    // If the root has a parent, then set the parent's new left or right child
    if (parent != NULL){
        if (parent->getLeft() == r) {
            parent->setLeft(leftChild);
        } else {
            parent->setRight(leftChild);
        }
    }


}

/**
* Rotates the tree to the left from a root Node.
*/
template<typename Key, typename Value>
void rotateBST<Key, Value>::leftRotate(Node<Key, Value>* r) {
    // If the right side is empty, no rotation
    if (r->getRight() == NULL) {
        return;
    }

    // Finding the three key players in the rotation
    auto rightChild = r->getRight();
    auto rightLeftChild = rightChild->getLeft();
    auto parent = r->getParent();

    // Changing the Pointers accordingly
    r->setRight(rightLeftChild);        // rightLeftChild becomes the right child of root, replacing rightChild
    r->setParent(rightChild);           // rightChild becomes the parent of root, replacing parent
    rightChild->setLeft(r);             // root becomes the left child of rightChild, replacing rightLeftChild
    rightChild->setParent(parent);      // parent becomes the parent of rightChild, replacing root

    // Changing the main root if necessary
    if (r == this->mRoot) {
        this->mRoot = rightChild;
    }

    // If the rightLeftChild exists, set its parent to the root
    if (rightLeftChild != NULL) {
        rightLeftChild->setParent(r);
    }
    // If the root has a parent, then set the parent's new left or right child
    if (parent != NULL){
        if (parent->getLeft() == r) {
            parent->setLeft(rightChild);
        } else {
            parent->setRight(rightChild);
        }
    }

}

/**
* Checks if two Rotate Binary Search Trees have an identical set of keys.
 * NOTE: Possibly could be more efficient
*/
template<typename Key, typename Value>
bool rotateBST<Key,Value>::sameKeys(const rotateBST<Key,Value>& t2) {
    return (checkEqual(t2, this->mRoot) && checkEqual(*this, t2.mRoot));
}

/**
* A recursive helper function for sameKeys that goes through the tree to every Node and tries to find it in
 * the other tree
*/
template<typename Key, typename Value>
bool rotateBST<Key,Value>::checkEqual(const rotateBST<Key,Value>& t2, Node<Key, Value>* r) {
    // Need to check right tree and left tree
    bool rightCheck;
    bool leftCheck;

    // If there is a left subtree, keep following it
    if (r->getLeft() != NULL) {
        leftCheck = checkEqual(t2, r->getLeft());
    } else {
        leftCheck = true;  // Means we have reached a lead node on the left side
    }

    // If there is a right subtree, keep following it
    if (r->getRight() != NULL) {
        rightCheck = checkEqual(t2, r->getRight());
    } else {
        rightCheck = true; // Means we have reached a leaf node on the right side
    }

    // Checks that for the value in the other tree, accounting for the left and right subtrees as well
    // Makes sure that if the result is false for either right or left subtree, the entire result is false
    return (isEqual(t2, r) && rightCheck && leftCheck);
}

/**
* A helper function for isEqual that takes a Node and checks specifically if it is in the other tree, returning
 * true or false
*/
template<typename Key, typename Value>
bool rotateBST<Key,Value>::isEqual(const rotateBST<Key,Value>& t2, Node<Key, Value>* r) {
    // Finds the value in the other tree
    auto find = t2.internalFind(r->getKey());
    if (find == NULL){
        return false;
    }
    return true;
}

/**
* Takes a rotateBST and transforms it to match the current rotateBST using rotations
*/
template<typename Key, typename Value>
void rotateBST<Key,Value>::transform(rotateBST<Key,Value>& t2){
    // Exits function if the keys are not the same
    if (!this->sameKeys(t2)) {
        return;
    }

    transformRightRecursive(t2, t2.mRoot);              // First converts given tree to a linked list
    transformLeftRecursive(t2, this->mRoot,t2.mRoot);   // Then recursively converts into this tree

}

/**
* Takes a rotateBST and transforms it into a linked list
*/
template<typename Key, typename Value>
void rotateBST<Key,Value>::transformRightRecursive(rotateBST<Key,Value>& t2, Node<Key, Value>* r){
    // Keep rotating to the left until there is no left children
    while (r->getLeft() != NULL) {
        t2.rightRotate(r);
        r = r->getParent();
    }

    // If there are no more right Nodes to rotate, exit the function
    if (r->getRight() == NULL) {
        return;
    } else {
        transformRightRecursive(t2, r->getRight());
    }
}

/**
* A helper function to transform() that takes a rotateBST and rotates it to the right recursively
*/
template<typename Key, typename Value>
void rotateBST<Key,Value>::transformRightRecursive(rotateBST<Key,Value>& t2, Node<Key, Value>* r1, Node<Key, Value>* r2){
    // If the two Nodes do not equal, rotate the tree to the right until they do
    while (r1->getKey() != r2->getKey()) {
        t2.rightRotate(r2);
        r2 = r2->getParent();
    }

    // Check if there is a right child to r2
    if (r2->getRight() != NULL) {
        // First check if those keys equal, otherwise move recursively on the left side to transform
        if (r2->getRight()->getKey() != r1->getRight()->getKey() || r2->getRight()->getRight() != NULL)
            transformLeftRecursive(t2, r1->getRight(), r2->getRight());
    }

    // Check if there is a left child to r2
    if (r2->getLeft() != NULL) {
        // First check if those keys equal, otherwise move recursively on the right side to transform
        if (r2->getLeft()->getKey() != r1->getLeft()->getKey() || r2->getLeft()->getLeft() != NULL){
            transformRightRecursive(t2, r1->getLeft(), r2->getLeft()) ;
        }
    }
}

/**
* A helper function to transform() that takes a rotateBST and rotates it to the left recursively
*/
template<typename Key, typename Value>
void rotateBST<Key,Value>::transformLeftRecursive(rotateBST<Key,Value>& t2, Node<Key, Value>* r1, Node<Key, Value>* r2){

    // If the two Nodes do not equal, rotate the tree to the left until they do
    while (r1->getKey() != r2->getKey()) {
        t2.leftRotate(r2);
        r2 = r2->getParent();
    }

    // Check if there is right child to r2
    if (r2->getRight() != NULL) {
        // First check if those keys equal, otherwise move recursively on the left side to transform
        if (r2->getRight()->getKey() != r1->getRight()->getKey()|| r2->getRight()->getRight() != NULL)
            transformLeftRecursive(t2, r1->getRight(), r2->getRight());
    }

    // Check if there is a left child to r2
    if (r2->getLeft() != NULL) {
        // First check if those keys equal, otherwise move recursively on the right side to transform
        if (r2->getLeft()->getKey() != r1->getLeft()->getKey() || r2->getLeft()->getLeft() != NULL){
            transformRightRecursive(t2, r1->getLeft(), r2->getLeft());
        }
    }

}


#endif //BINARYSEARCHTREES_ROTATEBST_H
