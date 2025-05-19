#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <stdexcept>
#include <iostream>

class Graph {
private:
    int _V;
    int _E;
    std::vector<std::vector<int>> _adj;

public:
    Graph(int V);
    void addEdge(int v, int w);
    const std::vector<int>& adj(int v) const;
    int V() const {return _V;}
    int E() const {return _E;}
};

#endif // GRAPH_H
