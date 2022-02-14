#pragma once

#include "Object.h"

//------------------------

class Food: public Object
{
public:
	Food ();
	Food (sf::Vector2f pos, double counter_limit);
	Food (sf::Vector2f pos, sf::Color color, double counter_limit);
	Food (const Food& copy);

	virtual void reset ();

	virtual void   setCounterLimit (double limit);
	virtual double getCounterLimit ();

	virtual void update ();
	virtual void draw   (sf::RenderWindow* window);

	virtual ObjectType type ();

	virtual std::string getDebugString ();

	static void SetSpawnRange (int range);
	static int  GetSpawnRange ();

protected:
	double m_counter;
	double m_counter_limit;

	static int s_spawn_range;

};

//------------------------