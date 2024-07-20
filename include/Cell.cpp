#include <SFML/Graphics.hpp>
#include "Cell.h"

Cell::Cell(sf::Vector2f pos)
{
	body.setSize(sf::Vector2f(25.0f, 25.0f));
	body.setPosition(pos);
	body.setFillColor(sf::Color::Black);
}
void Cell::toggleState()
{
	currentState = (currentState == EMPTY) ? currentState = WALL : currentState = EMPTY;
	if (currentState == WALL)
	{
		body.setFillColor(sf::Color::Black);
	}
	else
	{
		body.setFillColor(sf::Color::White);
	}
}
bool Cell::isWall()
{
	return currentState == WALL;
}
void Cell::clear()
{
	currentState = WALL;
	body.setFillColor(sf::Color::Black);
}
void Cell::drawBody(sf::RenderWindow &win)
{
	win.draw(body);
}