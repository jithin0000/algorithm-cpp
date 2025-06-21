#pragma once
#include <GraphVisualizer.h>
#include <SFML/Graphics.hpp>
#include <array>
#include <exception>
#include <iostream>
#include <unordered_map>
#include <utility>

namespace Vis {
/// class should allow to visulize graph algorithms
///
/// operations
/// - DrawGraph
///     - draw vertices
///         - draw circle shape
///         - draw text
///     - draw edges as lines
///         - draw lines as vertex array with start and end
///         - draw edge data if present
///     - animate path
///         - draw line stripes
///         - update with each second
///             - lerp
///
///
template <typename GraphType> class GraphViz {
private:
  std::unordered_map<int, std::pair<float, float>> positions;
  GraphType g;
  float minX = 1e9, maxX = -1e9, minY = 1e9, maxY = -1e9;
  float centerX;
  float centerY;
  float windowCenterX;
  float windowCenterY; // half of 600

public:
  using NodeID = typename GraphType::NodeIdType;
  GraphViz(const GraphType &G, float width, float height) : g(G) {
    if (G.get_node_count() == 0)
      throw std::runtime_error("Graph is empty");
    ForceDirectedLayout layout(G);
    layout.simulate(100);
    positions = layout.getPositions();

    for (const auto &[id, pos] : positions) {
      minX = std::min(minX, pos.first);
      maxX = std::max(maxX, pos.first);
      minY = std::min(minY, pos.second);
      maxY = std::max(maxY, pos.second);
    }

    centerX = (minX + maxX) / 2.0;
    centerY = (minY + maxY) / 2.0;
    windowCenterX = width / 2;  // half of 800
    windowCenterY = height / 2; // half of 600
  }

private:
  void drawIdValueAtCenter(sf::RenderWindow &window, sf::Font &font, int id,
                           std::pair<float, float> pos) {

    sf::Text text(font);
    text.setString(std::to_string(id));
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.getCenter());
    text.setPosition({pos.first - centerX + windowCenterX,
                      pos.second - centerY + windowCenterY - 2});
    window.draw(text);
  }
  void drawVertices(sf::RenderWindow &window, sf::Font &font) {

    for (const auto &[id, pos] : positions) {
      sf::CircleShape circle(10);
      circle.setPosition({pos.first - centerX + windowCenterX - 10,
                          pos.second - centerY + windowCenterY - 10});
      circle.setFillColor(sf::Color::Blue);
      window.draw(circle);

      drawIdValueAtCenter(window, font, id, pos);
    }
  }
  void drawEdges(sf::RenderWindow &window) {

    for (int i = 0; i < g.get_node_count(); i++) {
      for (int j : g.adj()[i]) {
        auto start = positions.at(i);
        auto end = positions.at(j);

        std::array<sf::Vertex, 2> line = {
            sf::Vertex{sf::Vector2f(start.first - centerX + windowCenterX,
                                    start.second - centerY + windowCenterY)},
            sf::Vertex{sf::Vector2f(end.first - centerX + windowCenterX,
                                    end.second - centerY + windowCenterY)},
        };
                window.draw(line.data(), line.size(),sf::PrimitiveType::Lines);
      }
    }
  }

public:
  void drawGraph(sf::RenderWindow &window, sf::Font &font) {
    drawVertices(window, font);
    drawEdges(window);
  }
};

} // namespace Vis
