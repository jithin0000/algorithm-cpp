#include "PGraph.h"
#include <catch2/catch_test_macros.hpp>
#include<iostream>
#include <initializer_list>
#include <string>
#include <utility>
#include <type_traits>
using namespace graphlib;
typedef PGraph<int, EmptyEdgeData, true> DirectedGraph;
typedef PGraph<int> SG;

TEST_CASE("PGraph Add Node", "[PGraph]") {
  PGraph<int> G;
    for(int i=0; i < 10 ; i++)
    {
        G.add_node(i);
    }
  REQUIRE(G.IsDirectedGraph == false);
  REQUIRE(G.get_node_count() == 10);
  REQUIRE(G.get_edge_count() == 0);

    int id=9;
  int data = G.get_node_data(id);
  REQUIRE(data == 9);
  REQUIRE_THROWS(G.get_node_data(11));

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
for (int i = 0; i < 10; i++) {
    G.add_node(i);
  }

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
    for (int i = 0; i < 3; i++) {
    G.add_node(i);
  }

    auto it = G.begin();
    REQUIRE(it->id()==0);
    REQUIRE((*it).id()==0);
    ++it;
    REQUIRE(it->id()==1);
    it++;
    REQUIRE(it->id()==2);
    ++it;
    REQUIRE(it==G.end());
}

TEST_CASE("Range Based","[PGraph]")
{
    PGraph<std::string, graphlib::EmptyEdgeData, false> G(3);
    G.add_node("A");
    G.add_node("B");
    G.add_node("C");
    std::vector<std::string> collected;
    for(auto node: G){
        collected.push_back(node.data());
    }
    auto out = std::vector<std::string>{"A", "B", "C"};;
    REQUIRE(collected== out);
}

TEST_CASE("RandomIterator","[PGraph]")
{
    SG g(5);
    for(int i=0 ; i<5 ; i++)
        g.add_node(i);
    auto it = g.begin();
    REQUIRE(it[2].id()==2);
    REQUIRE((it+3)->id()==3);
    REQUIRE((g.end()-2)->id()==3);
    REQUIRE((g.end()-g.begin())==5);

}
TEST_CASE("ConstIterator","[PGraph]")
{
    SG g(3);
    g.add_node(1);
    const auto& cg =g;
    auto it = cg.cbegin();
    static_assert(std::is_const_v<std::remove_reference_t<decltype(it->data())>>);
    REQUIRE(it->id()==0);

}
TEST_CASE("EdgeCases", "[PGraph]")
{
    SG G(1);
    REQUIRE(G.begin()==G.end());

    G.add_node(10);
    REQUIRE_FALSE(G.begin()==G.end());
    REQUIRE(++G.begin()==G.end());
}
TEST_CASE("NodeProxy", "[PGraph]"){
    SG G(2);
    G.add_node(42);
    G.add_node(23);
    G.add_edge(0,1);

    auto node = *G.begin();
    REQUIRE(node.id()==0);
    REQUIRE(node.degree()==1);
    REQUIRE(node.data()==42);
}

TEST_CASE("Bfs","[PGraph]")
{
   SG G(0);
    G.add_node(0);
    auto result = algorithms::bfs(G,0);

    REQUIRE(result.visited.size()==1);
    REQUIRE(result.distances.size()==1);

}




