#pragma once
#include <unordered_map>
#include <vector>

template <typename GraphType> class CC {
public:
  using ID = typename GraphType::NodeIdType;

private:
  std::vector<bool> marked;
  std::vector<ID> id;
  int _count = 0;
  std::unordered_map<ID, ID> map;

public:
  CC(const GraphType &G) {
    marked.resize(G.get_node_count());
    id.resize(G.get_node_count());

    for (int u = 0; u < G.get_node_count(); u++) {
      if (!marked[u]) {
        dfs(G, u);
        _count++;
      }
    }
  }

private:
  void dfs(const GraphType &G, ID u) {
    marked[u] = true;
    id[u] = _count;
    map[u] = _count;
    for (auto w : G.adj()[u]) {
      if (!marked[w])
        dfs(G, w);
    }
  }

public:
  bool connected(ID u, ID v) const { return id[u] == id[v]; }
  const ID key(int u) { return id[u]; }
  const int count() { return _count; }

    const std::vector<ID>& data()const{
        return id;
    }

};
