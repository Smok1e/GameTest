#pragma once

//------------------------

#include <SFML/Graphics.hpp>
#include "ObjectType.h"

//------------------------

class Game;

//------------------------

class Object
{
public:
	Object ();
	Object (sf::Vector2f pos, sf::Vector2f vel, sf::Color color, double r);
	Object (const Object& copy);

	virtual void         setPosition (sf::Vector2f position);
	virtual sf::Vector2f getPosition ();

	virtual void         setVelocity (sf::Vector2f velocity);
	virtual sf::Vector2f getVelocity ();

	virtual void      setColor (sf::Color color);
	virtual sf::Color getColor ();

	virtual void   setRadius (double radius);
	virtual double getRadius ();

	virtual void draw   (sf::RenderWindow* window);
	virtual void move   (sf::Rect <double> bounds);
	virtual void update ();

	virtual bool testCollision (Object* that);
	virtual void hit           (Object* that);

	virtual void  setGame (Game* game);
	virtual Game* getGame ();

	virtual std::string getDebugString ();

	virtual bool processEvent (const sf::Event& event);
	virtual bool onMouseMoved (const sf::Event& event);

	virtual ObjectType type ();

	static void   SetSpeedModifier (double modifier);
	static double GetSpeedModifier ();

protected:
	Game* m_game;

	sf::Vector2f m_pos;
	sf::Vector2f m_vel;

	sf::Color m_color;

	double m_r;

	static double s_speed_modifier;

};

//------------------------