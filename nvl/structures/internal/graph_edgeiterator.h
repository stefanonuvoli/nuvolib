#ifndef NVL_STRUCTURES_GRAPH_EDGEITERATOR_H
#define NVL_STRUCTURES_GRAPH_EDGEITERATOR_H

#include <iterator>
#include <vector>
#include <utility>

#include <nvl/nuvolib.h>

#include <nvl/structures/graph.h>

namespace nvl {

/**
 * @brief The edge iterator of a graph
 */
template<class T, class W>
class Graph<T,W>::EdgeIterator :
        public std::iterator<
            std::forward_iterator_tag,
            std::pair<const T&, const T&>>
{

    friend class Graph<T,W>;

public:

    /* Constructors */

    inline EdgeIterator();

private:

    inline EdgeIterator(
            const Graph<T,W>* graph);

    inline EdgeIterator(
            const Graph<T,W>* graph,
            const typename Graph<T,W>::NodeIterator& nodeIt,
            const typename Graph<T,W>::AdjacentIterator& adjIt);

public:

    /* Iterator operators */

    inline bool operator == (const EdgeIterator& otherIterator) const;
    inline bool operator != (const EdgeIterator& otherIterator) const;

    inline EdgeIterator operator ++ ();
    inline EdgeIterator operator ++ (int);

    inline std::pair<const T, const T> operator *() const;

private:

    /* Private methods */

    inline void next();


    /* Fields */

    const Graph<T,W>* graph;

    typename Graph<T,W>::NodeIterator nodeIt;
    typename Graph<T,W>::AdjacentIterator adjIt;

};


/**
 * @brief The range based iterator class for the graph
 */
template<class T, class W>
class Graph<T,W>::RangeBasedEdgeIterator {

public:

    inline RangeBasedEdgeIterator(const Graph<T,W>* graph) :
        graph(graph) {}

    inline EdgeIterator begin();
    inline EdgeIterator end();

private:

    const Graph<T,W>* graph;

};

}


#include "graph_edgeiterator.cpp"

#endif // NVL_STRUCTURES_GRAPH_EDGEITERATOR_H
