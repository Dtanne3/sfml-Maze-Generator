#include <SFML/Graphics.hpp>
#include <vector>
#include "Cell.h"
#include "Grid.h"

Grid::Grid(int numOfCol, int numOfRows, sf::Vector2f startingPos)
{
	
	col = numOfCol;
	rows = numOfRows;

	float xCoord = startingPos.x;
	float yCoord = startingPos.y;
	for (int i = 0; i < rows; i++)
	{
		std::vector<Cell> v;
		for (int j = 0; j < col; j++)
		{
			Cell c(sf::Vector2f(xCoord, yCoord));
			v.push_back(c);
			xCoord += 50.0f;
		}
		map.push_back(v);
		yCoord += 50.0f;
		xCoord = startingPos.x;
	}
}
void Grid::clearGrid()
{
	for (int i = 0; i < rows; i++)
	{
		for (Cell c : map[rows])
		{
			c.clear();
		}
	}
}
void Grid::generateMaze()
{
	
}

void Grid::drawGrid(sf::RenderWindow& win)
{
	for (std::vector<Cell> v : map)
	{
		for (Cell& c : v)
		{
			c.drawBody(win);
		}
	}
}