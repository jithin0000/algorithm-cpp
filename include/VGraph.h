#pragma once
#include <GraphVisualizer.h>
#include <SFML/Graphics.hpp>
#include <array>
#include <exception>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <utility>

namespace Vis {

struct Vertex {
  int id;
  sf::Vector2f pos;
};

template <typename GraphType> class GraphViz {
private:
  std::vector<Vertex> vertices;
  GraphType g;

public:
  using NodeID = typename GraphType::NodeIdType;
  GraphViz(const GraphType &G,
           const std::unordered_map<int, std::pair<float, float>> positions,
           float width, float height)
      : g(G) {
    if (G.get_node_count() == 0)
      throw std::runtime_error("Graph is empty");

    for (const auto &[id, pos] : positions) {
      vertices.push_back({id, sf::Vector2f(pos.first, pos.second)});
    }
  }

private:
  void drawIdValueAtCenter(sf::RenderWindow &window, sf::Font &font, int id) {

    Vertex v = vertices.at(id);
    sf::Text text(font);
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.getCenter());
    text.setString(std::to_string(id));
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);
    text.setPosition(v.pos);
    window.draw(text);
  }
  void drawVertices(sf::RenderWindow &window, sf::Font &font) {

    for (const auto &v : vertices) {
      sf::CircleShape circle(10);
      circle.setOrigin({10.0f, 10.0f});
      circle.setPosition(v.pos);
      circle.setFillColor(sf::Color::Blue);
      window.draw(circle);
      drawIdValueAtCenter(window, font, v.id);
    }
  }
  void drawEdges(sf::RenderWindow &window) {

    for (int i = 0; i < g.get_node_count(); i++) {
      for (int j : g.adj()[i]) {

        Vertex start = vertices.at(i);
        Vertex end = vertices.at(j);

        std::array<sf::Vertex, 2> line = {
            sf::Vertex{start.pos},
            sf::Vertex{end.pos},
        };
        window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
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
