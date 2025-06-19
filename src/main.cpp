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
using namespace graphlib;

using namespace std;

std::string tiny = "/Users/simelabs/Documents/learn/alogoritm-cpp/resources/tinyEWG.txt";
std::string medium = "/Users/simelabs/Downloads/mediumEWG.txt";

void renderGraph(
    const PGraph<std::string> &graph,
    const std::unordered_map<int, std::pair<double, double>> &positions,
    const std::vector<int> &path);
float lerp(float a, float b, float t) { return a + t * (b - a); };
int main() {

  // PGraph<std::string> graph(5); // 5 nodes (IDs: 0 to 4)
  // graph.add_edge(0, 1);
  // graph.add_edge(1, 2);
  // graph.add_edge(2, 3);
  // graph.add_edge(3, 4);
  // graph.add_edge(4, 0);
  // graph.add_edge(1, 3);

  // Compute layout
  // ForceDirectedLayout layout(graph);
  // layout.simulate(100);
  //  Get positions for visualization
  // auto positions = layout.getPositions();
  // for (const auto &[nodeId, pos] : positions) {
  // std::cout << "Node " << nodeId << ": (" << pos.first << ", " << pos.second
  //          << ")\n";
  // }
  ///
  ///

  std::ifstream input(tiny);
  if (!input.is_open()) {
    std::cerr << "Failed to open file" << std::endl;
    return 1;
  }

  int number_of_vertices;
  std::string line;
  std::vector<std::pair<int, int>> edges;
  if (std::getline(input, line)) {
  std:
    istringstream iss(line);
    if (!(iss >> number_of_vertices)) {
      return 1;
    }
  }
  std::getline(input, line);
  while (std::getline(input, line)) {
    std::istringstream iss(line);
    int first, second;
    float third;
    if (iss >> first >> second >> third) {
      edges.emplace_back(first, second);
    } else {
      std::cerr << "incorrect line format " << std::endl;
    }
  }

  input.close();

  PGraph<std::string> graph(number_of_vertices);
  std::string value = "Node ";
  for (int i = 0; i < number_of_vertices; i++) {
    value += std::to_string(i);
    graph.add_node(value);
  }

  for (const auto &edge : edges) {
    graph.add_edge(edge.first, edge.second);
  }

  ForceDirectedLayout layout(graph);
  layout.simulate(100);
  auto positions = layout.getPositions();
  Path::BfsPath dfs(graph, 0);
  auto path = dfs.pathTo(2);
  std::vector<int> pathv;
  while (!path.empty()) {
    pathv.push_back(path.top());
    path.pop();
  }

  renderGraph(graph, positions, pathv);
};

void renderGraph(
    const PGraph<std::string> &graph,
    const std::unordered_map<int, std::pair<double, double>> &positions,
    const std::vector<int> &path) {
  sf::RenderWindow window(sf::VideoMode({800, 800}, 24), "Graph Visualizer");

  // --- Center the graph in the window ---
  double minX = 1e9, maxX = -1e9, minY = 1e9, maxY = -1e9;
  for (const auto &[id, pos] : positions) {
    minX = std::min(minX, pos.first);
    maxX = std::max(maxX, pos.first);
    minY = std::min(minY, pos.second);
    maxY = std::max(maxY, pos.second);
  }
  double centerX = (minX + maxX) / 2.0;
  double centerY = (minY + maxY) / 2.0;
  double windowCenterX = 400; // half of 800
  double windowCenterY = 400; // half of 600

  sf::Clock animationClock;
  const sf::Time animationDuration = sf::seconds(3.0f);
  size_t currentSegment = 0;
  sf::VertexArray p(sf::PrimitiveType::LineStrip);
  p.append(sf::Vertex{sf::Vector2f(
      static_cast<float>(positions.at(0).first - centerX + windowCenterX),
      static_cast<float>(positions.at(0).second - centerY + windowCenterY))});

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
    for (int u = 0; u < graph.get_node_count(); u++) {
      for (int v : graph.adj()[u]) {
        auto fromPos = positions.at(u);
        auto toPos = positions.at(v);
        std::array<sf::Vertex, 2> line = {
            sf::Vertex{sf::Vector2f(
                static_cast<float>(fromPos.first - centerX + windowCenterX),
                static_cast<float>(fromPos.second - centerY + windowCenterY))},
            sf::Vertex{sf::Vector2f(
                static_cast<float>(toPos.first - centerX + windowCenterX),
                static_cast<float>(toPos.second - centerY + windowCenterY))}};
        window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
      }
    }
    sf::Font font;

    if (!font.openFromFile("/Users/Simelabs/Downloads/arial.ttf")) {
      // Handle error: font file not found
      // You can download a .ttf font and place it in your project directory
    }

    // Draw nodes
    for (const auto &[id, pos] : positions) {
      sf::CircleShape circle(10);
      circle.setPosition(
          {static_cast<float>(pos.first - centerX + windowCenterX - 10),
           static_cast<float>(pos.second - centerY + windowCenterY - 10)});
      circle.setFillColor(sf::Color::Blue);
      window.draw(circle);

      // Draw node ID at the center
      sf::Text text(font);
      text.setString(std::to_string(id));
      text.setCharacterSize(14);
      text.setFillColor(sf::Color::White);
      // Center the text in the circle
      sf::FloatRect textRect = text.getLocalBounds();
      text.setOrigin(textRect.getCenter());
      text.setPosition({
          static_cast<float>(pos.first - centerX + windowCenterX),
          static_cast<float>(pos.second - centerY + windowCenterY -
                             2) // -2 for better vertical centering
      });
      window.draw(text);
    }

    if (currentSegment < path.size() - 1) {
      sf::Time elapasedTime = animationClock.getElapsedTime();
      float t = elapasedTime.asSeconds() / animationDuration.asSeconds();

      auto u = path[currentSegment];
      auto w = path[currentSegment + 1];

      sf::Vector2f start = sf::Vector2f(
          static_cast<float>(positions.at(u).first - centerX + windowCenterX),
          static_cast<float>(positions.at(u).second - centerY + windowCenterY));
      sf::Vector2f end = sf::Vector2f(
          static_cast<float>(positions.at(w).first - centerX + windowCenterX),
          static_cast<float>(positions.at(w).second - centerY + windowCenterY));


      if (p.getVertexCount() == currentSegment + 1) {
        p.append(sf::Vertex{start, sf::Color::Red});
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
