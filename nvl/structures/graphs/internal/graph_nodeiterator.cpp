#include "graph_nodeiterator.h"

namespace nvl {


/* ----- CONSTRUCTORS ----- */

template<class T, class W>
Graph<T,W>::NodeIterator::NodeIterator() :
    Graph<T,W>::GenericNodeIterator(),
    it(typename std::vector<Node>::const_iterator())
{

}

template<class T, class W>
Graph<T,W>::NodeIterator::NodeIterator(
        const Graph<T,W>* graph) :
    Graph<T,W>::GenericNodeIterator(graph),
    it(typename std::vector<Node>::const_iterator())
{

}

template<class T, class W>
Graph<T,W>::NodeIterator::NodeIterator(
        const Graph<T,W>* graph,
        typename std::vector<Node>::const_iterator it) :
    Graph<T,W>::GenericNodeIterator(graph),
    it(it)
{
    if (it != this->graph->nodes.end())
        this->id = it->id;
}


/* ----- OPERATOR OVERLOAD ----- */


template<class T, class W>
bool Graph<T,W>::NodeIterator::operator ==(
        const NodeIterator& otherIterator) const
{
    return (this->graph == otherIterator.graph &&
            this->id == otherIterator.id &&
            it == otherIterator.it);
}

template<class T, class W>
bool Graph<T,W>::NodeIterator::operator !=(const NodeIterator& otherIterator) const
{
    return !(*this == otherIterator);
}



template<class T, class W>
typename Graph<T,W>::NodeIterator Graph<T,W>::NodeIterator::operator ++()
{
    next();
    return *this;
}

template<class T, class W>
typename Graph<T,W>::NodeIterator Graph<T,W>::NodeIterator::operator ++(int)
{
    NodeIterator oldIt = *this;
    next();
    return oldIt;
}



template<class T, class W>
const T& Graph<T,W>::NodeIterator::operator *() const
{
    return it->value;
}



/* ----- PROTECTED METHODS FOR NAVIGATION ----- */

template<class T, class W>
void Graph<T,W>::NodeIterator::next()
{
    ++it;
    it = this->graph->getFirstValidIteratorNode(it);

    if (it != this->graph->nodes.end())
        this->id = it->id;
    else
        this->id = -1;
}



/* --------- RANGE BASED ITERATOR --------- */

template<class T, class W>
typename Graph<T,W>::NodeIterator Graph<T,W>::RangeBasedNodeIterator::begin()
{
    return this->graph->nodeBegin();
}

template<class T, class W>
typename Graph<T,W>::NodeIterator Graph<T,W>::RangeBasedNodeIterator::end()
{
    return this->graph->nodeEnd();
}


}
