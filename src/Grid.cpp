#include "Grid.hpp"

Grid::Grid(sf::Vector2i windowSize, std::size_t tileWidth, std::size_t tileHeight) :
width(windowSize.x / tileWidth),
height(windowSize.y / tileWidth),
tileWidth(tileWidth),
tileHeight(tileHeight)
{
	font.loadFromFile("Sansation.ttf");

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

			tiles[x][y].textG.setFont(font);
			tiles[x][y].textG.setCharacterSize(10u);
			tiles[x][y].textG.setColor(sf::Color(0, 0, 0, 255));
			tiles[x][y].textG.setPosition(x * tileWidth + 2, y * tileHeight + 1);

			tiles[x][y].textH.setFont(font);
			tiles[x][y].textH.setCharacterSize(10u);
			tiles[x][y].textH.setColor(sf::Color(0, 0, 0, 255));
			tiles[x][y].textH.setPosition(x * tileWidth + 2, y * tileHeight + 11);

			tiles[x][y].textF.setFont(font);
			tiles[x][y].textF.setCharacterSize(10u);
			tiles[x][y].textF.setColor(sf::Color(0, 0, 0, 255));
			tiles[x][y].textF.setPosition(x * tileWidth + 2, y * tileHeight + 21);
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
}

void Grid::reset()
{
	for (std::size_t x = 0; x < width; ++x)
	{
		for (std::size_t y = 0; y < height; ++y)
		{
			tiles[x][y].type = Type::EMPTY;

			tiles[x][y].textG.setString("");
			tiles[x][y].textH.setString("");
			tiles[x][y].textF.setString("");

			tiles[x][y].quad[0].color = pickTileColor(Type::EMPTY);
			tiles[x][y].quad[1].color = pickTileColor(Type::EMPTY);
			tiles[x][y].quad[2].color = pickTileColor(Type::EMPTY);
			tiles[x][y].quad[3].color = pickTileColor(Type::EMPTY);
		}
	}
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

	if (tiles[tile.x][tile.y].type == Type::START)
	{
		tiles[tile.x][tile.y].type == Type::EMPTY;
		type = Type::EMPTY;
		startTilePosition.x = 0;
		startTilePosition.y = 0;
	}
	else if (tiles[tile.x][tile.y].type == Type::FINISH)
	{
		tiles[tile.x][tile.y].type == Type::EMPTY;
		type = Type::EMPTY;
		finishTilePosition.x = 0;
		finishTilePosition.y = 0;
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

	//draw texts
	//draw for expanded tiles
	for (std::size_t x = 0; x < width; ++x)
	{
		for (std::size_t y = 0; y < height; ++y)
		{
			target.draw(tiles[x][y].textG, states);
			target.draw(tiles[x][y].textH, states);
			target.draw(tiles[x][y].textF, states);
		}
	}
}

void Grid::startPathfinding()
{
	std::cout << heuristicManhattan(startTilePosition, finishTilePosition) << std::endl;
}

void Grid::recalculateCosts()
{
	/*
	//only recalc discovered
	for (std::size_t x = 0; x < width; ++x)
	{
		for (std::size_t y = 0; y < height; ++y)
		{
			//tiles[x][y].textG.setString("");
			//tiles[x][y].textH.setString("");
			//tiles[x][y].textF.setString("");
		}
	}
	*/

	//tiles[startTilePosition.x][startTilePosition.y].textH.setString("H:" + std::to_string(heuristicManhattan(startTilePosition, finishTilePosition)));
}

int Grid::heuristicManhattan(sf::Vector2i startTilePosition, sf::Vector2i finishTilePosition)
{
	return abs(startTilePosition.x - finishTilePosition.x) + abs(startTilePosition.y - finishTilePosition.y);
}