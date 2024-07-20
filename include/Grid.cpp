#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <cmath>
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
			//temporary solution
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
Grid::Grid(int numOfCol, int numOfRows, sf::Vector2f startingPos)
{
	
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
	//select random cell from column 0
	//find the up/down/left/right neighbors of cell and compute frontier cells (2 spaces ahead of chosen cell)
	//add frontier cells to a list
	//while list of frontier cells is not empty:
	/*
		-pick random frontier cell from list
		-toggle state of the cell in-between (AKA bridge cell)
		-compute frontier cells of the chosen frontier cell and add them to the list of frontier cells
		-remove chosen cell from the list
	*/
	this->clearGrid();


	srand(time(0));
	std::vector<std::vector<Cell*>> frontiers; //contains frontier cells and their neighbor
	int row = rand() % rows;
	int col = rand() % cols;
	
	Cell* startNode = &map.at(row).at(col);
	startNode->toggleState();
	
	inputFrontiers(map, frontiers, *startNode);

	while (frontiers.size() > 0)
	{
		
		//pick random frontier cell
		int index = rand() % frontiers.size();
		Cell* c = frontiers.at(index).at(0); 
		std::vector<int> sCoords = getCellCoordinates(map, *c);
		row = sCoords.at(0);
		col = sCoords.at(1);

		//get bridge cell
		std::vector<int> bCoords = getCellCoordinates(map, *frontiers.at(index).at(1));
		int x = bCoords.at(0);
		int y = bCoords.at(1);

		try 
		{
			//issue: does not properly store the neighbor of the frontier cells
			Cell* mid;
			int dir;

			if (x == row)
			{
				printf("same row");
				dir = (col - y) / 2;
				printf("(%i, %i)", x, y + dir);
				mid = &map.at(x).at(y + dir);
			}
			else
			{
				printf("same col");
				dir = (row - x) / 2;
				printf("(%i, %i)", x + dir, y);
				mid = &map.at(x + dir).at(y);
			}
			if (mid->isWall() && c->isWall())
			{
				mid->toggleState();
				c->toggleState();
			}
			//input frontier cells
			inputFrontiers(map, frontiers, *c);
		}
		catch (...)
		{ }
		
		frontiers.erase(frontiers.begin() + index);
		printf("%i\n", frontiers.size());
	}
	printf("Done generating maze");
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