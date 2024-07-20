#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Cell.h"


class Grid
{
private:
	std::vector<std::vector<Cell>> map;
	int rows;
	int cols;
public:
	Grid(int numOfCol, int numOfRows, sf::Vector2f startingPos);
	void clearGrid();
	void generateMaze();
	void drawGrid(sf::RenderWindow& win);
};