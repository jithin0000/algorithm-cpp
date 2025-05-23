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
  std::unordered_set<std::string> _vertices;
  std::unordered_map<std::string, std::vector<std::string>> _edges;

public:
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
        std::cout << "  /" << std::endl;
      } else if (parts.empty() && line.empty()) {
        std::cout << "line is empty " << std::endl;
      } else {
        // we get the data
        std::string v = parts.at(0);
        std::vector<std::string> es;
        _vertices.insert(v);
        for (int i = 1; i < parts.size(); i++) {
          _vertices.insert(parts.at(i));
          es.push_back(parts.at(i));
        }
        _edges.emplace(v, es);
      }
    }
    inputFile.close();
  }

  const std::unordered_set<std::string> vertices() const { return _vertices; }

  const std::unordered_map<std::string, std::vector<std::string>>
  edges() const {
    return _edges;
  };

  std::vector<std::string> splitString(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(str);

    while (std::getline(ss, token, delimiter)) {
      tokens.push_back(token);
    }
    return tokens;
  }
};

#endif
