/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "graph_node.h"

namespace nvl {

/**
 * @brief Default constructor
 * @param value Value of the node
 * @param id ID of the node in the current graph
 */
template<class T, class W>
Graph<T,W>::Node::Node() :
    id(-1), value()
{
}

/**
 * @brief Default constructor
 * @param value Value of the node
 * @param id If of the node in the current graph
 */
template<class T, class W>
Graph<T,W>::Node::Node(const T& value, const Index& id) :
    id(id), value(value)
{
}

}
