#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Micropather/Micropather.h"

enum Type { EMPTY, OBSTACLE, START, FINISH, PATH };

class Tile
{
public:
	Type type;
	sf::Vertex* quad;
	sf::Vector2i position;
};

class Grid : public sf::Drawable, public sf::Transformable, public micropather::Graph
{
public:
	Grid(sf::Vector2i windowSize, std::size_t tileWidth, std::size_t tileHeight);
	virtual ~Grid();
	void reset();
	void onMouseButtonPressedLeft(sf::RenderWindow& window);
	void onMouseButtonPressedRight(sf::RenderWindow& window);
	void startPathfinding();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual float LeastCostEstimate(void* stateStart, void* stateEnd);
	virtual void AdjacentCost(void* state, MP_VECTOR<micropather::StateCost>* adjacent);
	virtual void PrintStateInfo(void* state);

private:
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

	micropather::MPVector<void*> path;
	micropather::MicroPather* pather;
	bool Passable(int nx, int ny);
	void NodeToXY(void* node, int* x, int* y);
	void* XYToNode(int x, int y);
};