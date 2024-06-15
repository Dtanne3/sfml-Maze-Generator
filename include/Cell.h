#pragma once
#include <SFML/Graphics.hpp>

#ifndef Cell_H
#define Cell_H

class Cell
{
private:
	enum State {EMPTY, WALL};
	int currentState = EMPTY;
	sf::RectangleShape body;

public:
	Cell(sf::Vector2f pos);
	void clear();
	void toggleState();
	bool isWall();
	void drawBody(sf::RenderWindow &win);
};

#endif // !Cell_H
