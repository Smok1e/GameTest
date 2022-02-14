#define _USE_MATH_DEFINES

#include "Object.h"
#include <cmath>

//------------------------

class Player: public Object
{
public:
	Player ();
	Player (sf::Vector2f pos);
	Player (const Player& player);

	virtual void draw   (sf::RenderWindow* window);
	virtual void update ();

	virtual void      setBorderColor (sf::Color color);
	virtual sf::Color getBorderColor ();

	ObjectType type ();

	static void         SetMaxVelocity (sf::Vector2f velocity);
	static sf::Vector2f GetMaxVelocity ();

protected:
	static sf::Vector2f s_max_velocity;

	sf::Color m_border_color;

};

//------------------------