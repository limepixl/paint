#include <SFML/Graphics.hpp>

inline float Dist(const sf::Vector2f& first, const sf::Vector2f& second)
{
	float distX = first.x - second.x;
	float distY = first.y - second.y;
	return sqrtf(distX * distX + distY * distY);
}

struct LineWithThickness : public sf::Drawable
{
	LineWithThickness(const sf::Vector2f& p1, const sf::Vector2f& p2, sf::Color color, float thickness)
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
	bool currentlyBeingDrawn;
	sf::VertexArray line;
	std::vector<LineWithThickness> parts;
	std::vector<sf::CircleShape> joints;
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

			} else if(e.type == sf::Event::MouseButtonReleased && e.key.code == sf::Mouse::Left)
			{
				currentStroke.currentlyBeingDrawn = false;
				brushStrokes.push_back(currentStroke);
				currentStroke.line.clear();
				currentStroke.parts.clear();
				currentStroke.joints.clear();
			}

			if(e.type == sf::Event::KeyPressed)
			{
				if(e.key.code == sf::Keyboard::Num1)
					brushColor = sf::Color::Black;
				else if(e.key.code == sf::Keyboard::Num2)
					brushColor = sf::Color::Red;
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