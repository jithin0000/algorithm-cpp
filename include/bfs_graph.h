#pragma once
#include <iostream>
#include <queue>
#include <vector>

template <typename GraphType> class Bfs {
public:
  using ID = typename GraphType::NodeIdType;

private:
  std::vector<bool> visited;
  std::queue<ID> q;

public:
  Bfs(const GraphType &G, typename GraphType::NodeIdType start) {
    visited.resize(G.get_node_count());
    q.push(start);

    while (!q.empty()) {
      auto u = q.front();
      q.pop();
      visited[u] = true;
      for (auto w : G.adj()[u])
        if (!visited[w])
          q.push(w);
    }
  }
};
