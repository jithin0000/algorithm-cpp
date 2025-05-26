#ifndef DFS_CYCLE
#define DFS_CYCLE
#include "directed_graph.h"
#include <stack>
#include <vector>

class DfsCycle {

private:
  std::vector<bool> visited;
  std::vector<bool> onStack;
  std::vector<int> edgeTo;
  std::stack<int> cycle;

public:
  DfsCycle(DirectedGraph &G) {
    onStack.resize(G.V(), false);
    visited.resize(G.V(), false);
    edgeTo.resize(G.V(), -1);

    for (int i = 0; i < G.V(); i++)
      if (!visited[i])
        dfs(G, i);
  }

  bool hasCycle() const { return !cycle.empty(); }
  const std::stack<int> getCycle() const { return cycle; }

  void dfs(const DirectedGraph &G, int v) {
    visited[v] = true;
    onStack[v] = true;

    for (const int w : G.adj(v)) {
      // check has cycle then return
      if (hasCycle())
        return;
      else if (!visited[w])
        dfs(G, w);
      else if (onStack[w]) {
        cycle = std::stack<int>();
        for (int x = v; x != w; x = edgeTo[x])
          cycle.push(x);
        cycle.push(w);
        cycle.push(v);
      }
    }
    onStack[v] = false;
  }
};

#endif
