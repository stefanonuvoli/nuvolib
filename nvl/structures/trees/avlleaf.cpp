/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "avlleaf.h"

#include <stdexcept>
#include <algorithm>
#include <utility>

#include "internal/bstleaf_helpers.h"
#include "internal/avl_helpers.h"

namespace nvl {


/* --------- CONSTRUCTORS/DESTRUCTORS --------- */

/**
 * @brief Default constructor
 *
 * @param customComparator Custom comparator to be used to compare if a key
 * is less than another one. The default comparator is the < operator
 */
template<class K, class T, class C>
AVLLeaf<K,T,C>::AVLLeaf(const C& customComparator) :
    comparator(customComparator)
{
    this->initialize();
}

/**
 * @brief Constructor with a vector of entries (key/value pairs) to be inserted
 *
 * @param vec Vector of pairs of keys/values
 * @param customComparator Custom comparator to be used to compare if a key
 * is less than another one. The default comparator is the < operator
 */
template<class K, class T, class C>
AVLLeaf<K,T,C>::AVLLeaf(
        const std::vector<std::pair<K,T>>& vec,
        const C& customComparator) :
    comparator(customComparator)
{
    this->initialize();
    this->construction(vec);
}


/**
 * @brief Constructor with a vector of values to be inserted
 *
 * @param vec Vector of values
 * @param customComparator Custom comparator to be used to compare if a key
 * is less than another one. The default comparator is the < operator
 */
template<class K, class T, class C>
AVLLeaf<K,T,C>::AVLLeaf(
        const std::vector<K>& vec,
        const C& customComparator) :
    comparator(customComparator)
{
    this->initialize();
    this->construction(vec);
}

/**
 * @brief Copy constructor
 * @param bst BST
 */
template<class K, class T, class C>
AVLLeaf<K,T,C>::AVLLeaf(const AVLLeaf<K,T,C>& bst) :
    comparator(bst.comparator)
{
    this->root = internal::copySubtreeHelper<Node,T>(bst.root);
    this->entries = bst.entries;
}

/**
 * @brief Move constructor
 * @param bst BST
 */
template<class K, class T, class C>
AVLLeaf<K,T,C>::AVLLeaf(AVLLeaf<K,T,C>&& bst) :
    comparator(bst.comparator)
{
    this->root = bst.root;
    bst.root = nullptr;
    this->entries = bst.entries;
}

/**
 * @brief Destructor
 */
template<class K, class T, class C>
AVLLeaf<K,T,C>::~AVLLeaf()
{
    this->clear();
}




/* --------- PUBLIC METHODS --------- */




/**
 * @brief Construction of the BST given the initial values
 *
 * A clear operation is performed before the construction
 *
 * @param vec Vector of values
 */
template<class K, class T, class C>
void AVLLeaf<K,T,C>::construction(const std::vector<K>& vec)
{
    std::vector<std::pair<K,T>> pairVec;

    for (const K& entry : vec) {
        pairVec.push_back(std::make_pair(entry, entry));
    }

    construction(pairVec);
}

/**
 * @brief Construction of the BST given the initial values (pairs of
 * keys/values)
 *
 * A clear operation is performed before the construction
 *
 * @param vec Vector of pairs of keys/values
 */
template<class K, class T, class C>
void AVLLeaf<K,T,C>::construction(const std::vector<std::pair<K,T>>& vec)
{
    this->clear();

    if (vec.size() == 0)
        return;

    std::vector<std::pair<K,T>> sortedVec(vec.begin(), vec.end());

    //Sort the collection
    internal::PairComparator<K,T> pairComparator(comparator);
    std::sort(sortedVec.begin(), sortedVec.end(), pairComparator);

    //Create nodes
    std::vector<Node*> sortedNodes;
    for (std::pair<K,T>& pair : sortedVec) {
        Node* node = new Node(pair.first, pair.second);
        sortedNodes.push_back(node);
    }

    //Calling the bottom up helper
    this->entries = internal::constructionBottomUpHelperLeaf<Node,K,C>(
                sortedNodes,
                this->root,
                comparator);

    //Update the height of nodes
    for (Node*& node : sortedNodes) {
        if (node != nullptr && node->isLeaf()) {
            internal::updateHeightHelper(node);
        }
    }
}




/**
 * @brief Insert in the BST a given value
 *
 * @param key Key/value to be inserted
 * @return The iterator pointing to the node if it has been
 * successfully inserted, end iterator otherwise
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::iterator AVLLeaf<K,T,C>::insert(const K& key)
{
    return insert(key, key);
}

/**
 * @brief Insert in the BST a given value with the given key
 *
 * If an entry with the same key is already contained, the new
 * entry will be not inserted
 *
 * @param key Key of the entry
 * @param value Value of the entry
 * @return The iterator pointing to the node if it has been
 * successfully inserted, end iterator otherwise
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::iterator AVLLeaf<K,T,C>::insert(
        const K& key, const T& value)
{
    //Create new node
    Node* newNode = new Node(key, value);

    //Insert node
    Node* result = internal::insertNodeHelperLeaf<Node,K,C>(newNode, this->root, comparator);

    //If node has been inserted
    if (result != nullptr) {
        //Update height and rebalance
        internal::updateHeightAndRebalanceHelper(newNode, this->root);

        //Increment entry number
        this->entries++;

        //Returns the iterator to the node
        return iterator(this, newNode);
    }

    //Returns end iterator
    return this->end();
}






/**
 * @brief Erase value from BST given the key
 *
 * @param key Key of the node
 * @return True if item has been found and then erased, false otherwise
 */
template<class K, class T, class C>
bool AVLLeaf<K,T,C>::erase(const K& key)
{
    //Query the BST to find the node
    Node* node = internal::findNodeHelperLeaf(key, this->root, comparator);

    //If the node has been found
    if (node != nullptr) {
        //Erase node
        Node* replacingNode = internal::eraseNodeHelperLeaf(node, this->root);

        //Update height and rebalance
        internal::updateHeightAndRebalanceHelper(replacingNode, this->root);

        //Decrease the number of entries
        this->entries--;

        return true;
    }
    return false;
}

/**
 * @brief Erase an entry from BST given the iterator.
 * Note that iterator is not changed after element is removed
 *
 * @param it A generic iterator pointing to the node to be erased
 */
template<class K, class T, class C>
void AVLLeaf<K,T,C>::erase(generic_iterator it)
{
    //Throw exception if the iterator does not belong to this BST
    if (it.bst != this) {
        throw new std::runtime_error("A tree can only use its own nodes.");
    }

    //Save node to be deleted
    Node* node = it.node;


    if (node != nullptr) {
        //Erase node
        Node* replacingNode = internal::eraseNodeHelperLeaf(node, this->root);

        //Update height and rebalance
        internal::updateHeightAndRebalanceHelper(replacingNode, this->root);

        //Decrease the number of entries
        this->entries--;
    }
}



/**
 * @brief Find entry in the BST given the key
 *
 * @param key Key of the node to be found
 * @return The iterator pointing to the BST node if the element
 * is contained in the BST, end iterator otherwise
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::iterator AVLLeaf<K,T,C>::find(const K& key)
{
    //Query the BST to find the node
    Node* node = internal::findNodeHelperLeaf(key, this->root, comparator);

    return iterator(this, node);
}




/**
 * @brief Find the entry in the BST which is right lower than (or equal to)
 * a given key
 *
 * @param key Input key
 * @return The iterator pointing to the BST node if the element (lower/equal)
 * exists in the BST, end iterator otherwise
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::iterator AVLLeaf<K,T,C>::findLower(const K& key)
{
    //Query the BST to find the node
    Node* node = internal::findLowerHelperLeaf(key, this->root, comparator);

    return iterator(this, node);
}

/**
 * @brief Find the entry in the BST which is right upper than a given key
 *
 * @param key Key of the node to be found
 * @return The iterator pointing to the BST node if the element (upper)
 * exists in the BST, end iterator otherwise
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::iterator AVLLeaf<K,T,C>::findUpper(const K& key)
{
    //Query the BST to find the node
    Node* node = internal::findUpperHelperLeaf(key, this->root, comparator);

    return iterator(this, node);
}






/**
 * @brief Clear the tree, delete all its elements
 *
 */
template<class K, class T, class C>
void AVLLeaf<K,T,C>::clear()
{
    //Clear entire tree
    internal::clearHelper(this->root);

    //Decreasing entries
    this->entries = 0;
}




/**
 * @brief Get the number of entries in the BST
 *
 * @return Number of entries in the BST
 */
template<class K, class T, class C>
TreeSize AVLLeaf<K,T,C>::size() const
{
    return this->entries;
}

/**
 * @brief Check if tree in empty
 *
 * @return True if the BST is empty, false otherwise
 */
template<class K, class T, class C>
bool AVLLeaf<K,T,C>::empty() const
{
    return (this->size() == 0);
}



/**
 * @brief Get max height of the tree
 *
 * @return Max height of the tree
 */
template<class K, class T, class C>
TreeSize AVLLeaf<K,T,C>::getHeight() const
{
    return internal::getHeightHelper(this->root);
}



/**
 * @brief Find entries in the BST that are enclosed in a given range.
 * Start and end are included bounds of the range.
 *
 * @param start Starting value of the range
 * @param end End value of the range
 * @param out Output iterator for the container containing the iterators
 * pointing to the nodes which have keys enclosed in the input range
 */
template<class K, class T, class C> template<class OutputIterator>
void AVLLeaf<K,T,C>::rangeQuery(
        const K& start, const K& end,
        OutputIterator out)
{
    //Output
    std::vector<Node*> nodeOutput;

    //Execute range query
    internal::rangeQueryHelperLeaf(start, end, nodeOutput, this->root, comparator);

    for (Node* node : nodeOutput) {
        *out = iterator(this, node);
        out++;
    }
}



/* ----- ITERATOR MIN/MAX NEXT/PREV ----- */

/**
 * @brief Get minimum key entry in the BST
 *
 * @return The iterator pointing to the minimum node
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::iterator AVLLeaf<K,T,C>::getMin()
{
    return iterator(this, internal::getMinimumHelperLeaf(this->root));
}

/**
 * @brief Get maximum key entry in the BST
 *
 * @return The iterator pointing to the maximum node
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::iterator AVLLeaf<K,T,C>::getMax()
{
    return iterator(this, internal::getMaximumHelperLeaf(this->root));
}



/**
 * @brief Get successor of a element pointed by iterator
 *
 * @param it Iterator pointing to the node
 * @return The iterator pointing to the successor node (end
 * iterator if it has no successor)
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::generic_iterator AVLLeaf<K,T,C>::getNext(generic_iterator it)
{
    //Throw exception if the iterator does not belong to this BST
    if (it.bst != this) {
        throw new std::runtime_error("A tree can only use its own nodes.");
    }
    return generic_iterator(this, internal::getSuccessorHelperLeaf(it.node));
}

/**
 * @brief Get predecessor of a element pointed by iterator
 *
 * @param it Iterator pointing to the node
 * @return The iterator pointing to the predecessor node (end
 * iterator if it has no predecessor)
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::generic_iterator AVLLeaf<K,T,C>::getPrev(generic_iterator it)
{
    //Throw exception if the iterator does not belong to this BST
    if (it.bst != this) {
        throw new std::runtime_error("A tree can only use its own nodes.");
    }
    return generic_iterator(this, internal::getPredecessorHelperLeaf(it.node));
}



/* --------- ITERATORS --------- */

/**
 * @brief Begin iterator
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::iterator AVLLeaf<K,T,C>::begin()
{
    return iterator(this, internal::getMinimumHelperLeaf(this->root));
}

/**
 * @brief End iterator
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::iterator AVLLeaf<K,T,C>::end()
{
    return iterator(this, nullptr);
}


/**
 * @brief Begin const iterator
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::const_iterator AVLLeaf<K,T,C>::cbegin()
{
    return const_iterator(this, internal::getMinimumHelperLeaf(this->root));
}

/**
 * @brief End const iterator
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::const_iterator AVLLeaf<K,T,C>::cend()
{
    return const_iterator(this, nullptr);
}


/**
 * @brief Begin reverse iterator
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::reverse_iterator AVLLeaf<K,T,C>::rbegin()
{
    return reverse_iterator(this, internal::getMaximumHelperLeaf(this->root));
}

/**
 * @brief End reverse iterator
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::reverse_iterator AVLLeaf<K,T,C>::rend()
{
    return reverse_iterator(this, nullptr);
}


/**
 * @brief Begin const reverse iterator
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::const_reverse_iterator AVLLeaf<K,T,C>::crbegin()
{
    return const_reverse_iterator(this, internal::getMaximumHelperLeaf(this->root));
}

/**
 * @brief End const reverse iterator
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::const_reverse_iterator AVLLeaf<K,T,C>::crend()
{
    return const_reverse_iterator(this, nullptr);
}



/**
 * @brief Insert output iterator
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::insert_iterator AVLLeaf<K,T,C>::inserter()
{
    return insert_iterator(this);
}



/**
 * @brief Get range based iterator of the BST
 *
 * @return Range based iterator
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::RangeBasedIterator AVLLeaf<K,T,C>::getIterator()
{
    return RangeBasedIterator(this);
}

/**
 * @brief Get range based const iterator of the BST
 *
 * @return Range based const iterator
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::RangeBasedConstIterator AVLLeaf<K,T,C>::getConstIterator()
{
    return RangeBasedConstIterator(this);
}

/**
 * @brief Get range based reverse iterator of the BST
 *
 * @return Range based reverse iterator
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::RangeBasedReverseIterator AVLLeaf<K,T,C>::getReverseIterator()
{
    return RangeBasedReverseIterator(this);
}

/**
 * @brief Get range based const reverse iterator of the BST
 *
 * @return Range based const reverse iterator
 */
template<class K, class T, class C>
typename AVLLeaf<K,T,C>::RangeBasedConstReverseIterator AVLLeaf<K,T,C>::getConstReverseIterator()
{
    return RangeBasedConstReverseIterator(this);
}



/* ----- SWAP FUNCTION AND ASSIGNMENT ----- */


/**
 * @brief Assignment operator
 * @param bst Parameter BST
 * @return This object
 */
template<class K, class T, class C>
AVLLeaf<K,T,C>& AVLLeaf<K,T,C>::operator= (AVLLeaf<K,T,C> bst)
{
    swap(bst);
    return *this;
}


/**
 * @brief Swap BST with another one
 * @param bst BST to be swapped with this object
 */
template<class K, class T, class C>
void AVLLeaf<K,T,C>::swap(AVLLeaf<K,T,C>& bst)
{
    using std::swap;
    swap(this->root, bst.root);
    swap(this->entries, bst.entries);
    swap(this->comparator, bst.comparator);
}


/**
 * @brief Swap graph with another one
 * @param b1 First BST
 * @param b2 Second BST
 */
template<class K, class T, class C>
void swap(AVLLeaf<K,T,C>& b1, AVLLeaf<K,T,C>& b2)
{
    b1.swap(b2);
}


/* --------- PRIVATE METHODS --------- */

/**
 * @brief Initialization of the BST
 */
template<class K, class T, class C>
void AVLLeaf<K,T,C>::initialize()
{
    this->root = nullptr;
    this->entries = 0;
}


}
