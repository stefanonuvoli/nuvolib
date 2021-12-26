#include "graph_genericnodeiterator.h"

namespace nvl {


/* ----- CONSTRUCTORS ----- */

template<class T, class W>
Graph<T,W>::GenericNodeIterator::GenericNodeIterator() :
    GenericNodeIterator(nullptr, -1)
{

}


template<class T, class W>
Graph<T,W>::GenericNodeIterator::GenericNodeIterator(
        const Graph<T,W>* graph) :
    GenericNodeIterator(graph, -1)
{

}


template<class T, class W>
Graph<T,W>::GenericNodeIterator::GenericNodeIterator(
        const Graph<T,W>* graph,
        long long int id) :
    graph(graph),
    id(id)
{

}



}
