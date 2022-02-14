#pragma once

//------------------------

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

//------------------------

class ResourceManager
{
public:
	struct file_pair
	{
		const char* filename;
		int         id;
	};

	template <typename Type>
	struct resource_pair
	{
		resource_pair (int id):
			resource (),
			id       (id)
		{}

		resource_pair (const resource_pair <Type>& copy):
			resource (copy.resource),
			id       (copy.id)
		{}

		Type resource;
		int  id;
	};

	ResourceManager ();

	void load (
		const std::initializer_list <file_pair>& images, 
		const std::initializer_list <file_pair>& sounds, 
		const std::initializer_list <file_pair>& fonts 
	);

	sf::Image*       getImage  (int id);
	sf::SoundBuffer* getSound  (int id);
	sf::Font*        getFont   (int id);

protected:
	std::vector <resource_pair <sf::Image      >> m_images;
	std::vector <resource_pair <sf::SoundBuffer>> m_sounds; 
	std::vector <resource_pair <sf::Font       >> m_fonts;

};

//------------------------