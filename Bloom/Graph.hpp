#pragma once
#include <list>
#include <iostream>
#include "Neuron.hpp"

// A directed graph using
// adjacency list representation
class Graph {
	int V; // No. of vertices in graph
    // Pointer to a map containing adjacency lists
    std::map<NEURON_TYPES, connection*> adj;

	// A recursive function used by printAllPaths()
	void printAllPathsUtil(NeuronPrefab &start, NeuronPrefab &end, std::map<NEURON_TYPES, bool> &visited, std::vector<connection*> &path, int &PathIndex );

public:
	Graph(int totalVertices); // Constructor
	void addConnection(NeuronPrefab *base, NeuronPrefab*connection , std::string connectionWeight);
	void printAllPaths(int s, int d);
};

Graph::Graph(int V)
{
    this->V = V;
    //this->adj = new std::map<NEURON_TYPES, connection*>{};
}

void Graph::addConnection(NeuronPrefab * neuronA, NeuronPrefab* neuronB, std::string connectionWeight)
{
    connection* newEdge = new connection(neuronB, atof(connectionWeight.c_str()));
    neuronA->connections.push_back(newEdge);
    adj[neuronA->type] = newEdge; // Add neuronB to neuronA’s list.
}

// Prints all paths from 's' to 'd'
void Graph::printAllPaths(NeuronPrefab *start, NeuronPrefab *dest)
{
    // Mark all the vertices as not visited
    std::map<NEURON_TYPES, bool> visited;

    // Create a vector to store paths
    //int* path = new int[V];
    std::vector<connection*> path();

    int path_index = 0; // Initialize path[] as empty

    // Initialize all vertices as not visited
    for (int i = 0; i < V; i++)
        visited[i] = false;

    // Call the recursive helper function to print all paths
    printAllPathsUtil(start,dest, &visited, path, path_index);
}

// A recursive function to print all paths from 'u' to 'd'.
// visited[] keeps track of vertices in current path.
// path[] stores actual vertices and path_index is current
// index in path[]
void Graph::printAllPathsUtil(NeuronPrefab &u, NeuronPrefab &dest, std::map<NEURON_TYPES, bool> &visited, std::vector<connection*> &path, int& path_index)
{
    // Mark the current node and store it in path[]
    visited[u.type] = true;
    path[path_index] = u;
    path_index++;

    // If current vertex is same as destination, then print
    // current path[]
    if (u == d) {
        for (int i = 0; i < path_index; i++)
            std::cout << path[i] << " ";
        std::cout << std::endl;
    }
    else // If current vertex is not destination
    {
        // Recur for all the vertices adjacent to current vertex
       std::list<int>::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
            if (!visited[*i])
                printAllPathsUtil(*i, d, visited, path, path_index);
    }

    // Remove current vertex from path[] and mark it as unvisited
    path_index--;
    visited[u] = false;
}

//// Driver program
//int main()
//{
//    // Create a graph given in the above diagram
//    Graph g(4);
//    g.addEdge(0, 1);
//    g.addEdge(0, 2);
//    g.addEdge(0, 3);
//    g.addEdge(2, 0);
//    g.addEdge(2, 1);
//    g.addEdge(1, 3);
//
//    int s = 2, d = 3;
//    std::cout << "Following are all different paths from " << s << " to " << d << std::endl;
//    g.printAllPaths(s, d);
//
//    return 0;
//}