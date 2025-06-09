#pragma once
#include <concepts>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <concepts>

namespace graphlib {

template <typename NodeData, typename EdgeData, bool IsDirected = false>
class PGraph;

template <typename GraphClass, bool IsConstv> class NodeProxyImpl {

public:
  using ConcreteGraphType = GraphClass;
  using GraphPtrType = std::conditional_t<IsConstv, const ConcreteGraphType *,
                                          ConcreteGraphType *>;
  using NodeDataTypeRef =
      std::conditional_t<IsConstv,
                         const typename ConcreteGraphType::NodeDataType &,
                         typename ConcreteGraphType::NodeDataType &>;
  using NodeIdType = typename ConcreteGraphType::NodeIdType;

private:
  GraphPtrType _graph_ptr;
  NodeIdType _id;

public:
  NodeProxyImpl(GraphPtrType G, NodeIdType ID) : _graph_ptr(G), _id(ID) {
    if (!_graph_ptr)
      throw std::logic_error("NodeProxyImpl:: Graph Pointer cannot be null ");
  };
  NodeIdType id() const { return _id; }
  NodeDataTypeRef data() const { return _graph_ptr->get_node_data(_id); }
  int degree() const { return _graph_ptr->degree(_id); }

  NodeProxyImpl *operator->() { return this; }
  const NodeProxyImpl *operator->() const { return this; }
};

// node iterator implemenation
template <typename GraphClass, bool IsConstv>
class NodeIteratorImpl {
public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = NodeProxyImpl<GraphClass, IsConstv>;
  using difference_type = std::ptrdiff_t;
  using pointer = value_type;
  using reference = value_type;

private:
  using GraphPtrType =
      std::conditional_t<IsConstv, const GraphClass *, GraphClass*>;
  GraphPtrType _graph_ptr = nullptr;
  typename GraphClass::NodeIdType current_node_id = 0;

public:
  NodeIteratorImpl() = default;
  NodeIteratorImpl(GraphPtrType G, typename GraphClass::NodeIdType ID)
      : _graph_ptr(G), current_node_id(ID) {};

  reference operator*() const {
    if (!_graph_ptr)
      std::logic_error("Deference::NodeIteratorImpl");
    return value_type(_graph_ptr, current_node_id);
  }
  pointer operator->() const {
    if (!_graph_ptr)
      std::logic_error("Deference::NodeIteratorImpl");
    return value_type(_graph_ptr, current_node_id);
  }

  NodeIteratorImpl &operator++() {
    if (_graph_ptr && current_node_id < _graph_ptr->get_node_count())
      current_node_id++;
    else
      current_node_id = _graph_ptr->get_node_count();

    return *this;
  }

  NodeIteratorImpl operator++(int) {
    NodeIteratorImpl temp = *this;
    ++(*this);
    return temp;
  }

  NodeIteratorImpl &operator--() {
    if (_graph_ptr && current_node_id > 0)
      current_node_id--;
    return *this;
  }

  NodeIteratorImpl operator--(int) {
    NodeIteratorImpl temp = *this;
    --(*this);
    return temp;
  }
  NodeIteratorImpl &operator+=(difference_type n) {
    if (!_graph_ptr)
      return *this;
    difference_type target_id_signed =
        static_cast<difference_type>(current_node_id) + n;
    typename GraphClass::NodeIdType node_count = _graph_ptr->get_node_count();
    if (target_id_signed < 0)
      current_node_id = 0;
    else if (static_cast<typename GraphClass::NodeIdType>(target_id_signed) >=
             node_count)
      current_node_id = node_count;
    else
      current_node_id =
          static_cast<typename GraphClass::NodeIdType>(target_id_signed);
    return *this;
  }

  NodeIteratorImpl operator+(difference_type n) const {
    NodeIteratorImpl temp = *this;
    temp += n;
    return temp;
  }
  NodeIteratorImpl operator-(difference_type n) const {
    NodeIteratorImpl temp = *this;
    temp -= n;
    return temp;
  }
  NodeIteratorImpl &operator-=(difference_type n) const {
    return *this += (-n);
  }

  difference_type operator-(const NodeIteratorImpl &other) const {
    if (_graph_ptr != other._graph_ptr &&
        (_graph_ptr != nullptr && other._graph_ptr != nullptr))
      throw std::logic_error("Diffrent Graphs");
    return static_cast<typename GraphClass::NodeIdType>(current_node_id) -
           static_cast<typename GraphClass::NodeIdType>(other.current_node_id);
  }

  reference operator[](difference_type n) { return *(*this + n); }

  friend bool operator==(const NodeIteratorImpl &lhs,
                         const NodeIteratorImpl &rhs) {
    if (!lhs._graph_ptr && !rhs._graph_ptr)
      return lhs.current_node_id == rhs.current_node_id;
    return lhs._graph_ptr == rhs._graph_ptr &&
           lhs.current_node_id == rhs.current_node_id;
  }
  friend bool operator!=(const NodeIteratorImpl &lhs,
                         const NodeIteratorImpl &rhs) {
    return !(lhs == rhs);
  }
  friend bool operator<(const NodeIteratorImpl &lhs,
                        const NodeIteratorImpl &rhs) {
    if (lhs._graph_ptr != rhs._graph_ptr &&
        (lhs._graph_ptr != nullptr && rhs._graph_ptr != nullptr))
      throw std::logic_error("Diffrent Graphs");
    return lhs.current_node_id < rhs.current_node_id;
  }
    friend bool operator>(const NodeIteratorImpl& lhs, const NodeIteratorImpl& rhs){return rhs < lhs;}
    friend bool operator<=(const NodeIteratorImpl& lhs, const NodeIteratorImpl& rhs){return !(rhs < lhs);}
    friend bool operator>=(const NodeIteratorImpl& lhs, const NodeIteratorImpl& rhs){return !(lhs < rhs);}
};

struct EmptyEdgeData {};



template <typename NodeData, typename EdgeData = EmptyEdgeData, bool IsDirected>
class PGraph {

public:
  using NodeID = int;
  using NodeIdType = NodeID;
  using NodeDataType = NodeData;
  using EdgeDataType = EdgeData;
private:
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

private:
    using NodeProxy = NodeProxyImpl<PGraph<NodeData, EdgeData, IsDirected>, false>;
    using ConstNodeProxy = NodeProxyImpl<PGraph<NodeData, EdgeData, IsDirected>, true>;
    using NodeIterator = NodeIteratorImpl<PGraph<NodeData,EdgeData, IsDirected>,false>;
    using ConstNodeIterator = NodeIteratorImpl<PGraph<NodeData,EdgeData, IsDirected>,true>;

public:
  PGraph() = default;
  PGraph(int initial_node_count) {
    _nodes.resize(initial_node_count);
    _adj.resize(initial_node_count);
  };

  template <typename InputIt>
  PGraph(int node_count, InputIt edge_begin, InputIt edge_end) {
    _nodes.resize(node_count);
    _adj.resize(node_count);
    for (auto it = edge_begin; it != edge_end; ++it)
      add_edge(it->first, it->second);
  }

  PGraph(int node_count,
         std::initializer_list<std::pair<NodeIdType, NodeIdType>> edges) {
    _nodes.resize(node_count);
    _adj.resize(node_count);
    for (const auto &pair : edges)
      add_edge(pair.first, pair.second);
  }

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

  std::vector<InternalNode> &nodes() { return _nodes; }
  const std::vector<InternalNode> &nodes() const { return _nodes; }
  std::vector<std::vector<NodeIdType>> &adj() { return _adj; }
  const std::vector<std::vector<NodeIdType>> &adj() const { return _adj; }

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
    for (int u = 0; u < _nodes.size(); u++)
      for (NodeIdType v : _adj[u])
        if (ID == v)
          count++;

    return count;
  }

 NodeIterator nodes_begin(){return NodeIterator(this,0);}
 NodeIterator nodes_end(){return NodeIterator(this,get_node_count());}
 ConstNodeIterator nodes_begin()const{return ConstNodeIterator(this,0);}
 ConstNodeIterator nodes_end()const{return ConstNodeIterator(this,get_node_count());}
 ConstNodeIterator cnodes_begin()const{return ConstNodeIterator(this,0);}
 ConstNodeIterator cnodes_end()const{return ConstNodeIterator(this,get_node_count());}



private:
  struct InternalNode {
    NodeDataType data;
    int out_degree_count = 0;
  };
};

namespace algorithms {};
} // namespace graphlib
