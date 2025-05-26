#include <iostream>
#include "symbol_graph.h"
#include "topological_sort.h"
using namespace std;
int main(){
    int a = 1;
    int b = 3;

   DirectedGraph G(5); 
  TopologicalSort TS(G);
  while (!TS.order().empty()) {
    int index = TS.order().top();
    TS.order().pop();
  }

}
