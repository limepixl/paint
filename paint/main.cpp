#include "stroke.h"
#include "button.h"

int main()
{
	// Window setup
	const int WIDTH = 1280;
	const int HEIGHT = 720;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Paint");
	window.setFramerateLimit(60);

	int brushSize = 10;
	sf::Color brushColor(0, 0, 0);

	Stroke currentStroke;
	std::vector<Stroke> brushStrokes;

	// Interactable buttons
	std::vector<Button> buttons;
	InitializeButtons(buttons, WIDTH, HEIGHT);

	// Render loop
	while(window.isOpen())
	{
		window.clear(sf::Color::White);

		sf::Event e;
		while(window.pollEvent(e))
		{
			if(e.type == sf::Event::Closed)
				window.close();

			// Starting a new brush stroke
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				// Get center of brush
				sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);

				// If picking a color
				bool colorPicked = false;
				for(auto& b : buttons)
					if(IsPointWithinBox(b.buttonShape, mousePos) && !currentStroke.currentlyBeingDrawn)
					{
						colorPicked = true;
						brushColor = b.buttonShape.getFillColor();
						break;
					}

				if(!colorPicked)
				{
					currentStroke.line.append(sf::Vertex(mousePos, brushColor));
					currentStroke.currentlyBeingDrawn = true;

					int vertexCount = (int)currentStroke.line.getVertexCount();
					if(vertexCount < 2)
						continue;
					sf::Vertex& first = currentStroke.line[(size_t)vertexCount - 2];
					sf::Vertex& second = currentStroke.line[(size_t)vertexCount - 1];

					LineWithThickness tempPart(first.position, second.position, brushColor, (float)brushSize);
					currentStroke.parts.push_back(tempPart);

					float r = (float)brushSize * 0.5f;
					sf::CircleShape tempJoint(r);
					tempJoint.setOrigin(r, r);
					tempJoint.setPosition(second.position);
					tempJoint.setFillColor(brushColor);
					currentStroke.joints.push_back(tempJoint);
				}
			}
			
			// Current brush stroke has ended or the window has lost focus, store the stroke
			if((e.type == sf::Event::MouseButtonReleased && e.key.code == sf::Mouse::Left && currentStroke.currentlyBeingDrawn) || e.type == sf::Event::LostFocus)
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
				}
			}
		}

		for(auto& s : brushStrokes)	// Draw all stored brush strokes
		{
			for(auto& j : s.joints)
				window.draw(j);
			for(auto& p : s.parts)
				window.draw(p);
		}

		if(currentStroke.currentlyBeingDrawn)	// Draw the current brush stroke
		{
			for(auto& j : currentStroke.joints)
				window.draw(j);
			for(auto& p : currentStroke.parts)
				window.draw(p);
		}

		for(auto& b : buttons)				// Draw buttons on top of canvas
			window.draw(b.buttonShape);

		window.display();
	}
}