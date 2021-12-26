/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "graph_edgeiterator.h"

namespace nvl {


/* ----- CONSTRUCTORS ----- */

template<class T, class W>
Graph<T,W>::EdgeIterator::EdgeIterator() :
    graph(nullptr),
    nodeIt(NodeIterator()),
    adjIt(AdjacentIterator())
{

}

template<class T, class W>
Graph<T,W>::EdgeIterator::EdgeIterator(
        const Graph<T,W>* graph) :
    graph(graph),
    nodeIt(graph->nodeIteratorEnd()),
    adjIt(graph->adjacentNodeIteratorEnd(graph->nodeIteratorEnd()))
{

}

template<class T, class W>
Graph<T,W>::EdgeIterator::EdgeIterator(
        const Graph<T,W>* graph,
        const typename Graph<T,W>::NodeIterator& nodeIt,
        const typename Graph<T,W>::AdjacentIterator& adjIt) :
    graph(graph),
    nodeIt(nodeIt),
    adjIt(adjIt)
{

}


/* ----- OPERATOR OVERLOAD ----- */


template<class T, class W>
bool Graph<T,W>::EdgeIterator::operator ==(
        const EdgeIterator& otherIterator) const
{
    return (this->graph == otherIterator.graph &&
            this->nodeIt == otherIterator.nodeIt &&
            this->adjIt == otherIterator.adjIt);
}

template<class T, class W>
bool Graph<T,W>::EdgeIterator::operator !=(const EdgeIterator& otherIterator) const
{
    return !(*this == otherIterator);
}



template<class T, class W>
typename Graph<T,W>::EdgeIterator Graph<T,W>::EdgeIterator::operator ++()
{
    this->next();
    return *this;
}

template<class T, class W>
typename Graph<T,W>::EdgeIterator Graph<T,W>::EdgeIterator::operator ++(int)
{
    EdgeIterator oldIt = *this;
    this->next();
    return oldIt;
}



template<class T, class W>
std::pair<const T, const T> Graph<T,W>::EdgeIterator::operator *() const
{
    return std::make_pair(*nodeIt, *adjIt);
}



/* ----- PROTECTED METHODS FOR NAVIGATION ----- */

template<class T, class W>
void Graph<T,W>::EdgeIterator::next()
{
    adjIt++;
    this->graph->getFirstValidIteratorEdge(nodeIt, adjIt, nodeIt, adjIt);
}



/* --------- RANGE BASED ITERATOR --------- */

template<class T, class W>
typename Graph<T,W>::EdgeIterator Graph<T,W>::RangeBasedEdgeIterator::begin()
{
    return this->graph->edgeBegin();
}

template<class T, class W>
typename Graph<T,W>::EdgeIterator Graph<T,W>::RangeBasedEdgeIterator::end()
{
    return this->graph->edgeEnd();
}


}
