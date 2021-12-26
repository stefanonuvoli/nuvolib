/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_DIJKSTRA_H
#define NVL_DIJKSTRA_H

#include <map>
#include <list>

#include <nvl/structures/graphs/graph.h>

namespace nvl {

/* General purpose indexed implementation */

template<class G, class I>
void dijkstra(
        const G& graph,
        const std::vector<I>& nodes,
        const std::vector<std::vector<Index>>& nodeAdjacencies,
        const Index sourceId,
        std::vector<double>& dist,
        std::vector<long long int>& pred);

/* Implementation for Graph */

/**
 * @brief Single graph path representation
 */
template<class T, class W = double>
struct GraphPath {
    std::list<T> path;
    W cost;
};

/**
 * @brief Path from a given source to each node of the graph
 */
template<class T, class W = double>
using DijkstraResult = std::map<T, GraphPath<T,W>>;


template<class T, class W>
DijkstraResult<T,W> dijkstra(
        const Graph<T,W>& graph,
        const T& source);

template<class T, class W>
DijkstraResult<T,W> dijkstra(
        const Graph<T,W>& graph,
        const T& source,
        const std::vector<Index>& nodes,
        const std::vector<std::vector<Index>>& nodeAdjacencies,
        const std::unordered_map<Index, Index>& idMap);


template<class T, class W>
DijkstraResult<T,W> dijkstra(
        const Graph<T,W>& graph,
        const typename Graph<T,W>::iterator& sourceIt);


template<class T, class W>
DijkstraResult<T,W> dijkstra(
        const Graph<T,W>& graph,
        const typename Graph<T,W>::iterator& sourceIt,
        const std::vector<Index>& nodes,
        const std::vector<std::vector<Index>>& nodeAdjacencies,
        const std::unordered_map<Index, Index>& idMap);



template<class T, class W>
GraphPath<T,W> dijkstra(
        const Graph<T,W>& graph,
        const T& source,
        const T& destination);

template<class T, class W>
GraphPath<T,W> dijkstra(
        const Graph<T,W>& graph,
        const T& source,
        const T& destination,
        const std::vector<Index>& nodes,
        const std::vector<std::vector<Index>>& nodeAdjacencies,
        const std::unordered_map<Index, Index>& idMap);

template<class T, class W>
GraphPath<T,W> dijkstra(
        const Graph<T,W>& graph,
        const typename Graph<T,W>::iterator& sourceIt,
        const typename Graph<T,W>::iterator& destinationIt);

template<class T, class W>
GraphPath<T,W> dijkstra(
        const Graph<T,W>& graph,
        const typename Graph<T,W>::iterator& sourceIt,
        const typename Graph<T,W>::iterator& destinationIt,
        const std::vector<Index>& nodes,
        const std::vector<std::vector<Index>>& nodeAdjacencies,
        const std::unordered_map<Index, Index>& idMap);


template<class T, class W>
void dijkstraFillIndexedData(
        const Graph<T,W>& graph,
        std::vector<Index>& nodes,
        std::vector<std::vector<Index>>& nodeAdjacencies,
        std::unordered_map<Index, Index>& idMap);

} //namespace nvl

#include "dijkstra.cpp"

#endif // NVL_DIJKSTRA_H
