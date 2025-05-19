#ifndef DIRECTED_GRAPH_H
#define DIRECTED_GRAPH_H
#include <iostream>
#include <vector>

class DirectedGraph{
private:
    int _V=0;
    int _E=0;
    std::vector<std::vector<int>> _adj;
public:
    DirectedGraph(int v):_V(v), _adj(v){}
    void addEdge(int v, int w){
    _adj[v].push_back(w);
        _E++;
    }

    int V() const {return _V;}
    int E() const {return _E;}

    const std::vector<int>& adj(int v)const {
        return _adj[v];
    }

};


#endif

