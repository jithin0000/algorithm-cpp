#ifndef SYMBOL_GRAPH_H
#define SYMBOL_GRAPH_H
#include "directed_graph.h"
#include "unordered_map"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

class SymbolGraph {

private:
  std::unordered_map<std::string, int> st;
  std::vector<std::string> keys;
  DirectedGraph G;

public:
  std::vector<std::string> splitString(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(str);

    while (std::getline(ss, token, delimiter)) {
      tokens.push_back(token);
    }
    return tokens;
  }

  SymbolGraph(const std::string &filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
      throw std::runtime_error("Failed to open file");
    }
    std::string line;

    while (std::getline(inputFile, line)) {
      std::vector<std::string> parts = splitString(line, '/');

      if (parts.empty() && !line.empty() &&
          line.find('/') == std::string::npos) {
      } else if (parts.empty() && line.empty()) {
      } else {
        // we get the data
        for (const std::string &part : parts) {
          if (st.count(part) == 0) {
            st.emplace(part, st.size());
          }
        }
      }
    }

    inputFile.close();
    keys.resize(st.size());
    for (const auto &key : st)
    {
        keys[key.second] = key.first;
    }


    std::ifstream secondPass(filename);
    std::string edgeLine;
    G = DirectedGraph(st.size());
    while (std::getline(secondPass, edgeLine)) {
      std::vector<std::string> edgeParts = splitString(edgeLine, '/');
      if (edgeParts.empty() && !edgeLine.empty() && edgeLine.find('/')) {
      } else if (edgeParts.empty() && edgeLine.empty()) {
      } else {
        int v = st[edgeParts[0]];
        
        for (int i = 1; i < edgeParts.size(); i++) {
          G.addEdge(v, st[edgeParts[i]]);
        }
      }
    }
    secondPass.close();
  }

  const DirectedGraph &graph() const { return G; }
  std::string name(int v) const { return keys[v]; }
  int index(const std::string &s) { return st[s]; }
  bool contains(const std::string &s) const { return st.find(s) != st.end(); }
};

#endif
