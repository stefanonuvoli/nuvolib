/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "octree_node.h"

#include <queue>
#include <algorithm>

#include "assert.h"

namespace nvl {

namespace internal {

/* --------- CONSTRUCTORS/DESTRUCTOR --------- */

/**
 * @brief Constructor with key and value
 *
 * @param key Key of the node
 * @param value Value of the node
 */
template<class K, class T, class R>
OctreeNode<K,T,R>::OctreeNode(
        const K& center,
        const R& radius) :
    parent(nullptr)
{
    init(center, radius);
}


/* --------- PUBLIC METHODS --------- */

/**
 * @brief Split node in 8 parts, redistributing all the
 * values
 */
template<class K, class T, class R>
void OctreeNode<K,T,R>::split()
{

#ifndef NDEBUG
    size_t n = this->values.size();
    size_t inserted = 0;
#endif

    for (int i = X0Y0Z0; i <= X1Y1Z1; ++i) {
        R r = 0.5 * radius;

        K c;
        if      (i == X0Y0Z0) c = center + K(-r, -r, -r);
        else if (i == X0Y0Z1) c = center + K(-r, -r, +r);
        else if (i == X0Y1Z0) c = center + K(-r, +r, -r);
        else if (i == X0Y1Z1) c = center + K(-r, +r, +r);
        else if (i == X1Y0Z0) c = center + K(+r, -r, -r);
        else if (i == X1Y0Z1) c = center + K(+r, -r, +r);
        else if (i == X1Y1Z0) c = center + K(+r, +r, -r);
        else if (i == X1Y1Z1) c = center + K(+r, +r, +r);

        this->children[i] = new OctreeNode<K,T,R>(c,r);
        this->children[i]->parent = this;
        this->children[i]->position = static_cast<OctreeNodePosition>(i);

        for (const std::pair<K,T>& v : values) {
            if (this->children[i]->contains(v.first)) {
                this->children[i]->values.push_back(std::make_pair(v.first, v.second));
#ifndef NDEBUG
                inserted++;
#endif
            }
        }
    }

#ifndef NDEBUG
    assert(n == inserted);
#endif

    this->values.clear();
}

/**
 * @brief Check if the node contains the key
 * @param key Key
 * @return True if it is contained, false otherwise
 */
template<class K, class T, class R>
bool OctreeNode<K,T,R>::contains(const K& key) const
{
    if (key.x() < center.x() - radius) {
        return false;
    }
    if (key.x() >= center.x() + radius) {
        return false;
    }
    if (key.y() < center.y() - radius) {
        return false;
    }
    if (key.y() >= center.y() + radius) {
        return false;
    }
    if (key.z() < center.z() - radius) {
        return false;
    }
    if (key.z() >= center.z() + radius) {
        return false;
    }

    return true;
}

/**
 * @brief Check if the node is a leaf
 *
 * @return True if the node is a leaf
 */
template<class K, class T, class R>
bool OctreeNode<K,T,R>::isLeaf() const
{
    for (OctreeNode<K,T,R>* child : this->children) {
        if (child != nullptr) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Get neighbors of the current node
 * @return Collection of neighbors
 */
template<class K, class T, class R>
std::vector<const OctreeNode<K,T,R>*> OctreeNode<K,T,R>::neighbors(
        const OctreeDirection& direction) const
{
    const OctreeNode<K,T,R>* node = this->getNeighborHelper(direction);
    std::vector<const OctreeNode<K,T,R>*> n = this->getNeighborLeavesHelper(direction, node);

    return n;
}

/**
 * @brief Get neighbors of the current node
 * @return Collection of neighbors
 */
template<class K, class T, class R>
std::vector<const OctreeNode<K,T,R>*> OctreeNode<K,T,R>::neighbors() const
{
    std::vector<const OctreeNode<K,T,R>*> neighbors;

    for (int i = X0; i <= Z1; ++i) {
        std::vector<const OctreeNode<K,T,R>*> nd1 = this->neighbors(static_cast<OctreeDirection>(i));
        neighbors.insert(neighbors.end(), nd1.begin(), nd1.end());

        for (const OctreeNode<K,T,R>*& nd : nd1) {
            for (int j = X0; j <= Z1; ++j) {
                if (((i == X0 || i == X1) && j != X0 && j != X1) ||
                    ((i == Y0 || i == Y1) && j != Y0 && j != Y1) ||
                    ((i == Z0 || i == Z1) && j != Z0 && j != Z1))
                {
                    std::vector<const OctreeNode<K,T,R>*> nd2 = nd->neighbors(static_cast<OctreeDirection>(j));
                    neighbors.insert(neighbors.end(), nd2.begin(), nd2.end());
                }
            }
        }
    }

    std::sort(neighbors.begin(), neighbors.end());
    neighbors.erase(std::unique(neighbors.begin(), neighbors.end()), neighbors.end());
    neighbors.erase(std::remove(neighbors.begin(), neighbors.end(), this), neighbors.end());

    return neighbors;
}


/* --------- PRIVATE METHODS --------- */

/**
 * @brief Get neighbors of the current node
 * @param direction Direction
 * @return Collection of neighbors
 */
template<class K, class T, class R>
const OctreeNode<K,T,R>* OctreeNode<K,T,R>::getNeighborHelper(
        const OctreeDirection& direction) const
{
    if (this->parent == nullptr)
        return nullptr;

    switch (direction) {
    case X0:
        if (this->position == X1Y0Z0)
            return this->parent->children[X0Y0Z0];
        if (this->position == X1Y0Z1)
            return this->parent->children[X0Y0Z1];
        if (this->position == X1Y1Z0)
            return this->parent->children[X0Y1Z0];
        if (this->position == X1Y1Z1)
            return this->parent->children[X0Y1Z1];
        break;
    case X1:
        if (this->position == X0Y0Z0)
            return this->parent->children[X1Y0Z0];
        if (this->position == X0Y0Z1)
            return this->parent->children[X1Y0Z1];
        if (this->position == X0Y1Z0)
            return this->parent->children[X1Y1Z0];
        if (this->position == X0Y1Z1)
            return this->parent->children[X1Y1Z1];
        break;
    case Y0:
        if (this->position == X0Y1Z0)
            return this->parent->children[X0Y0Z0];
        if (this->position == X0Y1Z1)
            return this->parent->children[X0Y0Z1];
        if (this->position == X1Y1Z0)
            return this->parent->children[X1Y0Z0];
        if (this->position == X1Y1Z1)
            return this->parent->children[X1Y0Z1];
        break;
    case Y1:
        if (this->position == X0Y0Z0)
            return this->parent->children[X0Y1Z0];
        if (this->position == X0Y0Z1)
            return this->parent->children[X0Y1Z1];
        if (this->position == X1Y0Z0)
            return this->parent->children[X1Y1Z0];
        if (this->position == X1Y0Z1)
            return this->parent->children[X1Y1Z1];
        break;
    case Z0:
        if (this->position == X0Y0Z1)
            return this->parent->children[X0Y0Z0];
        if (this->position == X0Y1Z1)
            return this->parent->children[X0Y1Z0];
        if (this->position == X1Y0Z1)
            return this->parent->children[X1Y0Z0];
        if (this->position == X1Y1Z1)
            return this->parent->children[X1Y1Z0];
        break;
    case Z1:
        if (this->position == X0Y0Z0)
            return this->parent->children[X0Y0Z1];
        if (this->position == X0Y1Z0)
            return this->parent->children[X0Y1Z1];
        if (this->position == X1Y0Z0)
            return this->parent->children[X1Y0Z1];
        if (this->position == X1Y1Z0)
            return this->parent->children[X1Y1Z1];
        break;

    default:
        assert(false);
    }

    const OctreeNode<K,T,R>* node = this->parent->getNeighborHelper(direction);
    if (node == nullptr || node->isLeaf())
        return node;

    switch (direction) {
    case X0:
        assert(this->position != X1Y0Z0);
        assert(this->position != X1Y0Z1);
        assert(this->position != X1Y1Z0);
        assert(this->position != X1Y1Z1);
        if (this->position == X0Y0Z0)
            return node->children[X1Y0Z0];
        if (this->position == X0Y0Z1)
            return node->children[X1Y0Z1];
        if (this->position == X0Y1Z0)
            return node->children[X1Y1Z0];
        if (this->position == X0Y1Z1)
            return node->children[X1Y1Z1];
        break;
    case X1:
        assert(this->position != X0Y0Z0);
        assert(this->position != X0Y0Z1);
        assert(this->position != X0Y1Z0);
        assert(this->position != X0Y1Z1);
        if (this->position == X1Y0Z0)
            return node->children[X0Y0Z0];
        if (this->position == X1Y0Z1)
            return node->children[X0Y0Z1];
        if (this->position == X1Y1Z0)
            return node->children[X0Y1Z0];
        if (this->position == X1Y1Z1)
            return node->children[X0Y1Z1];
        break;
    case Y0:
        assert(this->position != X0Y1Z0);
        assert(this->position != X0Y1Z1);
        assert(this->position != X1Y1Z0);
        assert(this->position != X1Y1Z1);
        if (this->position == X0Y0Z0)
            return node->children[X0Y1Z0];
        if (this->position == X0Y0Z1)
            return node->children[X0Y1Z1];
        if (this->position == X1Y0Z0)
            return node->children[X1Y1Z0];
        if (this->position == X1Y0Z1)
            return node->children[X1Y1Z1];
        break;
    case Y1:
        assert(this->position != X0Y0Z0);
        assert(this->position != X0Y0Z1);
        assert(this->position != X1Y0Z0);
        assert(this->position != X1Y0Z1);
        if (this->position == X0Y1Z0)
            return node->children[X0Y0Z0];
        if (this->position == X0Y1Z1)
            return node->children[X0Y0Z1];
        if (this->position == X1Y1Z0)
            return node->children[X1Y0Z0];
        if (this->position == X1Y1Z1)
            return node->children[X1Y0Z1];
        break;
    case Z0:
        assert(this->position != X0Y0Z1);
        assert(this->position != X0Y1Z1);
        assert(this->position != X1Y0Z1);
        assert(this->position != X1Y1Z1);
        if (this->position == X0Y0Z0)
            return node->children[X0Y0Z1];
        if (this->position == X0Y1Z0)
            return node->children[X0Y1Z1];
        if (this->position == X1Y0Z0)
            return node->children[X1Y0Z1];
        if (this->position == X1Y1Z0)
            return node->children[X1Y1Z1];
        break;
    case Z1:
        assert(this->position != X0Y0Z0);
        assert(this->position != X0Y1Z0);
        assert(this->position != X1Y0Z0);
        assert(this->position != X1Y1Z0);
        if (this->position == X0Y0Z1)
            return node->children[X0Y0Z0];
        if (this->position == X0Y1Z1)
            return node->children[X0Y1Z0];
        if (this->position == X1Y0Z1)
            return node->children[X1Y0Z0];
        if (this->position == X1Y1Z1)
            return node->children[X1Y1Z0];
        break;
    default:
        assert(false);
    }

    assert(false);
    return node;
}


/**
 * @brief Get neighbor leaves of the current node
 * @param direction Direction
 * @param neighbor Neighbor
 * @return Collection of neighbors
 */
template<class K, class T, class R>
std::vector<const OctreeNode<K,T,R>*> OctreeNode<K,T,R>::getNeighborLeavesHelper(
        const OctreeDirection& direction,
        const OctreeNode<K,T,R>* neighbor) const
{
    std::queue<const OctreeNode<K,T,R>*> q;
    q.push(neighbor);

    std::vector<const OctreeNode<K,T,R>*> neighbors;

    while (!q.empty()) {
        const OctreeNode<K,T,R>* c = q.front();
        q.pop();

        if (c == nullptr)
            continue;

        if (c->isLeaf()) {
            neighbors.push_back(c);
        }
        else {
            switch (direction) {
            case X0:
                q.push(c->children[X1Y0Z0]);
                q.push(c->children[X1Y0Z1]);
                q.push(c->children[X1Y1Z0]);
                q.push(c->children[X1Y1Z1]);
                break;
            case X1:
                q.push(c->children[X0Y0Z0]);
                q.push(c->children[X0Y0Z1]);
                q.push(c->children[X0Y1Z0]);
                q.push(c->children[X0Y1Z1]);
                break;
            case Y0:
                q.push(c->children[X0Y1Z0]);
                q.push(c->children[X0Y1Z1]);
                q.push(c->children[X1Y1Z0]);
                q.push(c->children[X1Y1Z1]);
                break;
            case Y1:
                q.push(c->children[X0Y0Z0]);
                q.push(c->children[X0Y0Z1]);
                q.push(c->children[X1Y0Z0]);
                q.push(c->children[X1Y0Z1]);
                break;
            case Z0:
                q.push(c->children[X0Y0Z1]);
                q.push(c->children[X0Y0Z1]);
                q.push(c->children[X1Y1Z1]);
                q.push(c->children[X1Y1Z1]);
                break;
            case Z1:
                q.push(c->children[X0Y0Z0]);
                q.push(c->children[X0Y0Z0]);
                q.push(c->children[X1Y1Z0]);
                q.push(c->children[X1Y1Z0]);
                break;
            default:
                assert(false);
            }
        }
    }

    return neighbors;
}

/**
 * @brief Initialization of the node given key and value
 *
 * @aparam key Key of the node
 * @param value Value of the node
 */
template<class K, class T, class R>
void OctreeNode<K,T,R>::init(
        const K& center,
        const R& radius)
{
    this->values.clear();

    std::fill(this->children.begin(), this->children.end(), nullptr);

    this->parent = nullptr;

    this->center = center;
    this->radius = radius;
}

}

}
