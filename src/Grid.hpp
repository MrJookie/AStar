#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <unordered_map>

enum Type { EMPTY, OBSTACLE, START, FINISH };
enum Status { FREE, OPENED, CLOSED };

class Tile
{
public:
	Type type;
	Status status;
	sf::Vertex* quad;
	sf::Vector2i position;

	int Fvalue;

	sf::Text textG; //movement cost
	sf::Text textH; //heuristic distance
	sf::Text textF; //F = G + H

	bool operator==(const Tile& other) const
	{
		return position == other.position;
	}
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

	std::vector<Tile> openList;
	std::vector<Tile> closedList;

	void addToOpenList(Tile tile, int Fvalue);
	void addToClosedList(Tile tile);

	Tile findLeastF();
	int calculateMovementCost(sf::Vector2i tile);
	int calculateHeuristicManhattan(sf::Vector2i startTilePosition, sf::Vector2i finishTilePosition);
};