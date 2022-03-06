#include "Game.h"
#include "Logging.h"
#include "Collision.h"
#include "Utils.hpp"

//------------------------

struct ball
{
	sf::CircleShape shape;
	sf::Vector2f    position;
	double          radius;
	sf::Color       color;

	void draw (sf::RenderWindow& window)
	{
		shape.setFillColor        (sf::Color::Transparent);
		shape.setOutlineColor     (color);
		shape.setOutlineThickness (4);
		shape.setPosition         (position - sf::Vector2f (radius, radius));
		shape.setRadius           (radius);

		window.draw (shape);
	}
};

//------------------------

int testmain ()
{
	ball ball_1 = {};
	ball ball_2 = {};
	ball ball_3 = {};

	ball_1.color = sf::Color::Red;
	ball_2.color = sf::Color::Blue;

	ball_1.position = sf::Vector2f (100, 100);
	ball_2.position = sf::Vector2f (170, 100);

	ball_1.radius = 50;
	ball_2.radius = 20;

	sf::CircleShape point;
	point.setRadius    (3);
	point.setFillColor (sf::Color::White);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;

	sf::RenderWindow window (sf::VideoMode (1000, 1000), "Test", sf::Style::Default, settings);

	while (window.isOpen ())
	{
		sf::Event event;
		while (window.pollEvent (event))
		{
			switch (event.type)
			{
				case sf::Event::Closed: window.close (); break;
				default: break;
			}
		}

		ball_2.position = sf::Vector2f (sf::Mouse::getPosition (window).x, sf::Mouse::getPosition (window).y);

		window.clear (sf::Color (24, 24, 24));

		ball_1.draw (window);
		ball_2.draw (window);

		point.setPosition (GetHitPoint (ball_1.position, ball_1.radius, ball_2.position));
		window.draw (point);

		window.display ();
	}
}

//------------------------

int main ()
{
	Game game;

	try
	{
		game.run ();
	}

	catch (std::exception exc)
	{
		DebugMessage ("Unhandled exception thrown: %s\n", exc);
	}

	catch (...)
	{
		DebugMessage ("Unknown exception");
	}

	return 0;
}

//------------------------