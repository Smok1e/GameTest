#include "ObjectManager.h"
#include "Logging.h"

//------------------------

ObjectManager::ObjectManager ():
	m_objects ()
{
	Trace ("Object manager constructed");
}

ObjectManager::~ObjectManager ()
{
	clear ();

	Trace ("Object manager destructed");
}

//------------------------

void ObjectManager::addObject (Object* object)
{
	Trace ("Adding object %p", object);

	#ifdef _DEBUG
		if (std::find (m_objects.begin (), m_objects.end (), object) != m_objects.end ()) DebugMessage ("Trying to add an existing object");
	#endif

	m_objects.push_back (object);
}

void ObjectManager::deleteObject (Object* object)
{
	Trace ("Deleting object %p", object);

	auto iter = std::find (m_objects.begin (), m_objects.end (), object);
	if (iter != m_objects.end ())
	{
		delete[] (*iter);
		*iter = nullptr;
	}

	else Trace ("Failed to delete object: object does not exist");
}

//------------------------

void ObjectManager::drawObjects (sf::RenderWindow* window)
{
	size_t size = m_objects.size ();				
	for (size_t i = 0; i < size; i++)
		if (m_objects[i]) m_objects[i] -> draw (window);
}

//------------------------

void ObjectManager::moveObjects (sf::Rect <double> bounds)
{
	size_t size = m_objects.size ();				
	for (size_t i = 0; i < size; i++)
		if (m_objects[i]) m_objects[i] -> move (bounds);
}

//------------------------

void ObjectManager::updateObjects ()
{
	size_t size = m_objects.size ();
	for (size_t i = 0; i < size; i++)
		if (m_objects[i]) m_objects[i] -> update ();
}

//------------------------

void ObjectManager::removeDeletedObjects ()
{
	bool running = true;
	while (running)
	{
		auto iter = std::find_if (m_objects.begin (), m_objects.end (), [](auto object) { return !object; } );
		if (iter == m_objects.end ())
			running = false;

		else 
		{
			Trace ("Removing object %p", *iter);
			m_objects.erase (iter);
		}
	}
}

//------------------------

void ObjectManager::processCollision ()
{
	size_t count = m_objects.size ();
	for (size_t x = 0; x < count; x++)
	{
		for (size_t y = x+1; y < count; y++)
		{
			if (!m_objects[x] || !m_objects[y]) continue;

			if (m_objects[x] -> testCollision (m_objects[y]))
				m_objects[x] -> hit           (m_objects[y]);
		}
	}
}

//------------------------

void ObjectManager::clear ()
{
	Trace ("Deleting all objects");

	for (auto& object: m_objects)
		delete[] (object);

	m_objects.clear ();
}

//------------------------

bool ObjectManager::processEvent (const sf::Event& event)
{
	for (auto& object: m_objects)
		if (object && object -> processEvent (event)) return true;

	return false;
}

//------------------------

std::vector <Object*>& ObjectManager::data ()
{
	return m_objects;
}

//------------------------