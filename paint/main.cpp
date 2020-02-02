#include "stroke.h"
#include "button.h"

int main()
{
	// Window setup
	const int WIDTH = 1280;
	const int HEIGHT = 720;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Canvas", sf::Style::Close);
	window.setFramerateLimit(60);
	sf::Vector2i windowPos = window.getPosition();

	int brushSize = 10;
	sf::Color brushColor(0, 0, 0);

	Stroke currentStroke;
	std::vector<Stroke> brushStrokes;

	// Interactable buttons
	std::vector<Button> buttons;
	InitializeButtons(buttons, WIDTH, HEIGHT);

	// Mouse brush outline
	sf::CircleShape brushOutline(brushSize * 0.5f);
	brushOutline.setFillColor(brushColor);
	brushOutline.setOrigin(brushSize * 0.5f, brushSize * 0.5f);

	// Create a palette window
	int paletteSize = (int)buttons[0].buttonShape.getSize().x;
	sf::RenderWindow window2(sf::VideoMode(paletteSize, HEIGHT), "Palette", sf::Style::None);
	window2.setFramerateLimit(60);
	window2.setPosition(windowPos + sf::Vector2i(-paletteSize, 30));

	while(window.isOpen())
	{
		window.clear(sf::Color::White);

		// Cursor position relative to screen
		sf::Vector2f mousePos1 = (sf::Vector2f)sf::Mouse::getPosition(window);
		brushOutline.setPosition(mousePos1);

		sf::Event e;
		while(window.pollEvent(e))
		{
			if(e.type == sf::Event::Closed)
				window.close();

			// Starting a new brush stroke
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && window.hasFocus())
			{
				currentStroke.line.append(sf::Vertex(mousePos1, brushColor));
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
			
			// Current brush stroke has ended or the window has lost focus, store the stroke
			if((e.type == sf::Event::MouseButtonReleased && e.key.code == sf::Mouse::Left && currentStroke.currentlyBeingDrawn))
			{
				currentStroke.currentlyBeingDrawn = false;
				if(window.hasFocus())
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

				float r = brushSize * 0.5f;
				brushOutline.setRadius(r);
				brushOutline.setOrigin(r, r);

				if(!currentStroke.currentlyBeingDrawn)
				{
					// Undo
					if(e.key.control && e.key.code == sf::Keyboard::Z && !brushStrokes.empty())
						brushStrokes.erase(brushStrokes.end() - 1);
				}
			}
		}

		if(windowPos != window.getPosition())
		{
			windowPos = window.getPosition();
			window2.setPosition(windowPos + sf::Vector2i(-paletteSize, 30));
		}

		while(window2.pollEvent(e))
		{
			// Palette button click events
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				sf::Vector2f mousePos2((float)e.mouseMove.x, (float)e.mouseMove.y);

				for(auto& b : buttons)
				if(IsPointWithinBox(b.buttonShape, mousePos2) && !currentStroke.currentlyBeingDrawn)
				{
					brushColor = b.buttonShape.getFillColor();
					brushOutline.setFillColor(brushColor);
					break;
				}
			}

			// Duplicate code because undo and resizing the brush needs to work
			// regardless of which window has focus
			if(e.type == sf::Event::KeyPressed)
			{
				// Increase / decrease brush size
				if(e.key.code == sf::Keyboard::Add)
					brushSize += 5;
				else if(e.key.code == sf::Keyboard::Subtract)
					brushSize -= 5;

				float r = brushSize * 0.5f;
				brushOutline.setRadius(r);
				brushOutline.setOrigin(r, r);

				if(!currentStroke.currentlyBeingDrawn)
				{
					// Undo
					if(e.key.control && e.key.code == sf::Keyboard::Z && !brushStrokes.empty())
						brushStrokes.erase(brushStrokes.end() - 1);
				}
			}
		}

		for(auto& s : brushStrokes)		// Draw all stored brush strokes
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

		window.draw(brushOutline);
		window.display();

		window2.clear(sf::Color::White);
		for(auto& b : buttons)				// Draw buttons in palette window
			window2.draw(b.buttonShape);
		window2.display();
	}
}