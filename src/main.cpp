#include "GraphVisualizer.h"
#include "Graph.h"
#include "VGraph.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <stack>
#include <string.h>
#include <unordered_map>
using namespace graphlib;

using namespace std;

std::string tiny =
    "/Users/simelabs/Documents/learn/alogoritm-cpp/resources/tinyEWG.txt";
std::string medium = "/Users/simelabs/Downloads/mediumEWG.txt";

void renderGraph(
    const Graph<std::string> &graph,
    const std::unordered_map<int, std::pair<float, float>> &positions
    );
float lerp(float a, float b, float t) { return a + t * (b - a); };
int main() {

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
    if (iss >> first >> second) {
      edges.emplace_back(first, second);
    } else {
      std::cerr << "incorrect line format " << std::endl;
    }
  }

  input.close();

  Graph<std::string> graph(number_of_vertices);
  for (int i = 0; i < number_of_vertices; i++) {
    graph.add_node(std::to_string(i));
  }
  for (const auto &e : edges)
    graph.add_edge(e.first, e.second);

  ForceDirectedLayout layout(graph);
  layout.simulate(100);
  auto positions = layout.getPositions();

  renderGraph(graph, positions);
};

void renderGraph(
    const Graph<std::string> &graph,
    const std::unordered_map<int, std::pair<float, float>> &positions) {
  sf::RenderWindow window(sf::VideoMode({800, 800}, 24), "Graph Visualizer");
  Vis::GraphViz gvz(graph,positions, 800, 800);
  sf::Clock animationClock;
  const sf::Time animationDuration = sf::seconds(3.0f);

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
    gvz.drawGraph(window, font);

    sf::Time elapasedTime = animationClock.getElapsedTime();
    float t = elapasedTime.asSeconds() / animationDuration.asSeconds();

    window.display();
  }
}
