/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include <queue>
#include <utility>
#include <unordered_map>

#include "assert.h"

#include "dijkstra.h"

namespace nvl {



/** ----- GENERAL PURPOSE INDEXED IMPLEMENTATION ----- */

/**
 * @brief General porpouse indexed Dijkstra algorithm. The current implementation has
 * time complexity O(|E| + |V| log |V|).
 * @param graph Input graph. It is a templated type that must implement:
 * - getNode(id) that returns the node (or an iterator) given an id;
 * - getWeight(node1, node2) that takes the return type of getNode and returns
 *   the weight of the two nodes;
 * - A constant double MAX_WEIGHT which represent the max weight in the graph.
 * @param nodes List of ids of the input nodes of the graph
 * @param nodeAdjacencies Indexed adjacencies of each graph node (referring to the
 * indices of the vector "nodes")
 * @param sourceId Id of the source (referring to the indices of the vector "nodes")
 * @param dist Vector of shortest path costs from the source to each node
 * @param pred Vector for predecessors to compute the path
 */
template<class G, class I>
void dijkstra(
        const G& graph,
        const std::vector<I>& nodes,
        const std::vector<std::vector<Index>>& nodeAdjacencies,
        const Index sourceId,
        std::vector<double>& dist,
        std::vector<long long int>& pred)
{
    typedef std::pair<double, Index> QueueObject;

    Index numberOfNodes = nodes.size();

    dist.resize(numberOfNodes, G::MAX_WEIGHT);
    pred.resize(numberOfNodes, -1);

    dist[sourceId] = 0;
    pred[sourceId] = (long long int) sourceId;

    //Priority queue
    std::priority_queue<QueueObject, std::vector<QueueObject>, std::greater<QueueObject>> queue;

    queue.push(std::make_pair(0, sourceId));

    while (!queue.empty()) {
        //Get iterator on the queue and pop it from it
        const QueueObject& qObject = queue.top();
        Index uId = qObject.second;

        queue.pop();

        //For each adjacent node
        for (const Index& vId : nodeAdjacencies[uId]) {
            //Get weight
            double weight = graph.getWeight(
                        graph.getNode(nodes[uId]),
                        graph.getNode(nodes[vId]));

            assert(dist[uId] < G::MAX_WEIGHT);

            //If there is short path to v through u.
            if (dist[vId] > dist[uId] + weight)
            {
                //Update distance of v
                dist[vId] = dist[uId] + weight;

                //Set predecessor
                pred[vId] = (long long int) uId;

                //Add to the queue
                queue.push(std::make_pair(dist[vId], vId));
            }
        }
    }
}




/* ----- IMPLEMENTATION FOR Graph ----- */


namespace internal {

template<class T, class W>
GraphPath<T,W> getGraphShortestPath(
        const Graph<T,W>& graph,
        const typename Graph<T,W>::iterator sourceIt,
        const std::vector<Index>& nodes,
        const Index& sourceId,
        const Index& destinationId,
        const std::vector<double>& dist,
        const std::vector<long long int>& pred);

} //namespace internal




/**
 * @brief Execute Dijkstra algorithm given a nvl graph and the source. It
 * computes the shortest path between the source and the input nodes of the graph.
 * @param graph Input nvl graph.
 * @param source Source node value
 * @return A map that associates all the graph nodes to the shortest path from the source
 * to that node. It contains structs which contain the path (a list of nodes) and a double
 * value which represents the cost of the path. The map is implemented using the std::map,
 * hence the informations can be after retrieved in O(log |V|) complexity time.
 */
template<class T, class W>
inline DijkstraResult<T,W> dijkstra(const Graph<T,W>& graph, const T& source)
{
    typedef typename Graph<T,W>::iterator NodeIterator;

    //Search source in the graph
    NodeIterator sourceIt = graph.findNode(source);
    if (sourceIt == graph.end())
        throw std::runtime_error("Source has not been found in the graph.");

    return dijkstra(graph, sourceIt);
}

/**
 * @brief Execute Dijkstra algorithm given a nvl graph and the source. It
 * computes the shortest path between the source and the input nodes of the graph.
 * @param graph Input nvl graph.
 * @param source Source node value
 * @return A map that associates all the graph nodes to the shortest path from the source
 * to that node. It contains structs which contain the path (a list of nodes) and a double
 * value which represents the cost of the path. The map is implemented using the std::map,
 * hence the informations can be after retrieved in O(log |V|) complexity time.
 */
template<class T, class W>
DijkstraResult<T,W> dijkstra(
        const Graph<T,W>& graph,
        const T& source,
        const std::vector<Index>& nodes,
        const std::vector<std::vector<Index>>& nodeAdjacencies,
        const std::unordered_map<Index, Index>& idMap)
{
    typedef typename Graph<T,W>::iterator NodeIterator;

    //Search source in the graph
    NodeIterator sourceIt = graph.findNode(source);
    if (sourceIt == graph.end())
        throw std::runtime_error("Source has not been found in the graph.");

    return dijkstra(graph, sourceIt, nodes, nodeAdjacencies, idMap);
}

/**
 * @brief Execute Dijkstra algorithm given a nvl graph and the source. It
 * computes the shortest path between the source and the input nodes of the graph.
 * @param graph Input nvl graph.
 * @param sourceIt Source node iterator
 * @param nodes List of ids of the input nodes of the graph
 * @param nodeAdjacencies Indexed adjacencies of each graph node (referring to the
 * indices of the vector "nodes")
 * @param idMap Map to get the index of a node (referring to the indices of the vector
 * "nodes") given the id on the input nvl graph
 * @return A map that associates all the graph nodes to the shortest path from the source
 * to that node. It contains structs which contain the path (a list of nodes) and a double
 * value which represents the cost of the path. The map is implemented using the std::map,
 * hence the informations can be after retrieved in O(log |V|) complexity time.
 */
template<class T, class W>
DijkstraResult<T,W> dijkstra(
        const Graph<T,W>& graph,
        const typename Graph<T,W>::iterator& sourceIt)
{
    //Vector of nodes and adjacencies
    std::vector<Index> nodes;
    std::vector<std::vector<Index>> nodeAdjacencies;
    std::unordered_map<Index, Index> idMap;

    //Fill indexed data needed to execute general purpose Dijkstra algorithm
    dijkstraFillIndexedData(graph, nodes, nodeAdjacencies, idMap);

    return dijkstra(graph, sourceIt, nodes, nodeAdjacencies, idMap);
}

/**
 * @brief Execute Dijkstra algorithm given a nvl graph and the source. It
 * computes the shortest path between the source and the input nodes of the graph.
 * @param graph Input nvl graph.
 * @param sourceIt Source node iterator
 * @param nodes List of ids of the input nodes of the graph
 * @param nodeAdjacencies Indexed adjacencies of each graph node (referring to the
 * indices of the vector "nodes")
 * @param idMap Map to get the index of a node (referring to the indices of the vector
 * "nodes") given the id on the input nvl graph
 * @return A map that associates all the graph nodes to the shortest path from the source
 * to that node. It contains structs which contain the path (a list of nodes) and a double
 * value which represents the cost of the path. The map is implemented using the std::map,
 * hence the informations can be after retrieved in O(log |V|) complexity time.
 */
template<class T, class W>
DijkstraResult<T,W> dijkstra(
        const Graph<T,W>& graph,
        const typename Graph<T,W>::iterator& sourceIt,
        const std::vector<Index>& nodes,
        const std::vector<std::vector<Index>>& nodeAdjacencies,
        const std::unordered_map<Index, Index>& idMap)
{
    typedef typename Graph<T,W>::iterator NodeIterator;

    //Vector of distances and predecessor of the shortest path from the source
    std::vector<double> dist;
    std::vector<long long int> pred;

    //Id of the source
    Index sourceId = idMap.find(graph.getId(sourceIt))->second;


    //Execute Dijkstra
    dijkstra(graph, nodes, nodeAdjacencies, sourceId, dist, pred);



    //Result to be returned
    DijkstraResult<T,W> resultMap;

    //Update result map for each destination
    Index destinationId = 0;
    for (const Index& nodeId : nodes) {
        //If there is a path
        if (pred[destinationId] != -1) {
            //Return the resulting shortest path
            GraphPath<T,W> graphPath = internal::getGraphShortestPath(graph, sourceIt, nodes, sourceId, destinationId, dist, pred);

            //Fill map
            NodeIterator nodeIt = graph.getNode(nodeId);
            resultMap.insert(std::make_pair(*nodeIt, graphPath));
        }

        destinationId++;
    }


    return resultMap;
}






/**
 * @brief Execute Dijkstra algorithm to get the shortest path from the source
 * to the destination, given a nvl graph.
 * @param graph Input nvl graph.
 * @param source Source node value
 * @param destination Destination node value
 * @return A struct which contains the shortest path and its cost.
 * If no path exists, then an empty path of MAX_WEIGHT cost is returned.
 * The data can be retrieved in constant time.
 */
template<class T, class W>
inline GraphPath<T,W> dijkstra(
        const Graph<T,W>& graph,
        const T& source,
        const T& destination)
{
    typedef typename Graph<T,W>::iterator NodeIterator;

    //Search source in the graph
    NodeIterator sourceIt = graph.findNode(source);
    if (sourceIt == graph.end())
        throw std::runtime_error("Source has not been found in the graph.");

    //Search destination in the graph
    NodeIterator destinationIt = graph.findNode(destination);
    if (destinationIt == graph.end())
        throw std::runtime_error("Destination has not been found in the graph.");

    return dijkstra(graph, sourceIt, destinationIt);
}

/**
 * @brief Execute Dijkstra algorithm to get the shortest path from the source
 * to the destination, given a nvl graph.
 * @param graph Input nvl graph.
 * @param source Source node value
 * @param destination Destination node value
 * @param nodes List of ids of the input nodes of the graph
 * @param nodeAdjacencies Indexed adjacencies of each graph node (referring to the
 * indices of the vector "nodes")
 * @param idMap Map to get the index of a node (referring to the indices of the vector
 * "nodes") given the id on the input nvl graph
 * @return A struct which contains the shortest path and its cost.
 * If no path exists, then an empty path of MAX_WEIGHT cost is returned.
 * The data can be retrieved in constant time.
 */
template<class T, class W>
GraphPath<T,W> dijkstra(
        const Graph<T,W>& graph,
        const T& source,
        const T& destination,
        const std::vector<Index>& nodes,
        const std::vector<std::vector<Index>>& nodeAdjacencies,
        const std::unordered_map<Index, Index>& idMap)
{
    typedef typename Graph<T,W>::iterator NodeIterator;

    //Search source in the graph
    NodeIterator sourceIt = graph.findNode(source);
    if (sourceIt == graph.end())
        throw std::runtime_error("Source has not been found in the graph.");

    //Search destination in the graph
    NodeIterator destinationIt = graph.findNode(destination);
    if (destinationIt == graph.end())
        throw std::runtime_error("Destination has not been found in the graph.");

    return dijkstra(graph, sourceIt, destinationIt, nodes, nodeAdjacencies, idMap);
}

/**
 * @brief Execute Dijkstra algorithm to get the shortest path from the source
 * to the destination, given a nvl graph.
 * @param graph Input nvl graph.
 * @param sourceIt Source node iterator
 * @param destinationIt Destination node iterator
 * @return A struct which contains the shortest path and its cost.
 * If no path exists, then an empty path of MAX_WEIGHT cost is returned.
 * The data can be retrieved in constant time.
 */
template<class T, class W>
GraphPath<T,W> dijkstra(
        const Graph<T,W>& graph,
        const typename Graph<T,W>::iterator& sourceIt,
        const typename Graph<T,W>::iterator& destinationIt)
{
    //Vector of nodes and adjacencies
    std::vector<Index> nodes;
    std::vector<std::vector<Index>> nodeAdjacencies;
    std::unordered_map<Index, Index> idMap;


    //Fill indexed data needed by general purpose Dijkstra algorithm
    dijkstraFillIndexedData(graph, nodes, nodeAdjacencies, idMap);

    return dijkstra(graph, sourceIt, destinationIt, nodes, nodeAdjacencies, idMap);
}

/**
 * @brief Execute Dijkstra algorithm to get the shortest path from the source
 * to the destination, given a nvl graph.
 * @param graph Input nvl graph.
 * @param sourceIt Source node iterator
 * @param destinationIt Destination node iterator
 * @param nodes List of ids of the input nodes of the graph
 * @param nodeAdjacencies Indexed adjacencies of each graph node (referring to the
 * indices of the vector "nodes")
 * @param idMap Map to get the index of a node (referring to the indices of the vector
 * "nodes") given the id on the input nvl graph
 * @return A struct which contains the shortest path and its cost.
 * If no path exists, then an empty path of MAX_WEIGHT cost is returned.
 * The data can be retrieved in constant time.
 */
template<class T, class W>
GraphPath<T,W> dijkstra(
        const Graph<T,W>& graph,
        const typename Graph<T,W>::iterator& sourceIt,
        const typename Graph<T,W>::iterator& destinationIt,
        const std::vector<Index>& nodes,
        const std::vector<std::vector<Index>>& nodeAdjacencies,
        const std::unordered_map<Index, Index>& idMap)
{
    //Vector of distances and predecessor of the shortest path from the source
    std::vector<double> dist;
    std::vector<long long int> pred;

    //Id of the source and destination
    Index sourceId = idMap.find(graph.getId(sourceIt))->second;
    Index destinationId = idMap.find(graph.getId(destinationIt))->second;

    //Execute Dijkstra
    dijkstra(graph, nodes, nodeAdjacencies, sourceId, dist, pred);

    return internal::getGraphShortestPath(graph, sourceIt, nodes, sourceId, destinationId, dist, pred);
}



/**
 * @brief Fill indexed data structure needed by the Dijkstra algorithm for a nvl graph.
 * @param graph Input nvl graph
 * @param nodes List of ids of the input nodes of the graph
 * @param nodeAdjacencies Indexed adjacencies of each graph node (referring to the
 * indices of the vector "nodes")
 * @param idMap Map to get the index of a node (referring to the indices of the vector
 * "nodes") given the id on the input nvl graph
 */
template<class T, class W>
inline void dijkstraFillIndexedData(
        const Graph<T,W>& graph,
        std::vector<Index>& nodes,
        std::vector<std::vector<Index>>& nodeAdjacencies,
        std::unordered_map<Index, Index>& idMap)
{
    typedef typename Graph<T,W>::iterator NodeIterator;
    typedef typename Graph<T,W>::AdjacentIterator AdjacentIterator;

    Index id;

    //Fill vector of iterators for the nodes
    id = 0;

    //Iterate on nodes of the graph
    for (NodeIterator it = graph.begin(); it != graph.end(); ++it) {
        Index graphId = graph.getId(it);

        nodes.push_back(graphId);

        idMap.insert(std::make_pair(graphId, id));

        id++;
    }

    //Fill vector of adjacencies
    nodeAdjacencies.resize(nodes.size());

    id = 0;
    for (const Index& graphId : nodes) {
        NodeIterator nodeIt = graph.getNode(graphId);

        std::vector<Index>& adjList = nodeAdjacencies[id];

        //For each adjacent node
        for (AdjacentIterator adjNodeIt = graph.adjacentBegin(nodeIt); adjNodeIt != graph.adjacentEnd(nodeIt); adjNodeIt++) {
            typename std::unordered_map<Index, Index>::iterator findIdIt = idMap.find(graph.getId(adjNodeIt));
            Index adjId = findIdIt->second;

            adjList.push_back(adjId);
        }

        id++;
    }
}


namespace internal {

/**
 * @brief Get the resulting shortest path in the nvl graph, given the raw Dijkstra data,
 * given a source and a destination
 * @param graph Input nvl graph
 * @param sourceIt nvl graph iterator of the source node.
 * @param nodes List of ids of the input nodes of the graph
 * @param sourceId Id of the source (referring to the indices of the vector "nodes")
 * @param destinationId Id of the destination (referring to the indices of the vector "nodes")
 * @param dist Vector of shortest path costs from the source to each node
 * @param pred Vector for predecessors to compute the path
 * @return Shortest path between source and destination
 */
template<class T, class W>
inline GraphPath<T,W> getGraphShortestPath(
        const Graph<T,W>& graph,
        const typename Graph<T,W>::iterator sourceIt,
        const std::vector<Index>& nodes,
        const Index& sourceId,
        const Index& destinationId,
        const std::vector<double>& dist,
        const std::vector<long long int>& pred)
{
    typedef typename Graph<T,W>::iterator NodeIterator;

    //Result graph path
    GraphPath<T,W> graphPath;

    //Get the shortest path
    if (pred[destinationId] != -1) {
        //Create path
        Index idPred = destinationId;
        while (idPred != sourceId) {
            NodeIterator predIterator = graph.getNode(nodes[idPred]);

            graphPath.path.push_front(*predIterator);

            assert(pred[idPred] >= 0);

            idPred = (Index) pred[idPred];
        }

        graphPath.path.push_front(*sourceIt);
    }

    graphPath.cost = dist[destinationId];

    return graphPath;
}


} //namespace internal

} //namespace nvl
