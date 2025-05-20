#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "dfs_cycle.h"
#include "directed_graph.h"


TEST_CASE("Directed Cycle detect Correctly", "[DirectedCycle]"){
    SECTION("No cycle detected for empty graph")
    {
        DirectedGraph G(3);
        DfsCycle dc(G);
        REQUIRE(dc.hasCycle()==false);
    }
    SECTION("No cycle in graph")
    {
        DirectedGraph G(4);
        G.addEdge(0,1);
        G.addEdge(1,2);
        G.addEdge(2,3);

        DfsCycle dc(G);

        REQUIRE(dc.hasCycle()==false);
    }
    
    SECTION("Detects simple cycle") {
        DirectedGraph g(3);
        g.addEdge(0, 1);
        g.addEdge(1, 2);
        g.addEdge(2, 0); // Creates cycle 0->1->2->0
        
        DfsCycle dc(g);
        REQUIRE(dc.hasCycle());
        
        auto cycle = dc.getCycle();
        REQUIRE(cycle.size() == 4); // 0->1->2->0
        REQUIRE(cycle.top() == 0); cycle.pop();
        REQUIRE(cycle.top() == 2); cycle.pop();
        REQUIRE(cycle.top() == 1); cycle.pop();
        REQUIRE(cycle.top() == 0);
    }

}

