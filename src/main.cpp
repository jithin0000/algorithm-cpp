#include "CC.h"
#include "GraphVisualizer.h"
#include "PGraph.h"
#include "Path.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <stack>
#include <unordered_map>
#include "VGraph.h"
using namespace graphlib;

using namespace std;

std::string tiny =
    "/Users/simelabs/Documents/learn/alogoritm-cpp/resources/tinyG.txt";
std::string medium = "/Users/simelabs/Downloads/mediumEWG.txt";

void renderGraph(
    const PGraph<std::string> &graph,
    const std::unordered_map<int, std::pair<float, float>> &positions,
    const std::vector<int> &path);
float lerp(float a, float b, float t) { return a + t * (b - a); };
int main() {

  // std::ifstream input(tiny);
  // if (!input.is_open()) {
  //   std::cerr << "Failed to open file" << std::endl;
  //   return 1;
  // }

  // int number_of_vertices;
  // std::string line;
  // std::vector<std::pair<int, int>> edges;
  // if (std::getline(input, line)) {
  // std:
  //   istringstream iss(line);
  //   if (!(iss >> number_of_vertices)) {
  //     return 1;
  //   }
  // }
  // std::getline(input, line);
  // while (std::getline(input, line)) {
  //   std::istringstream iss(line);
  //   int first, second;
  //   if (iss >> first >> second) {
  //     edges.emplace_back(first, second);
  //   } else {
  //     std::cerr << "incorrect line format " << std::endl;
  //   }
  // }

  // input.close();

  PGraph<std::string> graph(13);
  std::string value = "Node ";
  for (int i = 0; i < 13; i++) {
    value += std::to_string(i);
    graph.add_node(value);
        value ="Node ";
  }
    graph.add_edge(0,1);
    graph.add_edge(0,2);


  ForceDirectedLayout layout(graph);
  layout.simulate(100);
  auto positions = layout.getPositions();
  Path::DfsPath dfs(graph, 0);
  auto path = dfs.pathTo(3);
  std::vector<int> pathv;
  while (!path.empty()) {
    pathv.push_back(path.top());
    path.pop();
  }


  renderGraph(graph, positions, pathv);
};

void renderGraph(
    const PGraph<std::string> &graph,
    const std::unordered_map<int, std::pair<float, float>> &positions,
    const std::vector<int> &path) {
  sf::RenderWindow window(sf::VideoMode({800, 800}, 24), "Graph Visualizer");

  // --- Center the graph in the window ---
  float minX = 1e9, maxX = -1e9, minY = 1e9, maxY = -1e9;
  for (const auto &[id, pos] : positions) {
    minX = std::min(minX, pos.first);
    maxX = std::max(maxX, pos.first);
    minY = std::min(minY, pos.second);
    maxY = std::max(maxY, pos.second);
  }

  float centerX = (minX + maxX) / 2.0;
  float centerY = (minY + maxY) / 2.0;
  float windowCenterX = 400; // half of 800
  float windowCenterY = 400; // half of 600
    Vis::GraphViz gvz(graph,800,800);

  sf::Clock animationClock;
  const sf::Time animationDuration = sf::seconds(3.0f);
  size_t currentSegment = 0;
  sf::VertexArray p(sf::PrimitiveType::LineStrip);
  p.append(sf::Vertex{
      sf::Vector2f(positions.at(0).first - centerX + windowCenterX,
                   positions.at(0).second - centerY + windowCenterY)});


  while (window.isOpen()) {

    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      } else if (const auto *keyPressed =
                     event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
          window.close();
      }
    }

    window.clear(sf::Color(128, 128, 128));
    // Draw edges

    sf::Font font;
    if (!font.openFromFile("/Users/Simelabs/Downloads/arial.ttf")) {
            std::runtime_error("Font not loaded");
    }

    // Draw nodes
    gvz.drawGraph(window,font);

    if (path.size() > 0 && currentSegment < path.size() - 1) {
      sf::Time elapasedTime = animationClock.getElapsedTime();
      float t = elapasedTime.asSeconds() / animationDuration.asSeconds();

      auto u = path[currentSegment];
      auto w = path[currentSegment + 1];

      sf::Vector2f start =
          sf::Vector2f(positions.at(u).first - centerX + windowCenterX,
                       positions.at(u).second - centerY + windowCenterY);
      sf::Vector2f end =
          sf::Vector2f(positions.at(w).first - centerX + windowCenterX,
                       positions.at(w).second - centerY + windowCenterY);

      if (p.getVertexCount() == currentSegment + 1) {
        p.append(sf::Vertex{start, sf::Color::Red});
        p.append(sf::Vertex{end, sf::Color::Red});
      }

      sf::Vector2f currentPos;
      currentPos.x = lerp(start.x, end.x, t);
      currentPos.y = lerp(start.y, end.y, t);
      p[currentSegment + 1].position = currentPos;

      if (t >= 1.0f) {
        p[currentSegment + 1].position = end;
        currentSegment++;
        animationClock.restart();
      }
    }
    window.draw(p);

    window.display();
  }
}
