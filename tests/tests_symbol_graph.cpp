#include "symbol_graph.h"
#include "topological_sort.h"
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>

using namespace std;

TEST_CASE("Symbol Graph From File", "[SymbolGraph]") {
  SymbolGraph SG("/Users/simelabs/Documents/learn/alogoritm-cpp/tests/"
                 "symbol_graph_input.txt");
  TopologicalSort TS(SG.graph());
  std::vector<std::string> order;
  while (!TS.order().empty()) {
    int index = TS.order().top();
    order.push_back(SG.name(index));
    TS.order().pop();
  }

}
