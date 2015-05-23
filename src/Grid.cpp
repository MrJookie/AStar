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

			quad[0].color = sf::Color(204, 204, 204, 255);
			quad[1].color = sf::Color(204, 204, 204, 255);
			quad[2].color = sf::Color(204, 204, 204, 255);
			quad[3].color = sf::Color(204, 204, 204, 255);

			quad[0].position = sf::Vector2f((x + 0) * tileWidth + 1, (y + 0) * tileHeight + 1);
			quad[1].position = sf::Vector2f((x + 1) * tileWidth + 0, (y + 0) * tileHeight + 1);
			quad[2].position = sf::Vector2f((x + 1) * tileWidth + 0, (y + 1) * tileHeight + 0);
			quad[3].position = sf::Vector2f((x + 0) * tileWidth + 1, (y + 1) * tileHeight + 0);

			tiles[x][y].type = 0;
			tiles[x][y].quad = quad;
		}
	}

	//obstacles
	colorTile(sf::Vector2i(10, 8), 1);
	colorTile(sf::Vector2i(10, 9), 1);
	colorTile(sf::Vector2i(10, 10), 1);
	colorTile(sf::Vector2i(10, 11), 1);
	colorTile(sf::Vector2i(10, 12), 1);
	colorTile(sf::Vector2i(10, 13), 1);
	colorTile(sf::Vector2i(10, 14), 1);
	colorTile(sf::Vector2i(10, 15), 1);
	colorTile(sf::Vector2i(10, 16), 1);
	colorTile(sf::Vector2i(10, 17), 1);
	colorTile(sf::Vector2i(10, 18), 1);
	colorTile(sf::Vector2i(10, 19), 1);
	colorTile(sf::Vector2i(10, 20), 1);

	//start
	colorTile(sf::Vector2i(5, 14), 2);

	//finish
	colorTile(sf::Vector2i(15, 14), 3);
}

void Grid::reset()
{
	for (std::size_t x = 0; x < width; ++x)
	{
		for (std::size_t y = 0; y < height; ++y)
		{
			tiles[x][y].type = 0;

			tiles[x][y].quad[0].color = sf::Color(204, 204, 204, 255);
			tiles[x][y].quad[1].color = sf::Color(204, 204, 204, 255);
			tiles[x][y].quad[2].color = sf::Color(204, 204, 204, 255);
			tiles[x][y].quad[3].color = sf::Color(204, 204, 204, 255);
		}
	}
}

void Grid::onMouseButtonPressedLeft(sf::RenderWindow& window)
{
	sf::Vector2i position(sf::Mouse::getPosition(window));
	sf::Vector2i tile(std::floor(position.x / tileWidth), std::floor(position.y / tileHeight));

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		colorTile(tile, 1);
}

void Grid::onMouseButtonPressedRight(sf::RenderWindow& window)
{
	sf::Vector2i position(sf::Mouse::getPosition(window));
	sf::Vector2i tile(std::floor(position.x / tileWidth), std::floor(position.y / tileHeight));

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		std::size_t type = 0;
		sf::Color color;

		bool startDrawn = false;
		bool finishDrawn = false;

		for (std::size_t x = 0; x < width; ++x)
		{
			for (std::size_t y = 0; y < height; ++y)
			{
				if (tiles[x][y].type == 2)
					startDrawn = true;

				if (tiles[x][y].type == 3)
					finishDrawn = true;
			}
		}

		if ((!startDrawn && !finishDrawn) || (!startDrawn && finishDrawn))
			type = 2;
		else if (startDrawn && !finishDrawn)
			type = 3;
		else
			type = 0;

		colorTile(tile, type);
	}
}

void Grid::colorTile(sf::Vector2i tile, std::size_t type)
{
	if (tile.x >= width || tile.x < 0 || tile.y >= height || tile.y < 0)
		return;

	sf::Color color;

	switch (type)
	{
	case 0:
		color.r = 204;
		color.g = 204;
		color.b = 204;
		color.a = 255;
		break;
	case 1:
		color.r = 102;
		color.g = 102;
		color.b = 102;
		color.a = 255;
		break;
	case 2:
		color.r = 51;
		color.g = 204;
		color.b = 0;
		color.a = 255;
		break;
	case 3:
		color.r = 51;
		color.g = 102;
		color.b = 204;
		color.a = 255;
		break;
	}

	tiles[tile.x][tile.y].type = type;

	tiles[tile.x][tile.y].quad[0].color = color;
	tiles[tile.x][tile.y].quad[1].color = color;
	tiles[tile.x][tile.y].quad[2].color = color;
	tiles[tile.x][tile.y].quad[3].color = color;
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(vertices, states);
}