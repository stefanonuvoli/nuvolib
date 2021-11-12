#include "graph_node.h"

namespace nvl {

/**
 * @brief Default constructor
 * @param[in] value Value of the node
 * @param[in] id ID of the node in the current graph
 */
template<class T, class W>
Graph<T,W>::Node::Node() :
    id(-1), value()
{
}

/**
 * @brief Default constructor
 * @param[in] value Value of the node
 * @param[in] id If of the node in the current graph
 */
template<class T, class W>
Graph<T,W>::Node::Node(const T& value, const Index& id) :
    id(id), value(value)
{
}

}
