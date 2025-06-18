#pragma once
#include <SFML/Graphics.hpp>;
#include <stack>


template <typename GraphType>
class PathShape : public sf::Shape{
public:
    using NodeId = typename GraphType::NodeIdType;
private:
    sf::Vector2f m_radius;
    std::stack<NodeId> path;
    const GraphType& G;
public:
    PathShape(const GraphType& g):G(g){

    }

};
