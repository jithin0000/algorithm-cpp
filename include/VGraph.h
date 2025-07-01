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
  GraphViz(const GraphType &G,
           const std::unordered_map<int, std::pair<float, float>> poss,
           float width, float height)
      : g(G), positions(poss) {
    if (G.get_node_count() == 0)
      throw std::runtime_error("Graph is empty");

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

class PathViz {
private:
  std::unordered_map<int, std::pair<float, float>> positions;
  sf::VertexArray p;
  int currentSegment = 0;
  std::vector<int> path;
  float minX = 1e9, maxX = -1e9, minY = 1e9, maxY = -1e9;
  float centerX;
  float centerY;
  float windowCenterX;
  float windowCenterY; // half of 600

  float lerp(float a, float b, float t) { return a + t * (b - a); };

public:
  PathViz(const std::vector<int> &pth,
          const std::unordered_map<int, std::pair<float, float>> &pos,
          float width, float height)
      : positions(pos), path(pth), p(sf::PrimitiveType::LineStrip) {

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
    if (!path.empty()) {
      auto first = positions[path[0]];
      p.append(
          sf::Vertex{sf::Vector2f(first.first - centerX + windowCenterX,
                                  first.second - centerY + windowCenterY)});
    }
  }

  void update(float t, sf::Clock &timer) {

    if (path.size() > 0 && currentSegment < path.size() - 1) {
      auto u = path[currentSegment];
      auto v = path[currentSegment + 1];

      sf::Vector2f start =
          sf::Vector2f(positions.at(u).first - centerX + windowCenterX,
                       positions.at(u).second - centerY + windowCenterY);
      sf::Vector2f end =
          sf::Vector2f(positions.at(v).first - centerX + windowCenterX,
                       positions.at(v).second - centerY + windowCenterY);
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
        timer.restart();
      }
    }
  }

  void drawPath(sf::RenderWindow &window) { window.draw(p); }
};

} // namespace Vis
