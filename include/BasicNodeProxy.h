#pragma once
#include <type_traits>

template <typename ND, typename ED, bool IsD> class PGraph;

template <typename NodeData, typename NodeId, bool IsConst, class PGraphPtrT>
class BasicNodeProxy {

public:
  using NodeIdType = NodeId;
  using NodeDataType = NodeData;

  BasicNodeProxy(NodeIdType ID, PGraphPtrT G) : _id(ID), _graph_ptr(G) {};

  NodeIdType id() const { return _id; }
  auto data() const
      -> std::conditional_t<IsConst, const NodeDataType &, NodeDataType &> {
    return _graph_ptr->get_node_data(_id);
  }

private:
  NodeIdType _id;
  PGraphPtrT _graph_ptr;
};
