#pragma once
#include <vector>
#include <iostream>

template <typename GraphType>
class Dfs{
public:
    using ID = typename GraphType::NodeIdType;
private:
    std::vector<bool>visited;
public:
    Dfs(const GraphType& G, typename GraphType::NodeIdType start)
    {
        visited.resize(G.get_node_count());
        dfs(G,start);
    }
private:
    void dfs(const GraphType& G, typename GraphType::NodeIdType u)
{
        visited[u]=true;
        for(auto w : G.adj()[u])
    {
            if(!visited[w])
                dfs(G,w);

        }
    }

};
