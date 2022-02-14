#pragma once

#include "Object.h"

//------------------------

class PlayerParticle: public Object
{
public:
	PlayerParticle ();
	PlayerParticle (sf::Vector2f pos, sf::Vector2f vel);
	PlayerParticle (sf::Vector2f pos, sf::Vector2f vel, sf::Color color);
	PlayerParticle (const PlayerParticle& copy);
	
	virtual void update ();
	virtual void draw   (sf::RenderWindow* window);

	virtual std::string getDebugString ();

	void   setCounter (double value);
	double getCounter ();

	ObjectType type ();

	static void SetCounterLimit (int limit);
	static int  GetCounterLimit ();

protected:
	double        m_counter;
	static double s_counter_limit;

};

//------------------------