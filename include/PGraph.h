#pragma once
#include <exception>
#include <string>
#include <type_traits>
#include <vector>

struct EmptyEdgeData {};
template <typename NodeData, typename EdgeData = EmptyEdgeData,
          bool IsDirected = false>
class PGraph {

private:
  using NodeID = int;
  using NodeIdType = NodeID;
  using NodeDataType = NodeData;
  using EdgeDataType = EdgeData;
  struct InternalNode;
  int _edge_count = 0;

  std::vector<InternalNode> _nodes;
  std::vector<std::vector<NodeID>> _adj;

  void check_valid_node(NodeIdType ID, std::string METHOD) {
    if (ID >= _nodes.size())
      throw std::out_of_range(std::string("out of range::") + METHOD);
  }

  void check_valid_node(NodeIdType ID, std::string METHOD) const {
    if (ID >= _nodes.size())
      throw std::out_of_range(std::string("out of range::") + METHOD);
  }

public:
  PGraph() = default;
  PGraph(int initial_node_count) {};

  static constexpr bool IsDirectedGraph = IsDirected;

  // Node Operations
  NodeIdType add_node(NodeDataType data = NodeDataType{}) {
    NodeIdType new_id = _nodes.size();
    _nodes.push_back({data});
    _adj.resize(_nodes.size());
    return new_id;
  }

  NodeDataType &get_node_data(NodeIdType ID) {
    check_valid_node(ID, "GET_NODE");
    return _nodes[ID].data;
  }

  const NodeDataType &get_node_data(NodeIdType ID) const {
    check_valid_node(ID, "GET_NODE_DATA_CONST");
    return _nodes[ID].data;
  }

  // EDGE Operations
  void add_edge(NodeIdType u, NodeIdType v) {
    check_valid_node(u, "ADD_EDGE");
    check_valid_node(v, "ADD_EDGE");
    if (u == v)
      throw std::runtime_error("self loop::ADD_EDGE");
    // check for parallel edge
    _adj[u].push_back(v);
    _nodes[u].out_degree_count++;
    if constexpr (!IsDirectedGraph) {
      _adj[v].push_back(u);
      _nodes[v].out_degree_count++;
    }
    _edge_count++;
  }

  int get_node_count() const { return _nodes.size(); }
  int get_edge_count() const { return _edge_count; }

  int degree(NodeIdType ID) const {
    check_valid_node(ID, "DEGREE");
    return _adj[ID].size();
  }

  template <bool D = IsDirectedGraph, typename = std::enable_if_t<D>>
  int out_degree(NodeIdType ID) const {
    check_valid_node(ID, "OUT_DEGREE");
    return _adj[ID].size();
  }

  template <bool D = IsDirectedGraph, typename = std::enable_if_t<D>>
  int in_degree(NodeIdType ID) const {
    check_valid_node(ID, "IN_DEGREE");
    int count = 0;
    for (int u =0 ; u < _nodes.size() ; u++)
      for (NodeIdType v : _adj[u])
        if (ID == v)
          count++;

    return count;
  }

private:
  struct InternalNode {
    NodeDataType data;
    int out_degree_count = 0;
  };
};
