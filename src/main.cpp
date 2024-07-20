#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>

#include <stdio.h>
#include "SUI.h"
#include "Grid.h"


int main(void)
{
	//vars
	enum States { ACTIVE, IDLE };
	unsigned const int FPS = 30;
	Grid Maze(28, 32, sf::Vector2f(0.0f, 0.0f));
	


	//font
	sf::Font font;
	font.loadFromFile("src/OpenSans.ttf");
	//window
	sf::RenderWindow win(sf::VideoMode(1100, 800), "Maze Gen", sf::Style::Close);
	//window elements
	sf::RectangleShape DisplayPanel(sf::Vector2f(700.0f,800.0f));
	DisplayPanel.setFillColor(sf::Color::Black);
	DisplayPanel.setPosition(sf::Vector2f(0.0f, 0.0f));
	sf::RectangleShape OptionsPanel(sf::Vector2f(400.0f, 800.0f));
	OptionsPanel.setFillColor(sf::Color::Blue);
	OptionsPanel.setPosition(sf::Vector2f(700.0f, 0.0f));

	SUI::Button Generate(sf::Vector2f(300.0f, 100.0f), sf::Vector2f(750.0f, 500.0f));
	Generate.setFont(font);
	Generate.setTextString("Generate Maze");
	Generate.setColor(sf::Color::White);
	Generate.setTextColor(sf::Color::Black);


	while (win.isOpen())
	{
		//event handling
		sf::Event event;
		while (win.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::MouseMoved:
				if (Generate.isHovered(win))
				{
					Generate.setColor(sf::Color::Cyan);
				}
				else
				{
					Generate.setColor(sf::Color::White);
				}
				break;
			case sf::Event::MouseButtonPressed:
				if (Generate.isHovered(win))
				{
					Maze.generateMaze();
				}
				break;

			
			case sf::Event::Closed:
				win.close();
				break;
			}
		}
		//element drawing
		win.clear();
		win.draw(DisplayPanel);
		win.draw(OptionsPanel);
		//display buttons
		Generate.drawTo(win);

		//display grid
		Maze.drawGrid(win);

		win.display();
	}


}
