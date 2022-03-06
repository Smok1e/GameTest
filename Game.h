#pragma once

//------------------------

#include "ObjectManager.h"
#include "ResourceManager.h"

//------------------------

enum Resources
{
	FontConsolas,
	ImageBackground,
	SoundEat1,
	SoundEat2,
	SoundEat3,
	SoundEat4,
	SoundEat5,
	SoundEat6,
	SoundEat7,
	SoundDeath,
	SoundHit
};

//------------------------

class Player;

//------------------------

class Game
{
public:
	Game ();

	void init      ();
	void run       ();
	bool isRunning ();
	void stop      ();

	void drawBackground ();
	void drawDebugInfo  ();
	void drawScore      ();

	void addObject    (Object* object);
	void removeObject (Object* object);

	sf::RenderWindow* getWindow          ();	
	ObjectManager*    getManager         ();
	ResourceManager*  getResourceManager ();

	sf::Vector2f getMousePosition ();

	double getDisplayTime ();
	double getFps         ();

	void setFramerateLimit (int limit);
	int  getFramerateLimit ();

	void setVerticalSyncEnabled (bool enable);
	bool getVerticalSyncEnabled ();

	void setDebugModeEnabled (bool enable);
	bool getDebugModeEnabled ();

	void loadData ();
	void saveData ();

	bool processEvent   (const sf::Event& event);
	bool onEventClosed  (const sf::Event& event);
	bool onEventKeyDown (const sf::Event& event);
	bool onKeyEscape    (const sf::Event& event); // exit game
	bool onKeyF1        (const sf::Event& event); // debug mode
	bool onKeyF2        (const sf::Event& event); // objects info
	bool onKeyF3        (const sf::Event& event); // objects lines
	bool onKeyF4        (const sf::Event& event); // toggle background
	bool onKeyF5        (const sf::Event& event); // god mode
	bool onKeyF6        (const sf::Event& event); // clear objects
	bool onKeyV         (const sf::Event& event); // vertical sync
	bool onKeyT         (const sf::Event& event); // toggle trace
	bool onKeyC         (const sf::Event& event); // player color
	bool onKeySpace     (const sf::Event& event); // pause

	template <typename object_t>
	object_t* findObject ();

	template <typename object_t>
	object_t* findObject (ObjectType type);

	sf::Rect <double> getBounds ();
	
	void   setSpeed (double speed);
	double getSpeed ();

	void setScore (int score);
	int  getScore ();

	void   setTemp (double temp);
	double getTemp ();

	void setGodMode (bool enable);
	bool getGodMode ();

	void onGameOver ();

private:
	sf::RenderWindow m_window;

	ObjectManager    m_manager;
	ResourceManager  m_resources;

	sf::Color m_background;

	bool m_running;

	static const char* s_data_file;

	double m_last_display_time;
	double m_speed;
	bool   m_debug_mode;
	bool   m_objects_info;
	bool   m_objects_lines;
	int    m_framerate_limit;
	bool   m_vertical_sync;
	int    m_antialiasing;
	bool   m_paused;
	bool   m_draw_background;
	int    m_score;
	int    m_last_score;
	double m_temp;
	bool   m_godmode;

};

//------------------------