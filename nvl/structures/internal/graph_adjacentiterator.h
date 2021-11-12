#ifndef NVL_STRUCTURES_GRAPH_ADJACENTITERATOR_H
#define NVL_STRUCTURES_GRAPH_ADJACENTITERATOR_H

#include <nvl/nuvolib.h>

#include "graph_genericnodeiterator.h"

namespace nvl {

/**
 * @brief The iterator of a graph
 */
template<class T, class W>
class Graph<T,W>::AdjacentIterator :
        public Graph<T,W>::GenericNodeIterator,
        public std::iterator<std::forward_iterator_tag, T>
{

    friend class Graph<T,W>;

public:

    /* Constructors */

    inline AdjacentIterator();

private:

    inline AdjacentIterator(
            const Graph<T,W>* graph);

    inline AdjacentIterator(
            const Graph<T,W>* graph,
            const NodeIterator& targetNodeIt,
            typename std::unordered_map<Index, W>::const_iterator it);
public:

    /* Iterator operators */

    inline bool operator == (const AdjacentIterator& otherIterator) const;
    inline bool operator != (const AdjacentIterator& otherIterator) const;

    inline AdjacentIterator operator ++ ();
    inline AdjacentIterator operator ++ (int);

    inline const T& operator *() const;

private:

    /* Private methods */

    inline void next();


    /* Fields */

    NodeIterator targetNodeIt;
    typename std::unordered_map<Index, W>::const_iterator it;

};


/**
 * @brief The range based iterator class for the graph
 */
template<class T, class W>
class Graph<T,W>::RangeBasedAdjacentIterator {

public:

    inline RangeBasedAdjacentIterator(
            const Graph<T,W>* graph,
            const NodeIterator& targetNodeIt) :
        graph(graph), targetNodeIt(targetNodeIt) {}

    inline AdjacentIterator begin();
    inline AdjacentIterator end();

private:

    const Graph<T,W>* graph;
    NodeIterator targetNodeIt;

};

}


#include "graph_adjacentiterator.cpp"

#endif // NVL_STRUCTURES_GRAPH_ADJACENTITERATOR_H
