#include "Object.h"
#include "Logging.h"
#include "Utils.hpp"
#include "Collision.h"

//------------------------

double Object::s_speed_modifier = 1;

//------------------------

Object::Object ():
	m_game  (nullptr),
	m_pos   (0, 0),
	m_vel   (0, 0),
	m_color (sf::Color::White),
	m_r     (10)
{
	Trace ("Base object constructed with default constructor");
}

Object::Object (sf::Vector2f pos, sf::Vector2f vel, sf::Color color, double r):
	m_game  (nullptr),
	m_pos   (pos),
	m_vel   (vel),
	m_color (color),
	m_r     (r)
{
	Trace ("Base object cunstructed with full contructor");
}

Object::Object (const Object& copy):
	m_game  (copy.m_game),
	m_pos   (copy.m_pos),
	m_vel   (copy.m_vel),
	m_color (copy.m_color),
	m_r     (copy.m_r)
{
	Trace ("Base object cunstructed with copy constructors");
}

//------------------------

void Object::setPosition (sf::Vector2f position)
{
	m_pos = position;
}

sf::Vector2f Object::getPosition ()
{
	return m_pos;
}

//------------------------

void Object::setVelocity (sf::Vector2f velocity)
{
	m_vel = velocity;
}

sf::Vector2f Object::getVelocity ()
{
	return m_vel;
}

//------------------------

void Object::setColor (sf::Color color)
{
	m_color = color;
}

sf::Color Object::getColor ()
{
	return m_color;
}

//------------------------

void Object::setRadius (double radius)
{
	m_r = radius;
}

double Object::getRadius ()
{
	return m_r; 
}

//------------------------

void Object::setGame (Game* game)
{
	m_game = game;
}

Game* Object::getGame ()
{
	return m_game;
}

//------------------------

std::string Object::getDebugString ()
{
	return Format ("position: {%.2lf, %.2lf}\nvelocity: {%.2lf, %.2lf}", m_pos.x, m_pos.y, m_vel.x, m_vel.y);
}

//------------------------

void Object::draw (sf::RenderWindow* window)
{

}

//------------------------

void Object::move (sf::Rect <double> bounds)
{
	sf::Vector2f vel (m_vel.x * s_speed_modifier, m_vel.y * s_speed_modifier);

	if (m_pos.x + vel.x < bounds.left || m_pos.x + vel.x + m_r*2 >= bounds.left + bounds.width ) m_vel.x *= -1;
	if (m_pos.y + vel.y < bounds.top  || m_pos.y + vel.y + m_r*2 >= bounds.top  + bounds.height) m_vel.y *= -1;

	m_pos += vel;

	m_pos.x = Clamp (m_pos.x, bounds.left, bounds.left + bounds.width  - m_r*2);
	m_pos.y = Clamp (m_pos.y, bounds.top,  bounds.top  + bounds.height - m_r*2);
}

//------------------------

void Object::update ()
{
	
}

//------------------------

bool Object::testCollision (Object* that)
{
	double distance_x = (that -> m_pos.x + that -> m_r) - (m_pos.x + m_r);
	double distance_y = (that -> m_pos.y + that -> m_r) - (m_pos.y + m_r);

	double distance = sqrt (distance_x*distance_x + distance_y*distance_y);

	return distance < (m_r + that -> m_r);
}

//------------------------

void Object::hit (Object* that)
{
	Hit (this, that);
}

//------------------------

bool Object::processEvent (const sf::Event& event)
{
	#define PROCESS_EVENT_(type, proc) case type: if (proc (event)) { return true; } break;

	switch (event.type)
	{
		PROCESS_EVENT_ (sf::Event::MouseMoved, onMouseMoved);

		default: break;
	}

	return false;

	#undef PROCESS_EVENT_
}

//------------------------

bool Object::onMouseMoved (const sf::Event& event)
{
	return false;
}

//------------------------

ObjectType Object::type ()
{
	return ObjectType::Base;
}

//------------------------

void Object::SetSpeedModifier (double modifier)
{
	s_speed_modifier = modifier;
}

double Object::GetSpeedModifier ()
{
	return s_speed_modifier;
}

//------------------------