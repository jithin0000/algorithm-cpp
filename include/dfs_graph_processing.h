#ifndef DFS_GRAPH_PROCESSING_H
#define DFS_GRAPH_PROCESSING_H

#include "directed_graph.h"
#include <iostream>
#include <memory.h>
using namespace std;

class DiagraphDFS {
private:
  std::unique_ptr<int[]> marked;

public:
  DiagraphDFS(DirectedGraph &G, int source)
      : marked(std::make_unique<int[]>(G.V())) {
    for (size_t i = 0; i < G.V(); i++)
      marked[i] = false;
    dfs(G, source);
  }
  void dfs(DirectedGraph &G, int v) {
    cout << v << endl;
    marked[v] = true;
    for (const int w : G.adj(v)) {
      if (!marked[w]) {
        dfs(G, w);
      }
    }
  }
};
#endif // DFS_GRAPH_PROCESSING_H
