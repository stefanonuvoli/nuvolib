#ifndef NVL_STRUCTURES_GRAPH_GENERICNODEITERATOR_H
#define NVL_STRUCTURES_GRAPH_GENERICNODEITERATOR_H

#include <nvl/nuvolib.h>

#include <nvl/structures/graph.h>

namespace nvl {

/**
 * @brief The node iterator of a graph
 */
template<class T, class W>
class Graph<T,W>::GenericNodeIterator
{

    friend class Graph<T,W>;

protected:

    /* Constructors */


    inline GenericNodeIterator();

    inline GenericNodeIterator(
            const Graph<T,W>* graph);

    inline GenericNodeIterator(
            const Graph<T,W>* graph,
            long long int id);


    /* Fields */

    const Graph<T,W>* graph;
    long long int id;

};


}

#include "graph_genericnodeiterator.cpp"

#endif // NVL_STRUCTURES_GRAPH_GENERICNODEITERATOR_H
