#pragma once
#include <SFML/Graphics.hpp>

struct Button
{
	sf::RectangleShape buttonShape;
};

void InitializeButtons(std::vector<Button>& buttons, int width, int height)
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
	float buttonSize = height / (float)numColors;
	sf::RectangleShape temp(sf::Vector2f(buttonSize, buttonSize));
	for(int i = 0; i < numColors; i++)
	{
		temp.setPosition(0.0f, i * buttonSize);
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
