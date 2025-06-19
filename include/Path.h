#pragma once
#include <iostream>
#include <queue>
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

template <typename GraphType> class BfsPath {
public:
  using NodeID = typename GraphType::NodeIdType;

private:
  std::vector<bool> marked;
  std::vector<NodeID> edgeTo;
  std::queue<NodeID> q;
  NodeID _s;

public:
  BfsPath(const GraphType &G, typename GraphType::NodeIdType start)
      : _s(start) {
    marked.resize(G.get_node_count());
    edgeTo.resize(G.get_node_count());
    q.push(start);
    marked[start] = true;
    while (!q.empty()) {
      NodeID u = q.front();
      std::cout << u << std::endl;
      q.pop();
      for (auto w : G.adj()[u]) {
        if (!marked[w]) {
          marked[w] = true;
          edgeTo[w] = u;
          q.push(w);
        }
      }
    }
  }

  const bool hasPathTo(NodeID v) const { return marked[v]; };
  std::stack<NodeID> pathTo(NodeID v) const {
    std::stack<NodeID> path;
    if (!hasPathTo(v))
      return path;

    for (int x = v; x != _s; x = edgeTo[x]) {
      path.push(x);
    }
    path.push(_s);
    return path;
  }
};

}; // namespace Path
