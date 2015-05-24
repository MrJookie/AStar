#include "Pathfinding.hpp"

inline int Pathfinding::heuristicManhattan(sf::Vector2i startTilePosition, sf::Vector2i finishTilePosition)
{
	return abs(startTilePosition.x - finishTilePosition.x) + abs(startTilePosition.y - finishTilePosition.y);
}