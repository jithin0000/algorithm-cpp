#pragma once
#include <stack>
#include <vector>

namespace Path {

template <typename GraphType> class DfsPath {
public:
  using NodeID = typename GraphType::NodeIdType;

private:
  std::vector<NodeID> edgeTo;
  std::vector<bool> visited;
  NodeID _s;

public:
  DfsPath(const GraphType &G, typename GraphType::NodeIdType source)
      : _s(source) {
    visited.resize(G.get_node_count());
    edgeTo.resize(G.get_node_count());
    dfs(G, source);
  }

private:
  void dfs(const GraphType &G, typename GraphType::NodeIdType u) {
    visited[u] = true;
    for (auto w : G.adj()[u]) {
      if (!visited[w]) {
        edgeTo[w] = u;
        dfs(G, w);
      }
    }
  }

public:
  const bool hasToPath(NodeID ID) const { return visited[ID]; }
  std::stack<NodeID> pathTo(NodeID v) const {
    std::stack<NodeID> path;
    if (!hasToPath(v))
      return path;

    for (int x = v; x != _s; x = edgeTo[x]) {
      path.push(x);
    }
    path.push(_s);
    return path;
  }
};
}; // namespace Path
