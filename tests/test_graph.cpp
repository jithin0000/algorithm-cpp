#include "graph.h"
#include "directed_graph.h"
#include <catch2/catch_test_macros.hpp>

 TEST_CASE("Graph initialization", "[graph]") {
    Graph g(5);
    REQUIRE(g.V() == 5);
}

TEST_CASE("Adding edges", "[graph]") {
    Graph g(3);
    
    SECTION("Directed edges") {
        g.addEdge(0, 1);
        g.addEdge(0, 2);
        REQUIRE(g.adj(0).size() == 2);
    }
}

