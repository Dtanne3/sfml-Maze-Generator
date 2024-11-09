#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <random>
#include <chrono>
#include "Cell.h"
#include "Grid.h"

//private functions
Cell* findCell(std::vector<std::vector<Cell>>& m, int x, int y)
{
	Cell* c = NULL;
	if (x >= 0 && x < m.size())
	{
		if (y >= 0 && y < m[x].size())
		{
			c = &m[x][y];
			if (!c -> isWall())
			{
				return NULL;
			}
		}
	}
	return c;
}
std::vector<int> getCellCoordinates(std::vector<std::vector<Cell>> &m, Cell &c)
{
	std::vector<int> r {-1, -1};
	for (int i = 0; i < m.size(); i++)
	{
		for (int j = 0; j < m.at(i).size(); j++)
		{
			if (&m[i][j] == &c)
			{
				r[0] = i;
				r[1] = j;
				return r;
			}
		}
	}
	return r;
}
void inputFrontiers(std::vector<std::vector<Cell>>& m, std::vector<std::vector<Cell*>>& fronts, Cell& prev)
{
	//inputs a vector that contains the frontier cell and the cell that it's attached to
	std::vector<int> coords = getCellCoordinates(m, prev);
	int dir = 1;
	for (int i = 0; i < 2; i++)
	{
		Cell* chosenCell = findCell(m,coords[0] - (2 * dir), coords[1]);
		std::vector<Cell*> f = { chosenCell, &prev };
		if (chosenCell != NULL && std::find(fronts.begin(), fronts.end(), f) == fronts.end())
		{
			fronts.push_back(f);
		}
		dir *= -1;
	}
	for (int i = 0; i < 2; i++)
	{
		Cell* chosenCell = findCell(m, coords[0], coords[1] - (2 * dir));
		std::vector<Cell*> f = { chosenCell,&prev };
		if (chosenCell != NULL && std::find(fronts.begin(), fronts.end(), f) == fronts.end())
		{
			fronts.push_back(f);
		}
		dir *= -1;
	}
}

//Class functions
Grid::Grid(int numOfCol, int numOfRows, sf::Vector2f startingPos, sf::RenderWindow* win)
{
	this->win = win;

	cols = numOfCol;
	rows = numOfRows;

	float xCoord = startingPos.x;
	float yCoord = startingPos.y;
	for (int i = 0; i < rows; i++)
	{
		std::vector<Cell> v;
		for (int j = 0; j < cols; j++)
		{
			Cell c(sf::Vector2f(xCoord, yCoord));
			v.push_back(c);
			xCoord += 25.0f;
		}
		map.push_back(v);
		yCoord += 25.0f;
		xCoord = startingPos.x;
	}
}
void Grid::clearGrid()
{
	for (int i = 0; i < rows; i++)
	{
		for (Cell &c : map.at(i))
		{
			c.clear();
		}
	}
}
void Grid::generateMaze()
{
	//Uses a variation of Prim's Algorithm
	this->clearGrid();

	typedef std::chrono::high_resolution_clock clock;
	clock::time_point start = clock::now();

	std::vector<std::vector<Cell*>> frontiers; 
	std::minstd_rand gen1;
	//get random index
	std::uniform_int_distribution<int> randRow(1, rows - 2), randCol(1, cols - 2);

	clock::duration d = clock::now() - start;
	gen1.seed(d.count());
	int row = randRow(gen1);
	int col = randCol(gen1);

	
	Cell* startNode = &map.at(row).at(col);
	startNode->toggleState();
	
	inputFrontiers(map, frontiers, *startNode);
	
	while (frontiers.size() > 0)
	{
		
		std::uniform_int_distribution<int> randFrontier(0, frontiers.size()-1);
		
		int index = randFrontier(gen1);
		Cell* c = frontiers.at(index).at(0);
		std::vector<int> sCoords = getCellCoordinates(map, *c);
		row = sCoords.at(0);
		col = sCoords.at(1);

		//get bridge cell
		Cell* prev = frontiers.at(index).at(1);
		std::vector<int> bCoords = getCellCoordinates(map, *prev);
		int x = bCoords.at(0);
		int y = bCoords.at(1);

		try 
		{
			//set neighbor of base cell to bridge, then set neighbor of bridge cell to frontier cell
			Cell* mid;
			int dir;

			if (x == row)
			{
				dir = (col - y) / 2;
				mid = &map.at(x).at(y + dir);
			}
			else
			{
				dir = (row - x) / 2;
				mid = &map.at(x + dir).at(y);
			}
			
			if (mid->isWall() && c->isWall())
			{
				mid->toggleState();


				c->toggleState();
			}
			
			prev->neighbors.push_back(mid);
			mid->neighbors.push_back(c);

			inputFrontiers(map, frontiers, *c);
		}
		catch (...)
		{ }
		frontiers.erase(frontiers.begin() + index);
	}
	printf("Done generating maze\n");

}



void Grid::drawGrid()
{
	for (std::vector<Cell> v : map)
	{
		for (Cell& c : v)
		{
			c.drawBody(*win);
		}
	}
}