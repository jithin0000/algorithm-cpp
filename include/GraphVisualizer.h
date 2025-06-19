#pragma once
#include "PGraph.h"
#include <cmath>
#include <random>
#include <unordered_map>
#include <utility>

template <typename Graph> class ForceDirectedLayout {
private:
  const Graph &G;
  std::unordered_map<int, double> NodeIdToX;
  std::unordered_map<int, double> NodeIdToY;
  std::unordered_map<int, double> NodeIdToVx;
  std::unordered_map<int, double> NodeIdToVy;

public:
  ForceDirectedLayout(const Graph &g) : G(g) {};

  void initializer() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 800.0);
    for (int id = 0; id < G.get_node_count(); id++) {
      NodeIdToX[id] = dist(gen);
      NodeIdToY[id] = dist(gen);
      NodeIdToVx[id] = 0;
      NodeIdToVy[id] = 0;
    }
  }

  void applyForces() {
    const double repulsionForce = 1000.0;
    const double springLength = 100.0;
    const double springStrength = 0.1;

    std::unordered_map<int, double> NodeIdToFx;
    std::unordered_map<int, double> NodeIdToFy;
    for (int id = 0; id < G.get_node_count(); id++) {
      NodeIdToFx[id] = 0;
      NodeIdToFy[id] = 0;
    }

    for (int i = 0; i < G.get_node_count(); i++) {
      for (int j = i + 1; j < G.get_node_count(); j++) {
        double dx = NodeIdToX[i] - NodeIdToX[j];
        double dy = NodeIdToY[i] - NodeIdToY[j];
        double distance = std::sqrt(dx * dx + dy * dy) + 0.1;
        double force = repulsionForce / (distance * distance);

        NodeIdToFx[i] += (dx / distance) * force;
        NodeIdToFy[i] += (dy / distance) * force;
        NodeIdToFx[j] -= (dx / distance) * force;
        NodeIdToFy[j] -= (dy / distance) * force;
      }
    }

    for (int u = 0; u < G.get_node_count(); u++) {
      for (int v : G.adj()[u]) {
        double dx = NodeIdToX[u] - NodeIdToX[v];
        double dy = NodeIdToY[u] - NodeIdToY[v];
        double distance = std::sqrt(dx * dx + dy * dy) + 0.1;

        double displacement = distance - springLength;
        double force = springStrength * displacement;

        NodeIdToFx[u] += (dx / distance) * force;
        NodeIdToFy[u] += (dy / distance) * force;
        NodeIdToFx[v] -= (dx / distance) * force;
        NodeIdToFy[v] -= (dy / distance) * force;
      }
    }

    for (int id = 0; id < G.get_node_count(); id++) {
      NodeIdToVx[id] = NodeIdToVx[id] * 0.9 + NodeIdToFx[id];
      NodeIdToVy[id] = NodeIdToVy[id] * 0.9 + NodeIdToFy[id];
    }
  }

  void updatePositions() {
    const double step = 0.01; // Small step size to prevent explosion
    for (int id = 0; id < G.get_node_count(); id++) {
      // Clamp velocity to prevent runaway values
      if (NodeIdToVx[id] > 10) NodeIdToVx[id] = 10;
      if (NodeIdToVx[id] < -10) NodeIdToVx[id] = -10;
      if (NodeIdToVy[id] > 10) NodeIdToVy[id] = 10;
      if (NodeIdToVy[id] < -10) NodeIdToVy[id] = -10;

      NodeIdToX[id] += NodeIdToVx[id] * step;
      NodeIdToY[id] += NodeIdToVy[id] * step;
    }
  }

  void simulate(int iteration) {
    initializer();

    for (int i = 0; i < iteration; i++) {
      applyForces();
      updatePositions();
    }
  }
  std::unordered_map<int, std::pair<double, double>> getPositions() {
    std::unordered_map<int, std::pair<double, double>> positions;
    for (int id = 0; id < G.get_node_count(); id++) {
      positions[id] = {NodeIdToX[id], NodeIdToY[id]};
    }
    return positions;
  }
};
