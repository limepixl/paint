#pragma once

inline float Dist(const sf::Vector2f& first, const sf::Vector2f& second)
{
	float distX = first.x - second.x;
	float distY = first.y - second.y;
	return sqrtf(distX * distX + distY * distY);
}

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

struct Button
{
	sf::RectangleShape buttonShape;
};

void InitializeButtons(std::vector<Button>& buttons, int width)
{
	std::vector<sf::Color> colors
	{
		sf::Color::Red,
		sf::Color(255, 165, 0, 255),
		sf::Color::Yellow,
		sf::Color(200, 255, 0, 255),
		sf::Color::Green,
		sf::Color(0, 244, 165),
		sf::Color::Cyan,
		sf::Color(0, 184, 255),
		sf::Color::Blue,
		sf::Color(111, 0, 255),
		sf::Color(212, 0, 212),
		sf::Color::Magenta,
		sf::Color(122, 63, 4),
		sf::Color::Black,
		sf::Color(128, 128, 128),
		sf::Color::White
	};

	int numColors = (int)colors.size();
	float buttonSize = width / (float)numColors;
	sf::RectangleShape temp(sf::Vector2f(buttonSize, buttonSize));
	for(int i = 0; i < numColors; i++)
	{
		temp.setPosition(i * buttonSize, 0.0f);
		temp.setFillColor(colors[i]);

		buttons.push_back({ temp });
	}
}

bool IsPointWithinBox(sf::RectangleShape& box, sf::Vector2f& pos)
{
	sf::Vector2f boxStart = box.getPosition();
	sf::Vector2f boxDimensions = box.getSize();

	return (pos.x >= boxStart.x && pos.x <= boxStart.x + boxDimensions.x) && (pos.y <= boxStart.y + boxDimensions.y && pos.y >= boxStart.y);
}
