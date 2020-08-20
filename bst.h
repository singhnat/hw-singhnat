




#ifndef BST_H
#define BST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <utility>

/**
* A templated class for a Node in a search tree. The getters for parent/left/right are virtual so that they
* can be overridden for future kinds of search trees, such as Red Black trees, Splay trees, and AVL trees.
*/
template <typename Key, typename Value>
class Node
{
public:
    Node(const Key& key, const Value& value, Node<Key, Value>* parent);
    virtual ~Node();

    const std::pair<Key, Value>& getItem() const;
    std::pair<Key, Value>& getItem();
    const Key& getKey() const;
    const Value& getValue() const;
    Key& getKey();
    Value& getValue();

    virtual Node<Key, Value>* getParent() const;
    virtual Node<Key, Value>* getLeft() const;
    virtual Node<Key, Value>* getRight() const;

    void setParent(Node<Key, Value>* parent);
    void setLeft(Node<Key, Value>* left);
    void setRight(Node<Key, Value>* right);
    void setValue(const Value &value);

protected:
    std::pair<Key, Value> mItem;
    Node<Key, Value>* mParent;
    Node<Key, Value>* mLeft;
    Node<Key, Value>* mRight;
};

/*
	-----------------------------------------
	Begin implementations for the Node class.
	-----------------------------------------
*/

/**
* Explicit constructor for a node.
*/
template<typename Key, typename Value>
Node<Key, Value>::Node(const Key& key, const Value& value, Node<Key, Value>* parent)
        : mItem(key, value)
        , mParent(parent)
        , mLeft(NULL)
        , mRight(NULL)
{

}

/**
* Destructor, which does not need to do anything since the pointers inside of a node
* are only used as references to existing nodes. The nodes pointed to by parent/left/right
* are freed within the deleteAll() helper method in the BinarySearchTree.
*/
template<typename Key, typename Value>
Node<Key, Value>::~Node()
{

}

/**
* A const getter for the item.
*/
template<typename Key, typename Value>
const std::pair<Key, Value>& Node<Key, Value>::getItem() const
{
    return mItem;
}

/**
* A non-const getter for the item.
*/
template<typename Key, typename Value>
std::pair<Key, Value>& Node<Key, Value>::getItem()
{
    return mItem;
}

/**
* A const getter for the key.
*/
template<typename Key, typename Value>
const Key& Node<Key, Value>::getKey() const
{
    return mItem.first;
}

/**
* A const getter for the value.
*/
template<typename Key, typename Value>
const Value& Node<Key, Value>::getValue() const
{
    return mItem.second;
}

/**
* A non-const getter for the key.
*/
template<typename Key, typename Value>
Key& Node<Key, Value>::getKey()
{
    return mItem.first;
}

/**
* A non-const getter for the value.
*/
template<typename Key, typename Value>
Value& Node<Key, Value>::getValue()
{
    return mItem.second;
}

/**
* An implementation of the virtual function for retreiving the parent.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getParent() const
{
    return mParent;
}

/**
* An implementation of the virtual function for retreiving the left child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getLeft() const
{
    return mLeft;
}

/**
* An implementation of the virtual function for retreiving the right child.
*/
template<typename Key, typename Value>
Node<Key, Value>* Node<Key, Value>::getRight() const
{
    return mRight;
}

/**
* A setter for setting the parent of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setParent(Node<Key, Value>* parent)
{
    mParent = parent;
}

/**
* A setter for setting the left child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setLeft(Node<Key, Value>* left)
{
    mLeft = left;
}

/**
* A setter for setting the right child of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setRight(Node<Key, Value>* right)
{
    mRight = right;
}

/**
* A setter for the value of a node.
*/
template<typename Key, typename Value>
void Node<Key, Value>::setValue(const Value& value)
{
    mItem.second = value;
}

/*
	---------------------------------------
	End implementations for the Node class.
	---------------------------------------
*/

/**
* A templated unbalanced binary search tree.
*/
template <typename Key, typename Value>
class BinarySearchTree
{
public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    virtual void insert(const std::pair<Key, Value>& keyValuePair);
    virtual void remove(const Key& key);
    void clear();
    void print() const;
    bool isBalanced() const;

private:
    void insertItem(const std::pair<Key, Value>& keyValuePair, Node<Key,Value>* root);
    Node<Key, Value>* getLargestNode(Node<Key, Value>* root) const;
    void removeTwoChildren(Node<Key,Value>* root);
    void removeOneChild(Node<Key,Value>* root);
    void removeZeroChildren(Node<Key,Value>* root);
    Node<Key, Value>* insidefind(const Key& key, Node<Key, Value>* root) const;
    void deleteTree(Node<Key, Value>* root);
    int getHeight(Node<Key, Value>* root) const;
    bool balanceFactor(Node<Key, Value>* root) const;

public:
    /**
    * An internal iterator class for traversing the contents of the BST.
    */
    class iterator
    {
    public:
        iterator(Node<Key,Value>* ptr);
        iterator();

        std::pair<Key,Value>& operator*() const;
        std::pair<Key,Value>* operator->() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;
        iterator& operator=(const iterator& rhs);

        iterator& operator++();

    protected:
        Node<Key, Value>* mCurrent;

        friend class BinarySearchTree<Key, Value>;
    };

public:
    iterator begin() const;
    iterator end() const;
    iterator find(const Key& key) const;

protected:
    Node<Key, Value>* internalFind(const Key& key) const; //TODO
    Node<Key, Value>* getSmallestNode() const; //TODO
    void printRoot (Node<Key, Value>* root) const;

protected:
    Node<Key, Value>* mRoot;

public:
    void print() {this->printRoot(this->mRoot);}

};

/*
	---------------------------------------------------------------
	Begin implementations for the BinarySearchTree::iterator class.
	---------------------------------------------------------------
*/

/**
* Explicit constructor that initializes an iterator with a given node pointer.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::iterator::iterator(Node<Key,Value>* ptr)
        : mCurrent(ptr)
{

}

/**
* A default constructor that initializes the iterator to NULL.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::iterator::iterator()
        : mCurrent(NULL)
{

}

/**
* Provides access to the item.
*/
template<typename Key, typename Value>
std::pair<Key, Value>& BinarySearchTree<Key, Value>::iterator::operator*() const
{
    return mCurrent->getItem();
}

/**
* Provides access to the address of the item.
*/
template<typename Key, typename Value>
std::pair<Key, Value>* BinarySearchTree<Key, Value>::iterator::operator->() const
{
    return &(mCurrent->getItem());
}

/**
* Checks if 'this' iterator's internals have the same value
* as 'rhs'
*/
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::iterator::operator==(const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return this->mCurrent == rhs.mCurrent;
}

/**
* Checks if 'this' iterator's internals have a different value
* as 'rhs'
*/
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::iterator::operator!=(const BinarySearchTree<Key, Value>::iterator& rhs) const
{
    return this->mCurrent != rhs.mCurrent;
}

/**
* Sets one iterator equal to another iterator.
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator &BinarySearchTree<Key, Value>::iterator::operator=(const BinarySearchTree<Key, Value>::iterator& rhs)
{
    this->mCurrent = rhs.mCurrent;
    return *this;
}

/**
* Advances the iterator's location using an in-order traversal.
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator& BinarySearchTree<Key, Value>::iterator::operator++()
{
    if(mCurrent->getRight() != NULL)
    {
        mCurrent = mCurrent->getRight();
        while(mCurrent->getLeft() != NULL)
        {
            mCurrent = mCurrent->getLeft();
        }
    }
    else if(mCurrent->getRight() == NULL)
    {
        Node<Key, Value>* parent = mCurrent->getParent();
        while(parent != NULL && mCurrent == parent->getRight())
        {
            mCurrent = parent;
            parent = parent->getParent();
        }
        mCurrent = parent;
    }
    return *this;
}

/*
	-------------------------------------------------------------
	End implementations for the BinarySearchTree::iterator class.
	-------------------------------------------------------------
*/

/*
	-----------------------------------------------------
	Begin implementations for the BinarySearchTree class.
	-----------------------------------------------------
*/
/**
* Default constructor for a BinarySearchTree, which sets the root to NULL.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::BinarySearchTree()
{
	mRoot = NULL;
}

/**
* Deconstructor for a BinarySearchTree, which calls the clear function.
*/
template<typename Key, typename Value>
BinarySearchTree<Key, Value>::~BinarySearchTree()
{
	this->clear();
}

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::print() const
{
	printRoot(mRoot);
	std::cout << "\n";
}

/**
* Returns an iterator to the "smallest" item in the tree
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::begin() const
{
	BinarySearchTree<Key, Value>::iterator begin(getSmallestNode());
	return begin;
}

/**
* Returns an iterator whose value means INVALID
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::end() const
{
	BinarySearchTree<Key, Value>::iterator end(NULL);
	return end;
}

/**
* Returns an iterator to the item with the given key, k
* or the end iterator if k does not exist in the tree
*/
template<typename Key, typename Value>
typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::find(const Key& key) const
{
	Node<Key, Value>* curr = internalFind(key);
	BinarySearchTree<Key, Value>::iterator it(curr);
	return it;
}

/**
* An insert method to insert into a Binary Search Tree. The tree will not remain balanced when
* inserting.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
    // If this is the first Node, create the newNode
    if (mRoot == NULL) {
        mRoot = new Node<Key, Value>(keyValuePair.first, keyValuePair.second, NULL);
	} else {
        insertItem(keyValuePair, mRoot);
    }
}

/**
 * A helper method for inserting into a Binary Search Tree that uses recursion
 */
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::insertItem(const std::pair<Key, Value>& keyValuePair, Node<Key, Value>* root) {
    auto rootKey = (root->getKey());
    auto itemKey = keyValuePair.first;
    // If the root's Key is greater than the new key, then the new key goes to the left
    if (rootKey > itemKey) {
        // If the left Child is empty, insert it there. Otherwise keep moving down on the left.
        if (root->getLeft() == NULL) {
            root->setLeft(new Node<Key, Value>(keyValuePair.first, keyValuePair.second, root));
        } else {
            insertItem(keyValuePair, root->getLeft());
        }
    // If the root's Key is less than the new key, then the new key goes to the right
    } else if (rootKey < itemKey) {
        // If the Right Child is empty, insert it there. Otherwise keep moving down on the right.
        if (root->getRight() == NULL) {
            root->setRight(new Node<Key, Value>(keyValuePair.first, keyValuePair.second, root));
        } else {
            insertItem(keyValuePair, root->getRight());
        }
    // If the keys are the same, override the current value.
    } else {
        root->setValue(keyValuePair.second);
    }
}

/**
* An remove method to remove a specific key from a Binary Search Tree. The tree may not remain balanced after
* removal.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::remove(const Key& key)
{
    auto rootNode = internalFind(key);                                          // Find the Node in tree
    if (rootNode == NULL) {                                                     // If the Node doesn't exist, do nothing
        return;
    } else if (rootNode->getRight() != NULL && rootNode->getLeft() != NULL) {   // If the Node has Two Children
        removeTwoChildren(rootNode);
    } else if (rootNode->getRight() != NULL || rootNode->getLeft() != NULL) {   // If the Node has One Child
        removeOneChild(rootNode);
    } else {                                                                    // If the Node has Zero Children
        removeZeroChildren(rootNode);
    }

}

/**
* A helper method to remove a specific Node from a Binary Search Tree with two children.
*/

template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::removeTwoChildren(Node<Key, Value>* root)
{
    // Finding the largest value in the left subtree
    auto largest = getLargestNode(root->getLeft());

    // Creating a new node to with the largest value in the right subtree
    auto newNode = new Node<Key, Value>(largest->getKey(), largest->getValue(), root->getParent());
    // Changing the pointers to the children both ways
    newNode->setLeft(root->getLeft());                  // NewNode's left child is the root's left Child
    newNode->setRight(root->getRight());                // NewNode's right child is the root's tight Child
    newNode->getLeft()->setParent(newNode);             // NewNode is set as the parent of the root's left child
    newNode->getRight()->setParent(newNode);            // NewNode is set as the parent of the root's right child

    // Checks if the root has a parent or not
    if (root->getParent() == NULL) {
        mRoot = newNode;
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
* A helper method to remove a specific Node from a Binary Search Tree with one child.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::removeOneChild(Node<Key, Value>* root)
{
    // Check which side the child is on
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

        // Checking if the root node has a parent
        if (root->getParent() == NULL) {
            mRoot = right;
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
            mRoot = left;
        }

        delete root;
    }
}

/**
* A helper method to remove a specific Node from a Binary Search Tree with zero children.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::removeZeroChildren(Node<Key, Value>* root)
{
    // If the node has no parent (i.e. only value), then set the base node to NULL
    if (root->getParent() == NULL) {
        mRoot = NULL;
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
* A method to remove all contents of the tree and reset the values in the tree
* for use again.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::clear()
{
    deleteTree(mRoot);
    mRoot = NULL;
}

/**
* A recursive helper function used to delete the Nodes before before deleting the actual Node.
*/
template<typename Key, typename Value>
void BinarySearchTree<Key, Value>::deleteTree(Node<Key, Value>* root)
{
    if (root == NULL) {
        return;
    } else {
        deleteTree(root->getLeft());
        deleteTree(root->getRight());
    }
    delete root;
}



/**
* A helper function to find the largest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::getLargestNode(Node<Key, Value>* root) const
{
    // Keep going on the right side until you reach the rightmost Node aka the largest Node
    if (root->getRight() != NULL) {
        return getLargestNode(root->getRight());
    } else {
        return root;
    }
}

/**
* A helper function to find the smallest node in the tree.
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::getSmallestNode() const
{
    // Keep going on the left side until you reach the leftmost Node aka the smallest Node
    auto smallest = mRoot;
	while (smallest->getLeft() != NULL) {
	    smallest = smallest->getLeft();
	}
	return smallest;
}


/**
* Helper function to find a node with given key, k and
* return a pointer to it or NULL if no item with that key
* exists
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::internalFind(const Key& key) const
{
    return insidefind(key, mRoot);
}

/**
* Helper recursive method for internalFind
*/
template<typename Key, typename Value>
Node<Key, Value>* BinarySearchTree<Key, Value>::insidefind(const Key& key, Node<Key, Value>* root) const
{
    // If nothing is found return NULL
    if (root == NULL) {
        return NULL;
    }

    auto itemKey = key;
    auto rootKey = root->getKey();

    // Go on a certain side of the tree based on comparing the value to the root
    if (itemKey > rootKey) {
        return insidefind(key, root->getRight());
    } else if (itemKey < rootKey) {
        return insidefind(key, root->getLeft());
    } else {
        return root;
    }
}

/**
 * Return true iff the BST is an AVL Tree.
 */
template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::isBalanced() const
{
    return balanceFactor(mRoot);
}

/**
 * Returns true if the balance factor is less than or equal to 1, AKA is balanced
 */

template<typename Key, typename Value>
bool BinarySearchTree<Key, Value>::balanceFactor(Node<Key, Value>* root) const
{
    // If we reach a NULL node, return true
    if (root == NULL) {
        return true;
    }
    // First check the balance factor of the left side is false
    if (!balanceFactor(root->getLeft())){
        return false;
    }
    int leftHeight = getHeight(root->getLeft());        // Get the height of the left side
    // Check the balance factor of the right side first
    if (!balanceFactor(root->getRight())){
        return false;
    }
    int rightHeight = getHeight(root->getRight());      // Get the height of the right side
    return abs(rightHeight - leftHeight) <= 1;          // Calculating the balance factor and returning true/false

}

/**
 * Returns the height of a tree from the given root.
 */
template<typename Key, typename Value>
int BinarySearchTree<Key, Value>::getHeight(Node<Key, Value>* root) const
{
    // If we reach a NULL node, the height is 0
    if (root == NULL) {
        return 0;
    } else {
        int rightHeight = getHeight(root->getRight());      // Calling the function on right subtree
        int leftHeight = getHeight(root->getLeft());        // Calling the function on left subtree

        // Whichever height is larger, use that and add one to get the height of that Node
        if (leftHeight > rightHeight) {
            return leftHeight + 1;
        } else {
            return rightHeight + 1;
        }
    }
}



/**
 * Lastly, we are providing you with a print function, BinarySearchTree::printRoot().
   Just call it with a node to start printing at, e.g:
   this->printRoot(this->mRoot)
   It will print up to 5 levels of the tree rooted at the passed node, in ASCII graphics format.
   We hope it will make debugging easier!
  */

// include print function (in its own file because it's fairly long)
#include "print_bst.h"
#include "rotateBST.h"
/*
	---------------------------------------------------
	End implementations for the BinarySearchTree class.
	---------------------------------------------------
*/

#endif

