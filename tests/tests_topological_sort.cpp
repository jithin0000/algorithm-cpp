#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "directed_graph.h"
#include "topological_sort.h"


TEST_CASE("Topological Sort", "[TopologicalSort]")
{
    DirectedGraph G(5);
    TopologicalSort ts(G);
}

