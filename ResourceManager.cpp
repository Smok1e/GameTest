#include "ResourceManager.h"
#include "Logging.h"

//------------------------

ResourceManager::ResourceManager ():
	m_images  (),
	m_sounds  (),
	m_fonts   ()
{
	Trace ("Resource manager constructed");
}

//------------------------

void ResourceManager::load (
	const std::initializer_list <file_pair>& images, 
	const std::initializer_list <file_pair>& sounds, 
	const std::initializer_list <file_pair>& fonts 
)

{
	Trace ("Loading resources...");

	#define LOAD_(type, list) 						         \
	for (auto& file: list)									 \
	{														 \
		resource_pair <type> resource (file.id);	         \
															 \
		Trace ("Loading %s...", file.filename);				 \
		resource.resource.loadFromFile (file.filename);		 \
                                                             \
		m_##list.push_back (resource);						 \
	}

	LOAD_ (sf::Image,       images );
	LOAD_ (sf::SoundBuffer, sounds );
	LOAD_ (sf::Font,        fonts  );

	#undef LOAD_
}

//------------------------

sf::Image* ResourceManager::getImage (int id)
{
	auto iter = std::find_if (m_images.begin (), m_images.end (), [id](const auto& elem) { return elem.id == id; } );
	if (iter == m_images.end ())
		return nullptr;

	return &iter -> resource;
}

sf::SoundBuffer* ResourceManager::getSound (int id)
{
	auto iter = std::find_if (m_sounds.begin (), m_sounds.end (), [id](const auto& elem) { return elem.id == id; } );
	if (iter == m_sounds.end ())
		return nullptr;

	return &iter -> resource;
}

sf::Font* ResourceManager::getFont (int id)
{
	auto iter = std::find_if (m_fonts.begin (), m_fonts.end (), [id](const auto& elem) { return elem.id == id; } );
	if (iter == m_fonts.end ())
		return nullptr;

	return &iter -> resource;
}

//------------------------