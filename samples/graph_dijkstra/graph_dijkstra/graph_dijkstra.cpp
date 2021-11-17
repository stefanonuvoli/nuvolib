#include <iostream>

#include <vector>

#include <nvl/structures/graph.h>
#include <nvl/structures/dijkstra.h>

int main() {
    std::cout << std::endl << " >> DIJKSTRA" << std::endl << std::endl;

    std::cout << "Create a graph of 10 nodes with some edges..." << std::endl;

    nvl::Graph<int> g(nvl::Graph<int>::UNDIRECTED);

    //Add nodes
    g.addNode(0);
    g.addNode(1);
    g.addNode(2);
    g.addNode(3);
    g.addNode(4);
    g.addNode(5);
    g.addNode(6);
    g.addNode(7);
    g.addNode(8);
    g.addNode(9);

    //Add edges
    g.addEdge(0, 1, 4);
    g.addEdge(0, 7, 8);
    g.addEdge(1, 2, 8);
    g.addEdge(1, 7, 11);
    g.addEdge(2, 3, 7);
    g.addEdge(2, 8, 2);
    g.addEdge(2, 5, 4);
    g.addEdge(3, 4, 9);
    g.addEdge(3, 5, 14);
    g.addEdge(4, 5, 10);
    g.addEdge(5, 6, 2);
    g.addEdge(6, 7, 1);
    g.addEdge(6, 8, 6);
    g.addEdge(7, 8, 7);

    std::cout << std::endl;


    //Execute dijkstra
    std::cout << "Execute Dijkstra algorithm (source is 0)..." << std::endl;
    nvl::DijkstraResult<int> pathMap0 = nvl::dijkstra(g, 0);

    std::cout << std::endl;

    for (const int& node : g) {
        nvl::DijkstraResult<int>::iterator findIt = pathMap0.find(node);

        std::cout << "0 to " << node << "\t->\t";

        if (findIt != pathMap0.end()) {
            nvl::GraphPath<int>& graphPath = findIt->second;

            std::cout << "Cost: " << graphPath.cost << "\tPath: ";

            for (int pathNode : graphPath.path) {
                std::cout << pathNode << " ";
            }
        }
        else {
            std::cout << "No path found!";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    //Execute dijkstra using source iterator (we use std::map<int, nvl::GraphPath<int>>
    //instead of DijkstraResult<int>)
    std::cout << "Execute Dijkstra algorithm (source is 2) using iterators..." << std::endl;
    nvl::Graph<int>::iterator it2 = g.findNode(2);
    std::map<int, nvl::GraphPath<int>> pathMap1 = nvl::dijkstra(g, it2);

    std::cout << std::endl;

    for (const int& node : g.nodeIterator()) {
        std::map<int, nvl::GraphPath<int>>::iterator findIt = pathMap1.find(node);

        std::cout << *it2 << " to " << node << "\t->\t";

        if (findIt != pathMap1.end()) {
            nvl::GraphPath<int>& graphPath = findIt->second;

            std::cout << "Cost: " << graphPath.cost << "\tPath: ";

            for (int pathNode : graphPath.path) {
                std::cout << pathNode << " ";
            }
        }
        else {
            std::cout << "No path found!";
        }
        std::cout << std::endl;
    }


    std::cout << std::endl;

    //Execute dijkstra to get the shortest path from source to destination.
    //We do not need a map
    nvl::GraphPath<int> path14 = nvl::dijkstra(g, 1, 4);
    std::cout << "Single path: \t 1 to 4 -> ";
    if (!path14.path.empty()) {
        std::cout << "Cost: " << path14.cost << "\tPath: ";

        for (int pathNode : path14.path) {
            std::cout << pathNode << " ";
        }
    }
    else {
        std::cout << "No path found!";
    }

    std::cout << std::endl;

    //Execute dijkstra to get the shortest path from source to destination.
    //We do not need a map
    nvl::GraphPath<int> path29 = nvl::dijkstra(g, 2, 9);
    std::cout << "Single path: \t 2 to 9 -> ";
    if (!path29.path.empty()) {
        std::cout << "Cost: " << path29.cost << "\tPath: ";

        for (int pathNode : path29.path) {
            std::cout << pathNode << " ";
        }
    }
    else {
        std::cout << "No path found!";
    }

    std::cout << std::endl;

    //Execute dijkstra to get the shortest path from source to destination using iterators
    //We do not need a map
    nvl::Graph<int>::iterator it1 = g.findNode(1);
    nvl::Graph<int>::iterator it6 = g.findNode(6);
    nvl::GraphPath<int> path16 = nvl::dijkstra(g, it1, it6);
    std::cout << "Single path (it): \t 1 to 6 -> ";
    if (!path16.path.empty()) {
        std::cout << "Cost: " << path16.cost << "\tPath: ";

        for (int pathNode : path16.path) {
            std::cout << pathNode << " ";
        }
    }
    else {
        std::cout << "No path found!";
    }

    std::cout << std::endl;
}
