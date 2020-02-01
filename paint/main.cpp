#include <SFML/Graphics.hpp>
#include "utility.h"

int main()
{
	const int WIDTH = 800;
	const int HEIGHT = 600;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Paint");
	window.setFramerateLimit(60);

	int brushSize = 10;
	sf::Color brushColor(0, 0, 0);

	Stroke currentStroke;
	std::vector<Stroke> brushStrokes;

	while(window.isOpen())
	{
		window.clear(sf::Color::White);

		sf::Event e;
		while(window.pollEvent(e))
		{
			if(e.type == sf::Event::Closed)
				window.close();
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				currentStroke.currentlyBeingDrawn = true;

				// Get center of brush
				sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
				currentStroke.line.append(sf::Vertex(mousePos, brushColor));

				int vertexCount = (int)currentStroke.line.getVertexCount();
				if(vertexCount < 2)
					continue;
				sf::Vertex& first = currentStroke.line[vertexCount - 2];
				sf::Vertex& second = currentStroke.line[vertexCount - 1];

				LineWithThickness tempPart(first.position, second.position, brushColor, (float)brushSize);
				currentStroke.parts.push_back(tempPart);

				float r = (float)brushSize * 0.5f;
				sf::CircleShape tempJoint(r);
				tempJoint.setOrigin(r, r);
				tempJoint.setPosition(second.position);
				tempJoint.setFillColor(brushColor);
				currentStroke.joints.push_back(tempJoint);
			} 
			
			if((e.type == sf::Event::MouseButtonReleased && e.key.code == sf::Mouse::Left) || e.type == sf::Event::LostFocus)
			{
				currentStroke.currentlyBeingDrawn = false;
				brushStrokes.push_back(currentStroke);
				currentStroke.line.clear();
				currentStroke.parts.clear();
				currentStroke.joints.clear();
			}

			if(e.type == sf::Event::KeyPressed)
			{
				// Increase / decrease brush size
				if(e.key.code == sf::Keyboard::Add)
					brushSize += 5;
				else if(e.key.code == sf::Keyboard::Subtract)
					brushSize -= 5;

				if(!currentStroke.currentlyBeingDrawn)
				{
					// Undo
					if(e.key.control && e.key.code == sf::Keyboard::Z && !brushStrokes.empty())
						brushStrokes.erase(brushStrokes.end() - 1);

					// Change brush color
					if(e.key.code == sf::Keyboard::Num1)
						brushColor = sf::Color::Black;
					else if(e.key.code == sf::Keyboard::Num2)
						brushColor = sf::Color::Red;
					else if(e.key.code == sf::Keyboard::Num3)
						brushColor = sf::Color::Blue;
				}
			}
		}

		for(auto& s : brushStrokes)
		{
			for(auto& j : s.joints)
				window.draw(j);
			for(auto& p : s.parts)
				window.draw(p);
		}

		if(currentStroke.currentlyBeingDrawn)
		{
			for(auto& j : currentStroke.joints)
				window.draw(j);
			for(auto& p : currentStroke.parts)
				window.draw(p);
		}

		window.display();
	}
}