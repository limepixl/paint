#include <SFML/Graphics.hpp>

inline float Dist(const sf::Vector2f& first, const sf::Vector2f& second)
{
	float distX = first.x - second.x;
	float distY = first.y - second.y;
	return sqrtf(distX * distX + distY * distY);
}

struct Stroke
{
	int size;
	sf::VertexArray line;
	std::vector<sf::RectangleShape> rectangles;
};

int main()
{
	const int WIDTH = 800;
	const int HEIGHT = 600;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Paint");
	window.setFramerateLimit(60);

	int brushSize = 10;
	std::vector<Stroke> brushStrokes;
	sf::Color brushColor(0, 0, 0);

	Stroke currentStroke;

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
				// Get center of brush
				sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
				currentStroke.line.append(sf::Vertex(mousePos, brushColor));
				currentStroke.size = brushSize;

				int vertexCount = (int)currentStroke.line.getVertexCount();
				if(vertexCount < 2)
					continue;
				sf::Vertex& first = currentStroke.line[(size_t)(vertexCount - 2)];
				sf::Vertex& second = currentStroke.line[(size_t)(vertexCount - 1)];

				float w = Dist(first.position, second.position) + 1.0f;
				float h = (float)brushSize;
				sf::RectangleShape temp(sf::Vector2f(w, h));
				temp.setPosition(first.position);
				temp.rotate(-57.2957795f * atan2(abs(second.position.y - first.position.y), abs(second.position.x - first.position.x)));
				temp.setOrigin(w * 0.5f, h * 0.5f);
				temp.setFillColor(brushColor);
				currentStroke.rectangles.push_back(temp);

				for(auto& r : currentStroke.rectangles)
					window.draw(r);
			} else if(e.type == sf::Event::MouseButtonReleased && e.key.code == sf::Mouse::Left)
			{
				brushStrokes.push_back(currentStroke);
				currentStroke.line.clear();
				currentStroke.rectangles.clear();
			}

			if(e.type == sf::Event::KeyPressed)
			{
				if(e.key.code == sf::Keyboard::Num1)
					brushColor = sf::Color::Black;
				else if(e.key.code == sf::Keyboard::Num2)
					brushColor = sf::Color::Red;
			}
		}

		for(auto& s : brushStrokes)	// BUG: Rectangles draw in wrong order
			for(auto& r : s.rectangles)
				window.draw(r);

		window.display();
	}
}