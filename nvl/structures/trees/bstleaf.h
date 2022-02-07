/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_BSTLEAF_H
#define NVL_BSTLEAF_H

#include <nvl/nuvolib.h>

#include <vector>
#include <utility>

#include "internal/tree_common.h"

#include "internal/iterators/tree_iterator.h"
#include "internal/iterators/tree_reverseiterator.h"
#include "internal/iterators/tree_insertiterator.h"
#include "internal/iterators/tree_rangebased_iterators.h"

#include "internal/nodes/bst_node.h"

namespace nvl {

/**
 * @brief A binary search tree
 *
 * Keys and values are saved only in the leaves
 * No duplicates are allowed.
 */
template <class K, class T = K, class C = DefaultComparatorType<K>>
class BSTLeaf
{

public:

    /* Typedefs */

    typedef internal::BSTNode<K,T> Node;

    typedef TreeGenericIterator<BSTLeaf<K,T,C>, Node> generic_iterator;

    typedef TreeIterator<BSTLeaf<K,T,C>, Node, T> iterator;
    typedef TreeIterator<BSTLeaf<K,T,C>, Node, const T> const_iterator;

    typedef TreeReverseIterator<BSTLeaf<K,T,C>, Node, T> reverse_iterator;
    typedef TreeReverseIterator<BSTLeaf<K,T,C>, Node, const T> const_reverse_iterator;

    typedef TreeInsertIterator<BSTLeaf<K,T,C>, K> insert_iterator;

    typedef TreeRangeBasedIterator<BSTLeaf<K,T,C>> RangeBasedIterator;
    typedef TreeRangeBasedConstIterator<BSTLeaf<K,T,C>> RangeBasedConstIterator;
    typedef TreeRangeBasedReverseIterator<BSTLeaf<K,T,C>> RangeBasedReverseIterator;
    typedef TreeRangeBasedConstReverseIterator<BSTLeaf<K,T,C>> RangeBasedConstReverseIterator;



    /* Constructors/destructor */

    explicit BSTLeaf(const C& customComparator = &internal::defaultComparator<K>);
    explicit BSTLeaf(const std::vector<std::pair<K,T>>& vec,
            const C& customComparator = &internal::defaultComparator<K>);
    explicit BSTLeaf(const std::vector<K>& vec,
            const C& customComparator = &internal::defaultComparator<K>);

    BSTLeaf(const BSTLeaf<K,T,C>& bst);
    BSTLeaf(BSTLeaf<K,T,C>&& bst);

    ~BSTLeaf();



    /* Public methods */

    void construction(const std::vector<K>& vec);
    void construction(const std::vector<std::pair<K,T>>& vec);

    iterator insert(const K& key);
    iterator insert(const K& key, const T& value);

    bool erase(const K& key);
    void erase(generic_iterator it);

    iterator find(const K& key);

    iterator findLower(const K& key);
    iterator findUpper(const K& key);

    void clear();

    TreeSize size() const;
    bool empty() const;

    TreeSize getHeight() const;



    template <class OutputIterator>
    void rangeQuery(
            const K& start, const K& end,
            OutputIterator out);



    /* Iterator Min/Max Next/Prev */

    iterator getMin();
    iterator getMax();

    generic_iterator getNext(const generic_iterator it);
    generic_iterator getPrev(const generic_iterator it);



    /* Iterators */

    iterator begin();
    iterator end();

    const_iterator cbegin();
    const_iterator cend();

    reverse_iterator rbegin();
    reverse_iterator rend();

    const_reverse_iterator crbegin();
    const_reverse_iterator crend();

    insert_iterator inserter();

    RangeBasedIterator getIterator();
    RangeBasedConstIterator getConstIterator();
    RangeBasedReverseIterator getReverseIterator();
    RangeBasedConstReverseIterator getConstReverseIterator();


    /* Swap function and assignment */

    inline BSTLeaf<K,T,C>& operator= (BSTLeaf<K,T,C> bst);
    inline void swap(BSTLeaf<K,T,C>& bst);

protected:

    /* Protected fields */

    Node* root;

    TreeSize entries;

    C comparator;


    /* Protected methods */

    void initialize();

};


template <class K, class T, class C>
void swap(BSTLeaf<K,T,C>& b1, BSTLeaf<K,T,C>& b2);


}


#include "bstleaf.cpp"

#endif // NVL_BSTLEAF_H




