#pragma once
#include <SFML/Graphics.hpp>

// Source: https://github.com/SFML/SFML/wiki/Source:-Line-segment-with-thickness
struct LineWithThickness : public sf::Drawable
{
	LineWithThickness(const sf::Vector2f& p1, const sf::Vector2f& p2, sf::Color color, float thickness) : thickness(thickness)
	{
		sf::Vector2f dir = p2 - p1;
		sf::Vector2f unitDir = dir / sqrtf(dir.x * dir.x + dir.y * dir.y);
		sf::Vector2f rightAngleUnitDir(-unitDir.y, unitDir.x);
		sf::Vector2f brushOffset = (thickness * 0.5f) * rightAngleUnitDir;

		vertices[0].position = p1 + brushOffset;
		vertices[1].position = p2 + brushOffset;
		vertices[2].position = p2 - brushOffset;
		vertices[3].position = p1 - brushOffset;

		for(auto& v : vertices)
			v.color = color;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(vertices, 4, sf::Quads);
	}

public:
	sf::Vertex vertices[4];
	float thickness;
};

struct Stroke
{
	bool currentlyBeingDrawn = false;
	sf::VertexArray line;
	std::vector<LineWithThickness> parts;
	std::vector<sf::CircleShape> joints;
};