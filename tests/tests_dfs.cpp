#include "dfs_graph.h"
#include "bfs_graph.h"
#include "Path.h"
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

TEST_CASE("DFS","[Dfs]")
{
    SG G(3);
    for(int i=0; i<5 ; i++)
        G.add_node(i);

    G.add_edge(0,1);
    G.add_edge(1,2);
    G.add_edge(2,0);

    Dfs dfs(G,0);

}

TEST_CASE("BFS","[Dfs]")
{
    SG G(3);
    for(int i=0; i<5 ; i++)
        G.add_node(i);

    G.add_edge(0,1);
    G.add_edge(1,2);
    G.add_edge(2,0);

    Bfs bfs(G,0);

}

TEST_CASE("DFS PATH","[Dfs]")
{
    SG G(3);
    for(int i=0; i<5 ; i++)
        G.add_node(i);

    G.add_edge(0,1);
    G.add_edge(1,2);
    G.add_edge(2,0);

    Path::DfsPath dp(G,0);
    auto path = dp.pathTo(2);
    while(!path.empty())
    {
        path.pop();
    }


}
