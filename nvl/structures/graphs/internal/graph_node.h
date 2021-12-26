/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_STRUCTURES_GRAPH_NODE_H
#define NVL_STRUCTURES_GRAPH_NODE_H

#include <nvl/nuvolib.h>

#include <nvl/structures/graphs/graph.h>

#include <unordered_map>

namespace nvl {

/**
 * @brief The node of a graph
 */
template<class T, class W>
class Graph<T,W>::Node
{
public:
    friend class Graph<T,W>;
    friend class Graph<T,W>::NodeIterator;

    inline Node();


private:

    /* Constructors */

    inline Node(const T& value, const Index& id);


    /* Fields */

    Index id;

    T value;

    std::unordered_map<Index, double> adjacentNodes;
};

}

#include "graph_node.cpp"

#endif // NVL_STRUCTURES_GRAPH_NODE_H
