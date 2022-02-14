#include "octree.h"

#include "internal/tree_common.h"

#include <stack>

#include <limits>
#include <algorithm>
#include <limits>

#define NVL_OCTREE_DEFAULT_BOX_ELEMENTS 10

namespace nvl {

/**
 * @brief Default constructor
 */
template<class K, class T, class R>
Octree<K,T,R>::Octree(const K& center, const R& radius) :
    center(center),
    radius(radius),
    maxBoxElements(NVL_OCTREE_DEFAULT_BOX_ELEMENTS),
    maxBoxRadius(std::numeric_limits<R>::max())
{
    this->initialize();
}

/**
 * @brief Constructor with a vector of entries (key/value pairs) to be inserted
 */
template<class K, class T, class R>
Octree<K,T,R>::Octree(
        const K& center,
        const R& radius,
        const std::vector<std::pair<K,T>>& vec) :
    center(center),
    radius(radius),
    maxBoxElements(NVL_OCTREE_DEFAULT_BOX_ELEMENTS),
    maxBoxRadius(std::numeric_limits<double>::max())
{
    this->initialize();
    this->construction(vec);
}


/**
 * @brief Constructor with a vector of values to be inserted
 * @param vec Vector of values
 */
template<class K, class T, class R>
Octree<K,T,R>::Octree(
        const K& center,
        const R& radius,
        const std::vector<K>& vec) :
    center(center),
    radius(radius),
    maxBoxElements(NVL_OCTREE_DEFAULT_BOX_ELEMENTS),
    maxBoxRadius(std::numeric_limits<double>::max())
{
    this->initialize();
    this->construction(vec);
}

/**
 * @brief Copy constructor
 * @param octree Octree
 */
template<class K, class T, class R>
Octree<K,T,R>::Octree(const Octree<K,T,R>& octree)
{
    this->root = this->copySubtreeHelper(octree.root);
    this->entries = octree.entries;
    this->center = octree.center;
    this->radius = octree.radius;
    this->maxBoxElements = octree.maxBoxElements;
    this->maxBoxRadius = octree.maxBoxRadius;
}

/**
 * @brief Move constructor
 * @param octree Octree
 */
template<class K, class T, class R>
Octree<K,T,R>::Octree(Octree<K,T,R>&& octree)
{
    this->root = octree.root;
    octree.root = nullptr;
    this->entries = octree.entries;    
    this->center = octree.center;
    this->radius = octree.radius;
    this->maxBoxElements = octree.maxBoxElements;
    this->maxBoxRadius = octree.maxBoxRadius;
}

/**
 * @brief Destructor
 */
template<class K, class T, class R>
Octree<K,T,R>::~Octree()
{
    this->clear();
}

/* --------- PUBLIC METHODS --------- */

/**
 * @brief Construction of the octree given the initial values
 * A clear operation is performed before the construction
 * @param vec Vector of values
 */
template<class K, class T, class R>
void Octree<K,T,R>::construction(const std::vector<K>& vec)
{
    std::vector<std::pair<K,T>> pairVec;

    for (const K& entry : vec) {
        pairVec.push_back(std::make_pair(entry, entry));
    }

    construction(pairVec);
}

/**
 * @brief Construction of the AABB tree given the initial values (pairs of
 * keys/values)
 * A clear operation is performed before the construction
 * @param vec Vector of pairs of keys/values
 */
template<class K, class T, class R>
void Octree<K,T,R>::construction(const std::vector<std::pair<K,T>>& vec)
{
    this->clear();

    if (vec.size() == 0)
        return;

    for (const std::pair<K,T>& v : vec) {
        this->insert(v);
    }
}

/**
 * @brief Insert in the octree a given value
 * @param key Key/value to be inserted
 * @return The pointer to the node
 */
template<class K, class T, class R>
typename Octree<K,T,R>::Node* Octree<K,T,R>::insert(const K& key)
{
    return insert(key, key);
}

/**
 * @brief Insert in the octree a given value with the given key
 * @param key Key of the entry
 * @param value Value of the entry
 * @return The pointer to the node
 */
template<class K, class T, class R>
typename Octree<K,T,R>::Node* Octree<K,T,R>::insert(
        const K& key, const T& value)
{
    //Find closest node or create if there is no root
    Node* node = this->findContainingNodeHelper(this->root, key);
    if (node == nullptr) {
        node = new Node(this->center, this->radius);
        this->root = node;
    }
    node->values.push_back(std::make_pair(key, value));

    //Split if needed
    std::stack<Node*> stack;
    stack.push(node);
    while (!stack.empty()) {
        Node* n = stack.top();
        stack.pop();

        if (n->radius > maxBoxRadius || n->values.size() > maxBoxElements) {
            n->split();
            for (Node* child : n->children) {
                stack.push(child);
            }
        }
    }

    assert(node != nullptr);

    //Increment entry number
    this->entries++;

    //Returns pointer to the node (not a leaf)
    return node;
}

/**
 * @brief Find value in an octree
 * @param key Key
 * @return The pointer to the node containing the element, nullptr otherwise
 */
template<class K, class T, class R>
typename Octree<K,T,R>::Node* Octree<K,T,R>::find(const K& key)
{
    Node* node = this->findContainingNodeHelper(this->root, key);

    //Return node if it contains the value
    if (std::find(node->values.begin(), node->values.end(), key) != node->values.end()) {
        return node;
    }

    return nullptr;
}

/**
 * @brief Find value in an octree
 * @param key Key
 * @return The pointer to the closest node containing the element, nullptr if empty
 */
template<class K, class T, class R>
typename Octree<K,T,R>::Node* Octree<K,T,R>::findClosest(const K& key)
{
    return this->findContainingNodeHelper(this->root, key);
}

/**
 * @brief Find value in an octree
 * @param key Key
 * @return The pointer to the node containing the element, nullptr otherwise
 */
template<class K, class T, class R>
const typename Octree<K,T,R>::Node* Octree<K,T,R>::find(const K& key) const
{
    Node* node = this->findContainingNodeHelper(this->root, key);

    //Return node if it contains the value
    if (std::find(node->values.begin(), node->values.end(), key) != node->values.end()) {
        return node;
    }

    return nullptr;
}

/**
 * @brief Find value in an octree
 * @param key Key
 * @return The pointer to the closest node containing the element, nullptr if empty
 */
template<class K, class T, class R>
const typename Octree<K,T,R>::Node* Octree<K,T,R>::findClosest(const K& key) const
{
    return this->findContainingNodeHelper(this->root, key);
}

/**
 * @brief Get the number of entries in the Octree
 * @return Number of entries in the octree
 */
template<class K, class T, class R>
size_t Octree<K,T,R>::size() const
{
    return this->entries;
}

/**
 * @brief Get the number of entries in the Octree
 * @return Number of entries in the octree
 */
template<class K, class T, class R>
bool Octree<K,T,R>::empty() const
{
    return this->entries == 0;
}

/**
 * @brief Clear the tree, delete all its elements
 */
template<class K, class T, class R>
void Octree<K,T,R>::clear()
{
    //Clear
    this->clearHelper(this->root);

    //Clear entries
    this->entries = 0;
}

/**
 * @brief Get root node
 * @return Root node
 */
template<class K, class T, class R>
typename Octree<K,T,R>::Node* Octree<K,T,R>::getRoot()
{
    return this->root;
}

/**
 * @brief Get root node
 * @return Root node
 */
template<class K, class T, class R>
const typename Octree<K,T,R>::Node* Octree<K, T, R>::getRoot() const
{
    return this->root;
}

/**
 * @brief Get center of the octree
 * @return Center
 */
template<class K, class T, class R>
const K& Octree<K,T,R>::getCenter() const
{
    return this->center;
}

/**
 * @brief Get radius of the octree
 * @return Radius
 */
template<class K, class T, class R>
const R& Octree<K,T,R>::getRadius() const
{
    return this->radius;
}

/**
 * @brief Set center of the octree
 * @param newCenter Center
 */
template<class K, class T, class R>
void Octree<K,T,R>::setCenter(const K& center)
{
    this->center = center;
}

/**
 * @brief Set radius of the octree
 * @param newRadius Radius
 */
template<class K, class T, class R>
void Octree<K,T,R>::setRadius(const R& radius)
{
    this->radius = radius;
}

/**
 * @brief Get maximum box radius of the octree
 * @return Maximum box radius
 */
template<class K, class T, class R>
const R& Octree<K,T,R>::getMaxBoxRadius() const
{
    return this->maxBoxRadius;
}

/**
 * @brief Set maximum number of elements in the box of the octree
 * @param Maximum number of elements
 */
template<class K, class T, class R>
void Octree<K,T,R>::setMaxBoxRadius(const R& boxRadius)
{
    this->maxBoxRadius = boxRadius;
}

/**
 * @brief Get maximum number of elements in the box of the octree
 * @return Maximum number of elements
 */
template<class K, class T, class R>
const size_t& Octree<K,T,R>::getMaxBoxElements() const
{
    return this->maxBoxElements;
}

/**
 * @brief Set maximum number of elements in the box of the octree
 * @param Maximum number of elements
 */
template<class K, class T, class R>
void Octree<K,T,R>::setMaxBoxElements(size_t boxElements)
{
    this->maxBoxElements = boxElements;
}


/* ----- SWAP FUNCTION AND ASSIGNMENT ----- */

/**
 * @brief Assignment operator
 * @param octree Parameter octree
 * @return This object
 */
template<class K, class T, class R>
Octree<K,T,R>& Octree<K,T,R>::operator= (Octree<K,T,R> octree)
{
    swap(octree);
    return *this;
}

/**
 * @brief Swap octree with another one
 * @param octree Octree to be swapped with this object
 */
template<class K, class T, class R>
void Octree<K,T,R>::swap(Octree<K,T,R>& octree)
{
    using std::swap;
    swap(this->root, octree.root);
    swap(this->entries, octree.entries);
    swap(this->center, octree.center);
    swap(this->radius, octree.radius);
    swap(this->maxBoxElements, octree.maxBoxElements);
    swap(this->maxBoxRadius, octree.maxBoxRadius);
}

/**
 * @brief Swap graph with another one
 * @param b1 First octree
 * @param b2 Second octree
 */
template<class K, class T, class R>
void swap(Octree<K,T,R>& b1, Octree<K,T,R>& b2)
{
    b1.swap(b2);
}

/* --------- PROTECTED METHODS --------- */

/**
 * @brief Initialization of the octree
 */
template<class K, class T, class R>
void Octree<K,T,R>::initialize()
{
    this->root = nullptr;
    this->entries = 0;
}

/**
 * @brief Create a copy of a given octree, having as root
 * the rootNode
 * @param rootNode Root of the subtree
 * @returns Copy of the subtree
 */
template<class K, class T, class R>
typename Octree<K,T,R>::Node* Octree<K,T,R>::copySubtreeHelper(
        const Node* rootNode,
        Node* parent)
{
    if (rootNode == nullptr)
        return nullptr;

    Node* newNode = new Node(*rootNode);

    for (int i = 0; i < 8; ++i) {
        Node* childNode = rootNode->children[i];
        if (childNode != nullptr) {
            newNode->children[i] = copySubtreeHelper(rootNode->children[i], newNode);
        }
    }

    newNode->parent = parent;
    if (rootNode->value != nullptr)
        newNode->value = new T(*(rootNode->value));

    return newNode;
}

/**
 * @brief Find closest node to the given key
 * @param rootNode Root of the subtree
 * @param key Key
 * @returns The closest node, nullptr if empty or the key is not inside the root node box
 */
template<class K, class T, class R>
typename Octree<K,T,R>::Node* Octree<K,T,R>::findContainingNodeHelper(
        Node* rootNode,
        const K& key)
{
    if (rootNode == nullptr)
        return nullptr;

    if (!rootNode->contains(key)) {
        return nullptr;
    }

    const K& center = rootNode->center;

    Node* child = nullptr;
    if (key.y() > center.y()) {
        //North (N1, N2, N3, N4)
        if (key.x() > center.x()) {
            //North east (N1, N2)
            if (key.z() > center.z()) {
                child = rootNode->children[Node::X1Y1Z1];
            }
            else {
                child = rootNode->children[Node::X1Y1Z0];
            }
        }
        else {
            //North west (N3, N4)
            if (key.z() > center.z()) {
                child = rootNode->children[Node::X0Y1Z1];
            }
            else {
                child = rootNode->children[Node::X0Y1Z0];
            }
        }
    }
    else {
        //South (S1, S2, S3, S4)
        if (key.x() > center.x()) {
            //South east (S1, S2)
            if (key.z() > center.z()) {
                child = rootNode->children[Node::X1Y0Z1];
            }
            else {
                child = rootNode->children[Node::X1Y0Z0];
            }
        }
        else {
            //South west (S3, S4)
            if (key.z() > center.z()) {
                child = rootNode->children[Node::X0Y0Z1];
            }
            else {
                child = rootNode->children[Node::X0Y0Z0];
            }
        }
    }

    if (child != nullptr) {
        return this->findContainingNodeHelper(child, key);
    }

    assert(rootNode == nullptr || rootNode->contains(key));
    return rootNode;
}

/**
 * @brief Find closest node to the given key
 * @param rootNode Root of the subtree
 * @param key Key
 * @returns The closest node, nullptr if empty or the key is not inside the root node box
 */
template<class K, class T, class R>
const typename Octree<K,T,R>::Node* Octree<K,T,R>::findContainingNodeHelper(
        const Node* rootNode,
        const K& key) const
{
    if (rootNode == nullptr)
        return nullptr;

    if (!rootNode->contains(key)) {
        return nullptr;
    }

    const K& center = rootNode->center;

    Node* child = nullptr;
    if (key.y() > center.y()) {
        //North (N1, N2, N3, N4)
        if (key.x() > center.x()) {
            //North east (N1, N2)
            if (key.z() > center.z()) {
                child = rootNode->children[Node::X1Y1Z1];
            }
            else {
                child = rootNode->children[Node::X1Y1Z0];
            }
        }
        else {
            //North west (N3, N4)
            if (key.z() > center.z()) {
                child = rootNode->children[Node::X0Y1Z1];
            }
            else {
                child = rootNode->children[Node::X0Y1Z0];
            }
        }
    }
    else {
        //South (S1, S2, S3, S4)
        if (key.x() > center.x()) {
            //South east (S1, S2)
            if (key.z() > center.z()) {
                child = rootNode->children[Node::X1Y0Z1];
            }
            else {
                child = rootNode->children[Node::X1Y0Z0];
            }
        }
        else {
            //South west (S3, S4)
            if (key.z() > center.z()) {
                child = rootNode->children[Node::X0Y0Z1];
            }
            else {
                child = rootNode->children[Node::X0Y0Z0];
            }
        }
    }

    if (child != nullptr) {
        return this->findContainingNodeHelper(child, key);
    }

    assert(rootNode == nullptr || rootNode->contains(key));
    return rootNode;
}

/**
 * @brief Clear a subtree of the octree
 * @param rootNode Root of the subtree
 */
template<class K, class T, class R>
void Octree<K,T,R>::clearHelper(
        const Node* rootNode)
{
    //If it is already empty
    if (rootNode == nullptr)
        return;

    //Clear subtrees
    for (Node* childNode : rootNode->children) {
        clearHelper(childNode);
    }

    //Delete data
    delete rootNode;
    rootNode = nullptr;
}

}
