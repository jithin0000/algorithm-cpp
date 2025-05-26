#ifndef DIRECTED_GRAPH_H
#define DIRECTED_GRAPH_H
#include <iostream>
#include <vector>

class DirectedGraph {
private:
  int _V = 0;
  int _E = 0;
  std::vector<std::vector<int>> _adj;

public:
  DirectedGraph() {};
  DirectedGraph(int v) : _V(v), _adj(v) {}
  void addEdge(int v, int w) {
    _adj[v].push_back(w);
    _E++;
  }

  int V() const { return _V; }
  int E() const { return _E; }

   DirectedGraph reverse() const {
    DirectedGraph G(V());
    for (int v = 0; v < V(); v++) {
      for (const int w : adj(v)) {
        G.addEdge(w, v);
      }
    }
    return G;
  }

  const std::vector<int> &adj(int v) const { return _adj[v]; }
  friend std::ostream &operator<<(std::ostream &os, const DirectedGraph &G) {
    os << "{ \n";
    for (int i = 0; i < G.V(); i++) {
      os << "\t" << i << ":";
      for (int w : G.adj(i)) {
        os << " -> " << w;
      }
      os << "\n";
    }
    os << "}";

    return os;
  }
};

#endif
