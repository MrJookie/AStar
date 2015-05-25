#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

enum Type { EMPTY, OBSTACLE, START, FINISH };

class Tile
{
public:
	Type type;
	sf::Vertex* quad;
	sf::Vector2i position;
	sf::Text textG; //movement cost
	sf::Text textH; //heuristic distance
	sf::Text textF; //F = G + H
};

class Grid : public sf::Drawable, public sf::Transformable
{
public:
	Grid(sf::Vector2i windowSize, std::size_t tileWidth, std::size_t tileHeight);
	void reset();
	void onMouseButtonPressedLeft(sf::RenderWindow& window);
	void onMouseButtonPressedRight(sf::RenderWindow& window);
	void startPathfinding();
	void recalculateCosts();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Font font;

	std::size_t width;
	std::size_t height;
	std::size_t tileWidth;
	std::size_t tileHeight;
	sf::VertexArray vertices;
	sf::Vector2i startTilePosition;
	sf::Vector2i finishTilePosition;
	std::vector<std::vector<Tile>> tiles;

	sf::Color Grid::pickTileColor(Type type);
	void colorTile(sf::Vector2i tile, Type type);

	int heuristicManhattan(sf::Vector2i startTilePosition, sf::Vector2i finishTilePosition);
};