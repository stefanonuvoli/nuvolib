#ifndef NVL_STRUCTURES_GRAPH_NODEITERATOR_H
#define NVL_STRUCTURES_GRAPH_NODEITERATOR_H

#include <nvl/nuvolib.h>

#include "graph_genericnodeiterator.h"

#include <vector>

namespace nvl {

/**
 * @brief The node iterator of a graph
 */
template<class T, class W>
class Graph<T,W>::NodeIterator :
        public Graph<T,W>::GenericNodeIterator,
        public std::iterator<std::forward_iterator_tag, T>
{

    friend class Graph<T,W>;

public:

    /* Constructors */

    inline NodeIterator();

private:

    inline NodeIterator(
            const Graph<T,W>* graph);

    inline NodeIterator(
            const Graph<T,W>* graph,
            typename std::vector<Node>::const_iterator it);

public:

    /* Iterator operators */

    inline bool operator == (const NodeIterator& otherIterator) const;
    inline bool operator != (const NodeIterator& otherIterator) const;

    inline NodeIterator operator ++ ();
    inline NodeIterator operator ++ (int);

    inline const T& operator *() const;

private:

    /* Private methods */

    inline void next();


    /* Fields */

    typename std::vector<Node>::const_iterator it;


};


/**
 * @brief The range based iterator class for the graph
 */
template<class T, class W>
class Graph<T,W>::RangeBasedNodeIterator {

public:

    inline RangeBasedNodeIterator(const Graph<T,W>* graph) :
        graph(graph) {}

    inline NodeIterator begin();
    inline NodeIterator end();

private:

    const Graph<T,W>* graph;

};

}


#include "graph_nodeiterator.cpp"

#endif // NVL_STRUCTURES_GRAPH_NODEITERATOR_H
