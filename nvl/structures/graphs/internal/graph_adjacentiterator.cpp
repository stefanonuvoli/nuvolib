/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "graph_adjacentiterator.h"

namespace nvl {

/* ----- CONSTRUCTORS ----- */

template<class T, class W>
Graph<T,W>::AdjacentIterator::AdjacentIterator() :
    Graph<T,W>::GenericNodeIterator(),
    targetNodeIt(NodeIterator()),
    it(typename std::unordered_map<Index, W>::const_iterator())
{

}


template<class T, class W>
Graph<T,W>::AdjacentIterator::AdjacentIterator(
        const Graph<T,W>* graph) :
    Graph<T,W>::GenericNodeIterator(graph),
    targetNodeIt(this->graph->nodeEnd()),
    it(typename std::unordered_map<Index, W>::const_iterator())
{

}

template<class T, class W>
Graph<T,W>::AdjacentIterator::AdjacentIterator(
        const Graph<T,W>* graph,
        const NodeIterator& targetNodeIt,
        typename std::unordered_map<Index, W>::const_iterator it) :
    Graph<T,W>::GenericNodeIterator(graph),
    targetNodeIt(targetNodeIt),
    it(it)
{
    if (targetNodeIt != this->graph->nodeEnd() &&
            it != this->graph->nodes.at((Index) targetNodeIt.id).adjacentNodes.end())
    {
        this->id = it->first;
    }
}



/* ----- OPERATOR OVERLOAD ----- */


template<class T, class W>
bool Graph<T,W>::AdjacentIterator::operator ==(
        const AdjacentIterator& otherIterator) const
{
    return (this->graph == otherIterator.graph &&
            this->id == otherIterator.id &&
            targetNodeIt == otherIterator.targetNodeIt &&
            it == otherIterator.it);
}

template<class T, class W>
bool Graph<T,W>::AdjacentIterator::operator !=(const AdjacentIterator& otherIterator) const
{
    return !(*this == otherIterator);
}



template<class T, class W>
typename Graph<T,W>::AdjacentIterator Graph<T,W>::AdjacentIterator::operator ++()
{
    next();
    return *this;
}

template<class T, class W>
typename Graph<T,W>::AdjacentIterator Graph<T,W>::AdjacentIterator::operator ++(int)
{
    AdjacentIterator oldIt = *this;
    next();
    return oldIt;
}


template<class T, class W>
const T& Graph<T,W>::AdjacentIterator::operator *() const
{
    return this->graph->nodes.at((Index) this->id).value;
}



/* ----- PROTECTED METHODS FOR NAVIGATION ----- */

template<class T, class W>
void Graph<T,W>::AdjacentIterator::next()
{
    ++it;
    it = this->graph->getFirstValidIteratorAdjacent(targetNodeIt, it);

    if (it != this->graph->nodes.at((Index) targetNodeIt.id).adjacentNodes.end()) {
        this->id = it->first;
    }
    else {
        this->id = -1;
    }
}





/* --------- RANGE BASED ITERATOR --------- */

template<class T, class W>
typename Graph<T,W>::AdjacentIterator Graph<T,W>::RangeBasedAdjacentIterator::begin()
{
    return this->graph->adjacentBegin(targetNodeIt);
}

template<class T, class W>
typename Graph<T,W>::AdjacentIterator Graph<T,W>::RangeBasedAdjacentIterator::end()
{
    return this->graph->adjacentEnd(targetNodeIt);
}


}
