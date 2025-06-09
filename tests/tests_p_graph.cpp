#include "PGraph.h"
#include <catch2/catch_test_macros.hpp>
#include <initializer_list>
#include <string>
#include <utility>
using namespace graphlib;
typedef PGraph<int, EmptyEdgeData, true> DirectedGraph;
typedef PGraph<int> SG;

TEST_CASE("PGraph Add Node", "[PGraph]") {
  PGraph<int> G;
  int id = G.add_node(10);
  REQUIRE(id == 0);
  REQUIRE(G.IsDirectedGraph == false);
  REQUIRE(G.get_node_count() == 1);
  REQUIRE(G.get_edge_count() == 0);

  int data = G.get_node_data(id);
  REQUIRE(data == 10);
  REQUIRE_THROWS(G.get_node_data(1));
}

TEST_CASE("PGraph Add Edge", "[PGraph]") {

  PGraph<int> G;
  for (int i = 0; i < 5; i++)
    G.add_node(i);
  G.add_edge(0, 1);
  G.add_edge(0, 2);
  G.add_edge(1, 3);
  REQUIRE(G.get_node_count() == 5);
  REQUIRE(G.get_edge_count() == 3);

  REQUIRE_THROWS(G.add_edge(0, 0));
  REQUIRE_THROWS(G.add_edge(10, 0));
  REQUIRE_THROWS(G.add_edge(0, 10));
}

TEST_CASE("Test Degree", "[PGraph]") {

  PGraph<std::string> G;
  G.add_node("hi");
  G.add_node("hello");
  G.add_node("how");
  G.add_node("are you");

  G.add_edge(0, 1);
  G.add_edge(0, 2);
  G.add_edge(0, 3);
  G.add_edge(1, 3);

  REQUIRE(G.degree(0) == 3);
  REQUIRE_THROWS(G.degree(10));
}

TEST_CASE("IN_DEGREE AND OUT_DEGREE", "[PGraph]") {
  DirectedGraph G;
  for (int i = 0; i < 5; i++) {
    G.add_node(i);
  }
  G.add_edge(0, 1);
  G.add_edge(0, 2);
  G.add_edge(0, 3);
  G.add_edge(1, 3);
  G.add_edge(2, 3);

  REQUIRE_THROWS(G.in_degree(10));
  REQUIRE_THROWS(G.out_degree(10));

  REQUIRE(G.out_degree(0) == 3);
  REQUIRE(G.in_degree(3) == 3);
  REQUIRE(G.out_degree(2) == 1);
  REQUIRE(G.out_degree(1) == 1);
  REQUIRE(G.in_degree(1) == 1);
  REQUIRE(G.in_degree(4) == 0);
}

TEST_CASE("Constructor Initialization", "[PGraph]") {
  PGraph<int> G(10);
  REQUIRE(G.get_node_count() == 10);
  REQUIRE(G.adj().size() == 10);
}

TEST_CASE("Constructor Initializer List", "[PGraph]") {
  SG G{4,
       {
           {0, 1},
           {1, 3},
           {2, 3},
       }};

  REQUIRE(G.get_node_count() == 4);
  REQUIRE(G.get_edge_count() == 3);
  REQUIRE(G.degree(0) == 1);
  REQUIRE(G.degree(1) == 2);
  REQUIRE(G.degree(2) == 1);
}

TEST_CASE("Basic Iterator","[PGraph]")
{
    SG G(3);
    auto it = G.nodes_begin();
    REQUIRE(it->id()==0);
    REQUIRE((*it).id()==0);
    ++it;
    REQUIRE(it->id()==1);
    it++;
    REQUIRE(it->id()==2);
    ++it;
    REQUIRE(it==G.nodes_end());

}
