#include "Grid.hpp"

Grid::Grid(sf::Vector2i windowSize, std::size_t tileWidth, std::size_t tileHeight) :
width(windowSize.x / tileWidth),
height(windowSize.y / tileWidth),
tileWidth(tileWidth),
tileHeight(tileHeight)
{
	vertices.setPrimitiveType(sf::Quads);
	vertices.resize(width * height * 4);

	tiles.resize(width);
	for (int i = 0; i < height; ++i)
		tiles[i].resize(height);

	for (std::size_t x = 0; x < width; ++x)
	{
		for (std::size_t y = 0; y < height; ++y)
		{
			sf::Vertex* quad = &vertices[(x + y * width) * 4];

			quad[0].color = pickTileColor(Type::EMPTY);
			quad[1].color = pickTileColor(Type::EMPTY);
			quad[2].color = pickTileColor(Type::EMPTY);
			quad[3].color = pickTileColor(Type::EMPTY);

			quad[0].position = sf::Vector2f((x + 0) * tileWidth + 1, (y + 0) * tileHeight + 1);
			quad[1].position = sf::Vector2f((x + 1) * tileWidth + 0, (y + 0) * tileHeight + 1);
			quad[2].position = sf::Vector2f((x + 1) * tileWidth + 0, (y + 1) * tileHeight + 0);
			quad[3].position = sf::Vector2f((x + 0) * tileWidth + 1, (y + 1) * tileHeight + 0);

			tiles[x][y].type = Type::EMPTY;
			tiles[x][y].quad = quad;
			tiles[x][y].position = sf::Vector2i(x, y);
		}
	}

	//obstacles
	colorTile(sf::Vector2i(10, 8), Type::OBSTACLE);
	colorTile(sf::Vector2i(10, 9), Type::OBSTACLE);
	colorTile(sf::Vector2i(10, 10), Type::OBSTACLE);
	colorTile(sf::Vector2i(10, 11), Type::OBSTACLE);
	colorTile(sf::Vector2i(10, 12), Type::OBSTACLE);
	colorTile(sf::Vector2i(10, 13), Type::OBSTACLE);
	colorTile(sf::Vector2i(10, 14), Type::OBSTACLE);
	colorTile(sf::Vector2i(10, 15), Type::OBSTACLE);
	colorTile(sf::Vector2i(10, 16), Type::OBSTACLE);
	colorTile(sf::Vector2i(10, 17), Type::OBSTACLE);
	colorTile(sf::Vector2i(10, 18), Type::OBSTACLE);
	colorTile(sf::Vector2i(10, 19), Type::OBSTACLE);
	colorTile(sf::Vector2i(10, 20), Type::OBSTACLE);

	//start
	colorTile(sf::Vector2i(5, 14), Type::START);

	//finish
	colorTile(sf::Vector2i(15, 14), Type::FINISH);

	pather = new micropather::MicroPather(this, 20);
}

Grid::~Grid() {
	delete pather;
}

void Grid::reset()
{
	for (std::size_t x = 0; x < width; ++x)
	{
		for (std::size_t y = 0; y < height; ++y)
		{
			tiles[x][y].type = Type::EMPTY;

			tiles[x][y].quad[0].color = pickTileColor(Type::EMPTY);
			tiles[x][y].quad[1].color = pickTileColor(Type::EMPTY);
			tiles[x][y].quad[2].color = pickTileColor(Type::EMPTY);
			tiles[x][y].quad[3].color = pickTileColor(Type::EMPTY);
		}
	}

	path.resize(0);

	startTilePosition.x = 0;
	startTilePosition.y = 0;
	finishTilePosition.x = 0;
	finishTilePosition.y = 0;
}

void Grid::onMouseButtonPressedLeft(sf::RenderWindow& window)
{
	sf::Vector2i position(sf::Mouse::getPosition(window));
	sf::Vector2i tile(std::floor(position.x / tileWidth), std::floor(position.y / tileHeight));

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		colorTile(tile, Type::OBSTACLE);
}

void Grid::onMouseButtonPressedRight(sf::RenderWindow& window)
{
	sf::Vector2i position(sf::Mouse::getPosition(window));
	sf::Vector2i tile(std::floor(position.x / tileWidth), std::floor(position.y / tileHeight));

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		Type type = Type::EMPTY;

		bool startDrawn = false;
		bool finishDrawn = false;

		for (std::size_t x = 0; x < width; ++x)
		{
			for (std::size_t y = 0; y < height; ++y)
			{
				if (tiles[x][y].type == Type::START)
					startDrawn = true;

				if (tiles[x][y].type == Type::FINISH)
					finishDrawn = true;
			}
		}

		if ((!startDrawn && !finishDrawn) || (!startDrawn && finishDrawn))
			type = Type::START;
		else if (startDrawn && !finishDrawn)
			type = Type::FINISH;
		else
			type = Type::EMPTY;

		colorTile(tile, type);
	}
}

sf::Color Grid::pickTileColor(Type type)
{
	switch (type)
	{
	case Type::EMPTY:
		return sf::Color(204, 204, 204, 255);
		break;
	case Type::OBSTACLE:
		return sf::Color(102, 102, 102, 255);
		break;
	case Type::START:
		return sf::Color(51, 204, 0, 255);
		break;
	case Type::FINISH:
		return sf::Color(51, 102, 204, 255);
		break;
	case Type::PATH:
		return sf::Color(255, 0, 0, 255);
		break;
	}
}

void Grid::colorTile(sf::Vector2i tile, Type type)
{
	if (tile.x >= width || tile.x < 0 || tile.y >= height || tile.y < 0)
		return;

	if (type == Type::START)
	{
		startTilePosition.x = tile.x;
		startTilePosition.y = tile.y;
	}
	else if (type == Type::FINISH)
	{
		finishTilePosition.x = tile.x;
		finishTilePosition.y = tile.y;
	}
	else if (tiles[tile.x][tile.y].type == Type::START)
	{
		type = Type::EMPTY;
		startTilePosition.x = 0;
		startTilePosition.y = 0;
	}
	else if (tiles[tile.x][tile.y].type == Type::FINISH)
	{
		type = Type::EMPTY;
		finishTilePosition.x = 0;
		finishTilePosition.y = 0;
	}
	else if (type == Type::PATH)
	{
		type = Type::PATH;
	}

	tiles[tile.x][tile.y].type = type;

	tiles[tile.x][tile.y].quad[0].color = pickTileColor(type);
	tiles[tile.x][tile.y].quad[1].color = pickTileColor(type);
	tiles[tile.x][tile.y].quad[2].color = pickTileColor(type);
	tiles[tile.x][tile.y].quad[3].color = pickTileColor(type);
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//draw tiles
	states.transform *= getTransform();
	target.draw(vertices, states);
}

void Grid::startPathfinding()
{
	for (std::size_t x = 0; x < width; ++x)
	{
		for (std::size_t y = 0; y < height; ++y)
		{
			if (tiles[x][y].type == Type::PATH)
			{
				tiles[x][y].type = Type::EMPTY;

				tiles[x][y].quad[0].color = pickTileColor(Type::EMPTY);
				tiles[x][y].quad[1].color = pickTileColor(Type::EMPTY);
				tiles[x][y].quad[2].color = pickTileColor(Type::EMPTY);
				tiles[x][y].quad[3].color = pickTileColor(Type::EMPTY);
			}
		}
	}
	
	float totalCost = 0;
	int result = pather->Solve(XYToNode(startTilePosition.x, startTilePosition.y), XYToNode(finishTilePosition.x, finishTilePosition.y), &path, &totalCost);

	//if (result == micropather::MicroPather::SOLVED)
	//	std::cout << "totalCost: " << totalCost << std::endl;

	for (int i = 0; i < path.size(); ++i)
	{
		int x, y;
		NodeToXY(path[i], &x, &y);
		if (i != 0 && i != path.size() - 1)
		{
			colorTile(sf::Vector2i(x, y), Type::PATH);
		}
	}

	pather->Reset();
}

bool Grid::Passable(int nx, int ny)
{
	//fix this
	if (nx >= 0 && nx < width &&
		ny >= 0 && ny < height)
	{
		if (tiles[nx][ny].type != Type::OBSTACLE)
			return true;
	}

	return false;
}

void Grid::NodeToXY(void* node, int* x, int* y)
{
	intptr_t index = (intptr_t)node;
	*y = index / width;
	*x = index - *y * width;
}

void* Grid::XYToNode(int x, int y)
{
	return (void*)(y*width + x);
}

float Grid::LeastCostEstimate(void* nodeStart, void* nodeEnd)
{
	int xStart, yStart, xEnd, yEnd;
	NodeToXY(nodeStart, &xStart, &yStart);
	NodeToXY(nodeEnd, &xEnd, &yEnd);

	/* Compute the minimum path cost using distance measurement. It is possible
	to compute the exact minimum path using the fact that you can move only
	on a straight line or on a diagonal, and this will yield a better result.
	*/
	int dx = xStart - xEnd;
	int dy = yStart - yEnd;
	return (float)sqrt((double)(dx*dx) + (double)(dy*dy));
}

void Grid::AdjacentCost(void* node, micropather::MPVector<micropather::StateCost>* neighbors)
{
	int x, y;
	const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	const int cost[8] = { 10, 14, 10, 14, 10, 14, 10, 14 };

	NodeToXY(node, &x, &y);

	for (int i = 0; i < 8; ++i) {
		int nx = x + dx[i];
		int ny = y + dy[i];

		if (Passable(nx, ny)) {
			switch (i)
			{
			case 1:
				//std::cout << "diagonal down left" << std::endl;
				if (Passable(nx - 1, ny) || Passable(nx, ny - 1))
				{
					micropather::StateCost nodeCost = { XYToNode(nx, ny), cost[i] };
					neighbors->push_back(nodeCost);
				}
				break;

			case 3:
				//std::cout << "diagonal down left" << std::endl;
				if (Passable(nx + 1, ny) || Passable(nx, ny - 1))
				{
					micropather::StateCost nodeCost = { XYToNode(nx, ny), cost[i] };
					neighbors->push_back(nodeCost);
				}
				break;

			case 5:
				//std::cout << "diagonal up left" << std::endl;
				if (Passable(nx + 1, ny) || Passable(nx, ny + 1))
				{
					micropather::StateCost nodeCost = { XYToNode(nx, ny), cost[i] };
					neighbors->push_back(nodeCost);
				}
				break;

			case 7:
				//std::cout << "diagonal up right" << std::endl;
				if (Passable(nx - 1, ny) || Passable(nx, ny + 1))
				{
					micropather::StateCost nodeCost = { XYToNode(nx, ny), cost[i] };
					neighbors->push_back(nodeCost);
				}
				break;

			default:
				{
					micropather::StateCost nodeCost = { XYToNode(nx, ny), cost[i] };
					neighbors->push_back(nodeCost);
				}
				break;
			}
		}
	}
}

void Grid::PrintStateInfo(void* node)
{
}