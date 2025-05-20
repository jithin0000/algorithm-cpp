#include "directed_graph.h"
#include "dfs_graph_processing.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Graph initialization", "[directed_graph]"){

    DirectedGraph dg(5);
    REQUIRE(dg.V() == 5);
}
TEST_CASE("GRAPH addEdge", "[directed_graph]")
{
    DirectedGraph db(5);
    db.addEdge(0,1);
    db.addEdge(0,2);

    REQUIRE(db.E()==2);
}

TEST_CASE("DFS", "[directed_graph]")
{

    DirectedGraph G(5);
    G.addEdge(0,1);
    G.addEdge(0,2);
    DiagraphDFS dag(G,0);
}
