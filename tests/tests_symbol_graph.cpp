#include "symbol_graph.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace std;

TEST_CASE("Symbol Graph From File", "[SymbolGraph]"){
    SymbolGraph SG("/Users/simelabs/Documents/learn/alogoritm-cpp/tests/symbol_graph_input.txt");

    for(const auto& pair : SG.edges())
    {
        cout << pair.first ;
        for(const auto& e :pair.second)
            cout << " - " << e  ;
        cout << endl;
    }
    
}
