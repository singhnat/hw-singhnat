




#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <typeinfo>
#include "rotateBST.h"

/**
* A special kind of node for an AVL tree, which adds the height as a data member, plus 
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
	// Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int getHeight() const;
    void setHeight(int height);

    // Getters for parent, left, and right. These need to be redefined since they 
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int mHeight;
};

/*
--------------------------------------------
Begin implementations for the AVLNode class.
--------------------------------------------
*/

/**
* Constructor for an AVLNode. Nodes are initialized with a height of 0.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent)
    : Node<Key, Value>(key, value, parent)
    , mHeight(0)
{

}

/**
* Destructor.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* Getter function for the height. 
*/
template<typename Key, typename Value>
int AVLNode<Key, Value>::getHeight() const
{
    return mHeight;
}

/**
* Setter function for the height. 
*/
template<typename Key, typename Value>
void AVLNode<Key, Value>::setHeight(int height)
{
    mHeight = height;
}

/**
* Getter function for the parent. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key,Value>*>(this->mParent);
}

/**
* Getter function for the left child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key,Value>*>(this->mLeft);
}

/**
* Getter function for the right child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key,Value>*>(this->mRight);
}

/*
------------------------------------------
End implementations for the AVLNode class.
------------------------------------------
*/

/**
* A templated balanced binary search tree implemented as an AVL tree.
*/
template <class Key, class Value>
class AVLTree : public rotateBST<Key, Value>
{
public:
	// Methods for inserting/removing elements from the tree. You must implement
	// both of these methods. 
    virtual void insert(const std::pair<Key, Value>& keyValuePair) override;
    void remove(const Key& key) override;

private:
    void insertItem(const std::pair<Key, Value>& keyValuePair, AVLNode<Key, Value>* root);
    void balance();
    AVLNode<Key, Value>* findPivot(AVLNode<Key, Value>* root);
    void printHeights(AVLNode<Key, Value>* root);
    void adjustHeights(AVLNode<Key, Value>* root);
    void removeTwoChildren(AVLNode<Key, Value>* root);
    void removeOneChild(AVLNode<Key, Value>* root);
    void removeZeroChildren(AVLNode<Key, Value>* root);
    AVLNode<Key, Value>* getLargestNode(AVLNode<Key, Value>* root) const;

};

/*
--------------------------------------------'
Begin implementations for the AVLTree class.
--------------------------------------------
*/

/**
* Insert function for a key value pair. Finds location to insert the node and then balances the tree. 
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
    // Checks this is the first entry
    if (this->mRoot == NULL) {
        this->mRoot = new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, NULL); // Create a new AVL Node
        dynamic_cast<AVLNode<Key,Value>*>(this->mRoot)->setHeight(1);
    } else {
        insertItem(keyValuePair, dynamic_cast<AVLNode<Key,Value>*>(this->mRoot));
    }

    // Adjusting the heights of each node
    this->adjustHeights(dynamic_cast<AVLNode<Key,Value>*>(this->mRoot));

    // Checking if the tree is balanced after the addition
    if (!this->isBalanced()) {
        this->balance();
    }

}

/**
* A helper function for insert that runs on recursion with the key value pair. Chooses right location to insert the node
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::insertItem(const std::pair<Key, Value>& keyValuePair, AVLNode<Key, Value>* root) {
    auto rootKey = (root->getKey());
    auto itemKey = keyValuePair.first;

    // If the root is greater than the new item, item goes to the left side
    if (rootKey > itemKey) {
        if (root->getLeft() == NULL) { // If the left side is empty, create a new Node and place the item
            root->setLeft(new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, root));
            root->getLeft()->setHeight(1);
        } else { // If the left child is occupied, keep moving down the left side
            insertItem(keyValuePair, root->getLeft());
        }
    // If the root is less than the new item, tem goes to the right side
    } else if (rootKey < itemKey) {
        if (root->getRight() == NULL) { // If the right side is empty, create a new Node and place the item
            root->setRight(new AVLNode<Key, Value>(keyValuePair.first, keyValuePair.second, root));
            root->getRight()->setHeight(1);
        } else { // If the right child is occupied, keep moving down the left side
            insertItem(keyValuePair, root->getRight());
        }
    // If the root equals the item, then overwrite the root's value
    } else {
        root->setValue(keyValuePair.second);
    }
}

/**
* A helper function that balances the tree
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::balance() {
    // Finding the first pivot of the tree
    auto pivot = this->findPivot(dynamic_cast<AVLNode<Key, Value> *>(this->mRoot));

    // Handling the four types of cases for rotations
    if (pivot->getRight() != NULL) {

        if (pivot->getRight()->getRight() != NULL) { // If the case is right right
            this->leftRotate(pivot);
        } else if (pivot->getRight()->getLeft() != NULL) { // If the case is right left
            this->rightRotate(pivot->getRight());
            this->leftRotate(pivot->getRight());
        }

    } else if (pivot->getLeft() != NULL) {
        if (pivot->getLeft()->getLeft() != NULL) { // If the case is left left
            this->rightRotate(pivot);
        } else if (pivot->getLeft()->getRight() != NULL) { // If the case is left right
            this->leftRotate(pivot->getLeft());
            this->rightRotate(pivot);
        }
    }
}

/**
* A helper function that finds the pivot to balance the tree on
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::findPivot(AVLNode<Key, Value>* root) {
    int leftHeight;
    int rightHeight;
    // Checking of the left node is NULL
    if (root->getLeft() == NULL) {
        leftHeight = 0;
    } else {
        // Getting the height of the left side
        leftHeight = root->getLeft()->getHeight();
    }

    // Checking if the right node is NULL
    if (root->getRight() == NULL) {
        rightHeight = 0;
    } else {
        // Getting the height of the right side
        rightHeight = root->getRight()->getHeight();
    }

    // Creating variables
    AVLNode<Key, Value>* leftPivot;
    AVLNode<Key, Value>* rightPivot;

    // Checking if the Node has any left children
    if (leftHeight > 1) {
        // Checking if the left side has any pivots
        leftPivot = findPivot(root->getLeft()); // Recursive
        // Once we find a pivot, we just keep returning it
        if (leftPivot != NULL) {
            // If we find a pivot, we just keep returning it, not looking anymore
            return leftPivot;
        }
    }

    // Checking if the Node has any right children
    if (rightHeight > 1) {
        // Checking if the right side has any pivots
        rightPivot = findPivot(root->getRight()); // Recursive
        // If we find a pivot, we just keep returning it, not looking anymore
        if (rightPivot != NULL) {
            return rightPivot;
        }
    }

    // If the balance factor is less than or equal to one, there is no pivot
    if (abs(rightHeight-leftHeight) <= 1) {
        return NULL;
    } else {
        // Return this as the pivot if the balance factor is too big
        return root;
    }

}

/**
* A helper function that just prints the heights of each node, used for debugging purposes
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::printHeights(AVLNode<Key, Value>* root) {
        if (root->getLeft() != NULL) {
            printHeights(root->getLeft());
        }
        if (root->getRight() != NULL){
            printHeights(root->getRight());
        }
}

/**
* A recursive function that adjusts the heights of each node after a value has been removed or inserted
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::adjustHeights(AVLNode<Key, Value>* root) {
    // If the root is nonexistent, then just get out
    if (root == NULL) {
        return;
    }
    // Adjust the heights of the left and right side first
    adjustHeights(root->getLeft());
    adjustHeights(root->getRight());

    // Based on how many children the node has, we set the height of the node
    if (root->getRight() == NULL && root->getLeft() == NULL)  {
        root->setHeight(1); // If it is a leaf node, the height is one
    // If the node only has one child, add one to the child's height
    } else if (root->getRight() == NULL) {
        root->setHeight(root->getLeft()->getHeight() + 1);
    } else if (root->getLeft() == NULL) {
        root->setHeight(root->getRight()->getHeight() + 1);
    // If the node has two children, chose the max between their heights
    } else {
        root->setHeight(std::max(root->getRight()->getHeight(), root->getLeft()->getHeight()) + 1);
    }

}



/**
* Remove function for a given key. Finds the node, reattaches pointers, and then balances when finished. 
*/

template<typename Key, typename Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // Finding the Node for the corresponding key
    auto rootNode = dynamic_cast<AVLNode<Key,Value>*>(this->internalFind(key));
    if (rootNode == NULL) {
        return;
    } else if (rootNode->getRight() != NULL && rootNode->getLeft() != NULL) {
        removeTwoChildren(rootNode);
    } else if (rootNode->getRight() != NULL || rootNode->getLeft() != NULL) {
        removeOneChild(rootNode);
    } else {
        removeZeroChildren(rootNode);
    }

    // If the tree is unbalanced, balance it
    if (!this->isBalanced()) {
        this->balance();
    }
    // After the tree has been balanced, adjust the heights of the tree
    adjustHeights(dynamic_cast<AVLNode<Key,Value>*>(this->mRoot));

}

/**
* A helper function that removes a node if it has two children
 * Same as in Binary Search Tree but with AVLNode instead
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::removeTwoChildren(AVLNode<Key, Value>* root)
{
    // Finding the largest value in the left subtree
    auto largest = getLargestNode(root->getLeft());

    // Creating a new node to with the largest value in the right subtree
    auto newNode = new AVLNode<Key, Value>(largest->getKey(), largest->getValue(), root->getParent());
    // Changing the pointers to the children both ways
    newNode->setLeft(root->getLeft());
    newNode->setRight(root->getRight());
    newNode->getLeft()->setParent(newNode);
    newNode->getRight()->setParent(newNode);

    // Checks if the root has a parent or not
    if (root->getParent() == NULL) {
        this->mRoot = newNode;
    } else {
        // Checks which child of the parent this new node is going to be, either right or left
        if ((root->getParent())->getLeft() == root) {
            root->getParent()->setLeft(newNode);
        } else {
            root->getParent()->setRight(newNode);
        }
    }

    // Removes the largest value from wherever it was found
    if (largest->getLeft() != NULL) {
        removeOneChild(largest);
    } else {
        removeZeroChildren(largest);
    }

    delete root;

}

/**
* A helper function that removes a node if it has one child
 * Same as in Binary Search Tree but with AVLNode instead
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::removeOneChild(AVLNode<Key, Value>* root)
{
    // Checking which side the child is on
    if (root->getRight() != NULL) {
        auto right = root->getRight();
        auto parent = root->getParent();

        // Changing the right child's pointer to point to the parent of the original node
        right->setParent(parent);
        // Replacing the original node with it's right child by changing the parent's pointer
        if (parent->getRight() == root) {
            parent->setRight(right);
        } else {
            parent->setLeft(right);
        }

        // Checking if the node has a parent
        if (root->getParent() == NULL) {
            this->mRoot = right;
        }

        delete root;

    } else {
        auto left = root->getLeft();
        auto parent = root->getParent();

        // Changing the left child's pointer to point to the parent of the original node
        left->setParent(parent);
        // Replacing the original node with it's left child by changing the parent's pointer
        if (parent->getLeft() == root) {
            parent->setLeft(left);
        } else {
            parent->setRight(left);
        }

        // Checking if the node has a parent
        if (root->getParent() == NULL) {
            this->mRoot = left;
        }

        delete root;
    }
}

/**
* A helper function that removes a node if it has zero children
 * Same as in Binary Search Tree but with AVLNode instead
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::removeZeroChildren(AVLNode<Key, Value>* root)
{
    // If the node has no parent (i.e. only value), then set the base node to NULL
    if (root->getParent() == NULL) {
        this->mRoot = NULL;
    }

    // Checking what side the child is on for the parent, to set to NULL
    if (root->getParent()->getLeft() == root) {
        root->getParent()->setLeft(NULL);
    } else {
        root->getParent()->setRight(NULL);
    }

    delete root;

}

/**
* A helper function to removeTwoChildren that finds the largest Node from a starting Node
 * Same as in Binary Search Tree but with AVLNode instead
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::getLargestNode(AVLNode<Key, Value>* root) const
{
    // Keeps moving all the way down until it finds the right-most Node
    if (root->getRight() != NULL) {
        return getLargestNode(root->getRight());
    } else {
        return root;
    }
}
/*
------------------------------------------
End implementations for the AVLTree class.
------------------------------------------
*/



#endif
