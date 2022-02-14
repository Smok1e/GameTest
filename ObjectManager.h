#pragma once

//------------------------

#include <SFML/Graphics.hpp>
#include <vector>
#include "Object.h"

//------------------------

class ObjectManager
{
public:
	 ObjectManager ();
	~ObjectManager ();

	void addObject    (Object* object);
	void deleteObject (Object* object);

	void drawObjects          (sf::RenderWindow* window);
	void moveObjects          (sf::Rect <double> bounds);
	void updateObjects        ();
	void removeDeletedObjects ();
	void processCollision     ();

	void clear ();

	bool processEvent (const sf::Event& event);

	std::vector <Object*>& data ();

private:
	std::vector <Object*> m_objects;

};

//------------------------