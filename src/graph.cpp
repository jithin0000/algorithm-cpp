#include "graph.h"

Graph::Graph(int V) : _V(V), _adj(V) {}
void Graph::addEdge(int v, int w) {
  _adj[v].push_back(w);
  _adj[w].push_back(v);
  _E++;
}

const std::vector<int> &Graph::adj(int v) const { return _adj[v]; }
