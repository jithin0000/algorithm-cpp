#ifndef TOPOLOGICAL_SORT
#define TOPOLOGICAL_SORT
#include "directed_graph.h"
#include <stack>
#include <vector>

/// TopologicalSort
/// Graph should be diagraph
/// it should not contains cycles
/// Edges should be precedence constraings
/// vertices are tasks
class TopologicalSort {

private:
  std::vector<bool> marked;
  std::stack<int> reversePostOrder;

public:
  TopologicalSort(const DirectedGraph &G) {
    marked.resize(G.V(), false);

    for (int i = 0; i < G.V(); i++) {
      if (!marked[i])
        dfs(G, i);
    }
  }
  void dfs(const DirectedGraph &G, int v) {

    marked[v] = true;
    for (const int &w : G.adj(v)) {
      if (!marked[w])
        dfs(G, w);
    }
    reversePostOrder.emplace(v);
  }

  // FIXME :: make it const
  std::stack<int> &order() { return reversePostOrder; }
};

#endif
