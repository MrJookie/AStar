#include <SFML/Graphics.hpp>
#include <iostream>

#include "Grid.hpp"

int main()
{
	sf::Vector2i windowSize(512, 512);

	sf::RenderWindow window;
	//windowSize.x + 1 to keep border lines
	window.create(sf::VideoMode(windowSize.x + 1, windowSize.y + 1), "A* SFML", sf::Style::Close | sf::Style::Titlebar);

	//window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(true);
	//window.setVerticalSyncEnabled(true);

	sf::Clock clock;

	Grid grid(windowSize, 16, 16);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed: window.close(); break;
				case sf::Event::KeyPressed: 
					if (event.key.code == sf::Keyboard::Escape)
					{
						window.close();
						break;
					}
					if (event.key.code == sf::Keyboard::Space)
					{
						grid.reset();
						break;
					}
					/*
					if (event.key.code == sf::Keyboard::S)
					{
						grid.startPathfinding();
						break;
					}
					*/
				case sf::Event::MouseButtonPressed: grid.onMouseButtonPressedRight(window);

				default: break;
			}
		}

		if (sf::Mouse::isButtonPressed)
		{
			grid.onMouseButtonPressedLeft(window);
		}

		grid.startPathfinding();

		sf::Time elapsed = clock.restart();
		float dt = elapsed.asSeconds();

		window.clear();

		window.draw(grid);

		window.display();

		window.setTitle("A* SFML " + std::to_string(1.f / dt));
	}

    return 0;
}
