#include "GraphVisualizer.h"
#include "PGraph.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <optional>
#include <unordered_map>

using namespace std;

void renderGraph(
    const PGraph<std::string> &graph,
    const std::unordered_map<int, std::pair<double, double>> &positions);
int main() {

  PGraph<std::string> graph(5); // 5 nodes (IDs: 0 to 4)
  graph.add_edge(0, 1);
  graph.add_edge(1, 2);
  graph.add_edge(2, 3);
  graph.add_edge(3, 4);
  graph.add_edge(4, 0);

  // Compute layout
  ForceDirectedLayout layout(graph);
  layout.simulate(100);
  //  Get positions for visualization
  auto positions = layout.getPositions();
  for (const auto &[nodeId, pos] : positions) {
    std::cout << "Node " << nodeId << ": (" << pos.first << ", " << pos.second
              << ")\n";
  }

  renderGraph(graph, positions);
};

void renderGraph(
    const PGraph<std::string> &graph,
    const std::unordered_map<int, std::pair<double, double>> &positions) {
  sf::RenderWindow window(sf::VideoMode({800, 600}, 24), "Graph Visualizer");
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

    window.clear(sf::Color::Black);

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
    double windowCenterY = 300; // half of 600

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
      circle.setFillColor(sf::Color::Green);
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

    window.display();
  }
}
