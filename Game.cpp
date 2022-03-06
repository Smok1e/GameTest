#include "Game.h"
#include "Logging.h"
#include "Player.h"
#include "PlayerParticle.h"
#include "Food.h"
#include "Utils.hpp"
#include "RoundRect.h"
#include <Windows.h>

//------------------------

const char* Game::s_data_file = "data.bin";

//------------------------

Game::Game ():
	m_window            (),
	m_manager           (),
	m_resources         (),
	m_background        (sf::Color (24, 24, 24)),
	m_running           (false),
	m_last_display_time (0),
	m_debug_mode        (false),
	m_framerate_limit   (0),
	m_vertical_sync     (false),
	m_objects_info      (true),
	m_objects_lines     (false),
	m_antialiasing      (8),
	m_paused            (false),
	m_draw_background   (true),
	m_speed             (1.0),
	m_score             (0),
	m_temp              (1),
	m_godmode           (false),
	m_last_score        (0)
{
	Trace ("Game constructed");
}

//------------------------

void Game::init ()
{
	Trace ("Initializing game");

	m_resources.load (
		// Images
		{
			{"Resources/Images/Test.jpg", ImageBackground}
		},

		// Sounds
		{
			{"Resources/Sounds/Eat1.ogg",  SoundEat1 },
			{"Resources/Sounds/Eat2.ogg",  SoundEat2 },
			{"Resources/Sounds/Eat3.ogg",  SoundEat3 },
			{"Resources/Sounds/Eat4.ogg",  SoundEat4 },
			{"Resources/Sounds/Eat5.ogg",  SoundEat5 },
			{"Resources/Sounds/Eat6.ogg",  SoundEat6 },
			{"Resources/Sounds/Eat7.ogg",  SoundEat7 },
			{"Resources/Sounds/Death.ogg", SoundDeath},
			{"Resources/Sounds/Hit.ogg",   SoundHit  }
		},

		// Fonts
		{
			{"Resources/Fonts/consolas.ttf", FontConsolas}
		}
	);

	sf::ContextSettings settings;
	settings.antialiasingLevel = m_antialiasing;

	m_window.create (sf::VideoMode (1000, 1000), "Game", sf::Style::Titlebar | sf::Style::Close, settings);
	
	ShowWindow (GetConsoleWindow (), SW_SHOW);
	SetActiveWindow     (m_window.getSystemHandle ());
	SetForegroundWindow (m_window.getSystemHandle ());

	loadData ();

	SetTraceAllowed (false);

	PlayerParticle::SetCounterLimit (25);
	Food::SetSpawnRange (std::min (m_window.getSize ().x, m_window.getSize ().y) / 2);

	setVerticalSyncEnabled (true);
	setFramerateLimit      (0);

	addObject (new Player (sf::Vector2f (m_window.getSize ().x / 2, m_window.getSize ().y / 2)));

	Food* food = new Food ();
	addObject (food);
	food -> reset           ();
	food -> setCounterLimit (1000);

	m_running = true;
}

//------------------------

void Game::run ()
{
	init ();

	Trace ("Starting game");

	sf::Clock clock;
	while (m_running)
	{
		Object::SetSpeedModifier (m_speed * (1.0 / 8.0 * getDisplayTime ()));
		Player::SetMaxVelocity (sf::Keyboard::isKeyPressed (sf::Keyboard::LShift)? sf::Vector2f (15, 15): sf::Vector2f (5, 5));

		drawBackground ();
		drawScore      ();

		if (!m_paused)
		{
			m_manager.moveObjects      (getBounds ());
			m_manager.updateObjects    ();
			m_manager.processCollision ();
		}

		m_manager.drawObjects          (&m_window);
		m_manager.removeDeletedObjects ();

		sf::Event event;
		while (m_window.pollEvent (event))
		{
			          processEvent (event);
			m_manager.processEvent (event);
		}
		
		if (m_debug_mode)
			drawDebugInfo ();

		if (m_paused)
		{
			static sf::Text text;
			text.setFont          (*m_resources.getFont (FontConsolas));
			text.setString        ("Paused");
			text.setCharacterSize (50);
			text.setStyle         (sf::Text::Bold);
			text.setFillColor     (sf::Color::White);
			text.setPosition      (5, 5);

			m_window.draw (text);
		}

		if (sf::Mouse::isButtonPressed (sf::Mouse::Right))
		{
			PlayerParticle* object = new PlayerParticle (getMousePosition (), sf::Vector2f (0, 0));
			object -> setRadius (10);
			object -> setColor  (HSV (rand () % 256, 255, 255));

			addObject (object);
		}

		m_window.display ();

		m_last_display_time = clock.getElapsedTime ().asMilliseconds ();
		clock.restart ();
	}

	m_window.close ();
}

//------------------------

bool Game::isRunning ()
{
	return m_running;
}

//------------------------

void Game::stop ()
{
	Trace ("Stopping game");
	saveData ();
	m_running = false;
}

//------------------------

void Game::drawBackground ()
{
	if (m_draw_background)
		m_window.clear (m_background);
}

//------------------------

void Game::drawDebugInfo ()
{
	static const int indent = 5;

	static sf::RoundedRectangleShape rect;
	static sf::Text                  text;

	text.setFont          (*m_resources.getFont (FontConsolas));
	text.setCharacterSize (20);
	text.setStyle         (sf::Text::Bold);

	sf::Vector2u window_size = m_window.getSize ();

	if (m_objects_info)
	{
		for (auto& object: m_manager.data ())
		{
			if (!object) continue;

			std::string string = object -> getDebugString ();
			if (string.empty ()) continue;

			text.setString    (Format ("[%s]\n%s", StrObjectType (object -> type ()), string.c_str ()));
			text.setFillColor (object -> getColor ());

			auto text_bounds = text.getGlobalBounds ();

			sf::Vector2f position = object -> getPosition () + sf::Vector2f (-text_bounds.width / 2 + object -> getRadius (), object -> getRadius () * 2.5);
			position.x = Clamp (position.x, indent, window_size.x - text_bounds.width  - indent);
			position.y = Clamp (position.y, indent, window_size.y - text_bounds.height - indent);

			rect.setCornerPointCount (10);
			rect.setCornersRadius    (10);
			rect.setPosition         (position.x - indent, position.y);
			rect.setSize             (sf::Vector2f (text_bounds.width + indent*2, text_bounds.height + indent*2));
			rect.setFillColor        (sf::Color (0, 0, 0, 100));

			text.setPosition (position);

			m_window.draw (rect);
			m_window.draw (text);
		}
	}

	if (m_objects_lines)
	{
		static sf::Vertex points[2] = {};
		
		points[0] = getMousePosition ();
		for (auto& object: m_manager.data ())
		{
			if (!object) continue;

			points[1] = object -> getPosition () + sf::Vector2f (object -> getRadius (), object -> getRadius ());

			points[0].color = object -> getColor ();
			points[1].color = object -> getColor ();
			m_window.draw (points, 2, sf::Lines);
		}
	}

	std::string string = Format 
	(
		"Build: " BUILD "\n"
		"FPS: %.0lf/%s\n"
		"Display time: %.0lf msec\n"
		"Vertical sync: %s\n"
		"Antialiasing level: %d\n"
		"Speed modifier: %.2lf\n"
		"Objects count: %zu\n"
		"Trace: %s\n"
		"Show info: %s\n"
		"Game temp: %.2lf\n"
		"Score: %d", 

		getFps (), 
		Format (getFramerateLimit ()? "%d": "0 (no limit)", getFramerateLimit ()).c_str (), 
		getDisplayTime (), 
		getVerticalSyncEnabled ()? "Enabled": "Disabled",
		m_antialiasing,
		Object::GetSpeedModifier (),
		m_manager.data ().size (),
		GetTraceAllowed ()? "Enabled": "Disabled",
		m_objects_info?     "Enabled": "Disabled",
		m_temp,
		m_score
	);

	text.setStyle         (sf::Text::Regular);
	text.setString        (string);
	text.setFillColor     (sf::Color (0, 170, 255));
	text.setPosition      (10, 10);
	auto bounds = text.getGlobalBounds ();

	rect.setPosition  (sf::Vector2f (bounds.left  - indent,   bounds.top    - indent  ));
	rect.setSize      (sf::Vector2f (bounds.width + indent*2, bounds.height + indent*2));
	rect.setFillColor (sf::Color (255, 255, 255, 50));

	m_window.draw (rect);
	m_window.draw (text);
}

//------------------------

void Game::drawScore ()
{
	static char str[BUFFSIZE] = "";
	sprintf_s (str, "Score: %d\nLast score: %d\n", m_score, m_last_score);

	static sf::Text text;
	text.setString        (str);
	text.setFont          (*m_resources.getFont (FontConsolas));
	text.setCharacterSize (20);
	text.setPosition      (5, m_window.getSize ().y - 35 - text.getGlobalBounds ().height);
	text.setFillColor     (sf::Color (200, 200, 200));

	m_window.draw (text);
}

//------------------------

struct _datastruct
{
	int score;
};

//------------------------

void Game::loadData ()
{
	_datastruct data = {};

	FILE* file = nullptr;
	errno_t err = fopen_s (&file, s_data_file, "rb");
	if (!file || err)
	{
		char errbuff[BUFFSIZE] = "Unknown error";
		if (err) strerror_s (errbuff, err);

		Trace ("Failed to load data: %s\n", errbuff);
		return;
	}

	fread (&data, sizeof (data), 1, file);
	m_last_score = data.score;

	fclose (file);

	Trace ("Data loaded");
}

//------------------------

void Game::saveData ()
{
	_datastruct data = {};
	data.score = m_score;

	FILE* file = nullptr;
	errno_t err = fopen_s (&file, s_data_file, "wb");
	if (!file || err)
	{
		char errbuff[BUFFSIZE] = "Unknown error";
		if (err) strerror_s (errbuff, err);

		Trace ("Failed to save data: %s\n", errbuff);
		return;
	}

	fwrite (&data, sizeof (data), 1, file);
	fclose (file);

	Trace ("Data saved");
}

//------------------------

sf::Vector2f Game::getMousePosition ()
{
	static sf::Vector2i mouse;
	mouse = sf::Mouse::getPosition (m_window);

	return sf::Vector2f (mouse.x, mouse.y);
}

//------------------------

void Game::addObject (Object* object)
{
	object -> setGame (this);
	m_manager.addObject (object);
}

void Game::removeObject (Object* object)
{
	m_manager.deleteObject (object);
}

//------------------------

sf::RenderWindow* Game::getWindow ()
{
	return &m_window;
}

ObjectManager* Game::getManager ()
{
	return &m_manager;
}

ResourceManager* Game::getResourceManager ()
{
	return &m_resources;
}

//------------------------

double Game::getDisplayTime ()
{
	return m_last_display_time? m_last_display_time: 1;
}

double Game::getFps ()
{
	return 1000.0 / m_last_display_time;
}

//------------------------

void Game::setFramerateLimit (int limit)
{
	m_framerate_limit = limit;
	m_window.setFramerateLimit (limit);
}

int Game::getFramerateLimit ()
{
	return m_framerate_limit;
}

//------------------------

void Game::setVerticalSyncEnabled (bool enable)
{
	m_vertical_sync = enable;
	m_window.setVerticalSyncEnabled (m_vertical_sync);
}

bool Game::getVerticalSyncEnabled ()
{
	return m_vertical_sync;
}

//------------------------

void Game::setDebugModeEnabled (bool enable)
{
	m_debug_mode = enable;
}

bool Game::getDebugModeEnabled ()
{
	return m_debug_mode;
}

//------------------------

bool Game::processEvent (const sf::Event& event)
{
	#define PROCESS_EVENT_(type, proc) case type: if (proc (event)) { return true; } break;

	switch (event.type)
	{
		PROCESS_EVENT_ (sf::Event::Closed,     onEventClosed );
		PROCESS_EVENT_ (sf::Event::KeyPressed, onEventKeyDown);

		default: break;
	}

	return false;

	#undef PROCESS_EVENT_
}

//------------------------

bool Game::onEventClosed (const sf::Event& event)
{
	Trace ("Got close event, stopping game");
	stop ();

	return true;
}

//------------------------

bool Game::onEventKeyDown (const sf::Event& event)
{
	Trace ("Got key event");

	#define PROCESS_KEY_CODE_(code, proc) case code: if (proc (event)) { return true; } break;
	#define PROCESS_KEY_(key)             PROCESS_KEY_CODE_ (sf::Keyboard::##key, onKey##key);

	switch (event.key.code)
	{
		PROCESS_KEY_ (F1    );
		PROCESS_KEY_ (F2    );
		PROCESS_KEY_ (F3    );
		PROCESS_KEY_ (F4    );
		PROCESS_KEY_ (F5    );
		PROCESS_KEY_ (F6    );
		PROCESS_KEY_ (Escape);
		PROCESS_KEY_ (V     );
		PROCESS_KEY_ (T     );
		PROCESS_KEY_ (C     );
		PROCESS_KEY_ (Space );

		default: break;
	}

	return false;

	#undef PROCESS_KEY_CODE_
	#undef PROCESS_KEY_
}

//------------------------

bool Game::onKeyEscape (const sf::Event& event)
{
	Trace ("Escape pressed, stopping game");
	stop ();
	
	return true;
}

//------------------------

bool Game::onKeyF1 (const sf::Event& event)
{
	Trace (m_debug_mode? "Disabling debug mode": "Enabling debug mode");
	m_debug_mode = !m_debug_mode;

	return true;
}

//------------------------

bool Game::onKeyF2 (const sf::Event& event)
{
	Trace (m_objects_info? "Disabling objects info drawing": "Enabling objects info drawing");
	m_objects_info = !m_objects_info;

	return true;
}

//------------------------

bool Game::onKeyF3 (const sf::Event& event)
{
	Trace (m_objects_lines? "Disabling objects lines drawing": "Enabling objects lines drawing");
	m_objects_lines = !m_objects_lines;

	return true;
}

//------------------------

bool Game::onKeyF4 (const sf::Event& event)
{
	Trace (m_draw_background? "Disabling drawing background": "Enabling drawing background");
	m_draw_background = !m_draw_background;

	return true;
}

//------------------------

bool Game::onKeyF5 (const sf::Event& event)
{
	Trace (m_godmode? "Disabling god mode": "Enabling god mode");
	m_godmode = !m_godmode;

	return true;
}

//------------------------

bool Game::onKeyF6 (const sf::Event& event)
{
	Trace ("Clearing all object except player");

	for (auto& object: m_manager.data ())
		if (object -> type () != ObjectType::Player) removeObject (object);

	return true;
}

//------------------------

bool Game::onKeyV (const sf::Event& event)
{
	Trace (m_vertical_sync? "Disabling vertical sync": "Enabling vertical sync");
	m_vertical_sync = !m_vertical_sync;

	return true;
}

//------------------------

bool Game::onKeyT (const sf::Event& event)
{
	Trace (GetTraceAllowed ()? "Disabling trace": "Enabling trace");
	SetTraceAllowed (!GetTraceAllowed ());

	return true;
}

//------------------------

bool Game::onKeyC (const sf::Event& event)
{
	Trace ("Changing player color");

	Player* player = findObject <Player> (ObjectType::Player);
	if (!player)
	{
		DebugMessage ("Failed to find player");
		return true;
	}

	player -> setColor       (HSV (rand () % 256, 255, 255));
	player -> setBorderColor (HSV (rand () % 256, 255, 255));

	return true;
}

//------------------------

bool Game::onKeySpace (const sf::Event& event)
{
	Trace (m_paused? "Resuming": "Pausing");
	m_paused = !m_paused;

	return true;
}

//------------------------

template <typename object_t>
object_t* Game::findObject ()
{
	auto iter = std::find_if (m_manager.data ().begin (), m_manager.data ().end (), [](Object* object) { return dynamic_cast <object_t*> (object) != nullptr; });
	if (iter == m_manager.data().end ())
		return nullptr;

	return dynamic_cast <object_t*> (*iter);
}

template <typename object_t>
object_t* Game::findObject (ObjectType type)
{
	auto iter = std::find_if (m_manager.data ().begin (), m_manager.data ().end (), [type](Object* object) { return object -> type () == type; });
	if (iter == m_manager.data().end ())
		return nullptr;

	return dynamic_cast <object_t*> (*iter);	
}

//------------------------

sf::Rect <double> Game::getBounds ()
{
	return sf::Rect <double> (0, 0, m_window.getSize ().x, m_window.getSize ().y);
}

//------------------------

void Game::setSpeed (double speed)
{
	m_speed = speed;
}

double Game::getSpeed ()
{
	return m_speed;
}

//------------------------

void Game::setGodMode (bool enable)
{
	m_godmode = enable;
}

bool Game::getGodMode ()
{
	return m_godmode;
}

//------------------------

void Game::onGameOver ()
{
	if (m_godmode)
		return;

	printf ("Player died with score %d\n", m_score);

	static sf::Sound sound;
	sound.setBuffer (*m_resources.getSound (SoundDeath));
	sound.play ();
	sf::sleep (sound.getBuffer () -> getDuration ());

	stop ();
}

//------------------------

void Game::setScore (int score)
{
	m_score = score;
}

int Game::getScore ()
{
	return m_score;
}

//------------------------

void Game::setTemp (double temp)
{
	m_temp = temp;
}

double Game::getTemp ()
{
	return m_temp;
}

//------------------------