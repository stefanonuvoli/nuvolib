#ifndef NVL_OCTREE_H
#define NVL_OCTREE_H

#include "internal/nodes/octree_node.h"

#include <vector>
#include <stddef.h>

namespace nvl {

/**
 * @brief Octree class
 * @tparam K Key
 * @tparam T Value
 */
template<class K, class T = K, class R = typename K::Scalar>
class Octree
{

public:

    /* Typedefs */

    typedef internal::OctreeNode<K,T,R> Node;

    explicit Octree(const K& center, const R& radius);
    explicit Octree(const K& center, const R& radius, const std::vector<std::pair<K,T>>& vec);
    explicit Octree(const K& center, const R& radius, const std::vector<K>& vec);

    Octree(const Octree<K,T,R>& bst);
    Octree(Octree<K,T,R>&& bst);

    ~Octree();

    void construction(const std::vector<K>& vec);
    void construction(const std::vector<std::pair<K,T>>& vec);

    Node* insert(const K& key);
    Node* insert(const K& key, const T& value);

    Node* find(const K& key);
    Node* findClosest(const K& key);
    const Node* find(const K& key) const;
    const Node* findClosest(const K& key) const;

    size_t size() const;
    bool empty() const;

    void clear();

    Node* getRoot();
    const Node* getRoot() const;
    const K& getCenter() const;
    const R& getRadius() const;
    void setCenter(const K& center);
    void setRadius(const R& radius);

    const R& getMaxBoxRadius() const;
    void setMaxBoxRadius(const R& boxRadius);
    const size_t& getMaxBoxElements() const;
    void setMaxBoxElements(size_t boxElements);


    /* Swap function and assignment */

    inline Octree<K,T,R>& operator= (Octree<K,T,R> bst);
    inline void swap(Octree<K,T,R>& bst);


protected:

    /* Protected fields */

    Node* root;

    size_t entries;

    K center;
    R radius;

    size_t maxBoxElements;
    R maxBoxRadius;


    /* Protected methods */

    void initialize();

    Node* copySubtreeHelper(
            const Node* rootNode,
            Node* parent = nullptr);
    Node* findContainingNodeHelper(
            Node* rootNode,
            const K& key);
    const Node* findContainingNodeHelper(
            const Node* rootNode,
            const K& key) const;
    void clearHelper(
            const Node* rootNode);

};

template<class K, class T, class R>
void swap(Octree<K,T,R>& b1, Octree<K,T,R>& b2);

}

#include "octree.cpp"

#endif // NVL_OCTREE_H
