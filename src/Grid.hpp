#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "Pathfinding.hpp"

class Tile
{
public:
	sf::Vertex* quad;

	/*
	type:
	0 = empty
	1 = obstacle
	2 = start
	3 = finish
	*/
	std::size_t type;
};

class Grid : public sf::Drawable, public sf::Transformable
{
public:
	Grid(sf::Vector2i windowSize, std::size_t tileWidth, std::size_t tileHeight);
	void reset();
	void onMouseButtonPressedLeft(sf::RenderWindow& window);
	void onMouseButtonPressedRight(sf::RenderWindow& window);
	
	std::vector<std::vector<Tile>> tiles;
	 
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::size_t width;
	std::size_t height;
	std::size_t tileWidth;
	std::size_t tileHeight;

	sf::VertexArray vertices;

	void colorTile(sf::Vector2i tile, std::size_t type);

	Pathfinding pathfinding;
};