#define _USE_MATH_DEFINES

#include "Food.h"
#include "Player.h"
#include "Logging.h"
#include "Utils.hpp"
#include "Game.h"
#include <cmath>

//------------------------

int Food::s_spawn_range = 100;

//------------------------

Food::Food ():
	Food (sf::Vector2f (0, 0), 100)
{
	Trace ("Food constructed with default constructor");
}

Food::Food (sf::Vector2f pos, double counter_limit):
	Food (pos, sf::Color::White, counter_limit)
{
	Trace ("Food constructed with position constructor");
}

Food::Food (sf::Vector2f pos, sf::Color color, double counter_limit):
	Object          (pos, sf::Vector2f (0, 0), color, 15),
	m_counter       (0),
	m_counter_limit (counter_limit)
{
	Trace ("Food constructed with full constructor");
}

Food::Food (const Food& copy):
	Food (copy.m_pos, copy.m_color, copy.m_counter_limit)
{
	Trace ("Food constructed with copy constructor");
}

//------------------------

void Food::reset ()
{
	Player* player = m_game -> findObject <Player> (ObjectType::Player);
	GameAssert (player);

	sf::Rect <double> bounds = m_game -> getBounds ();

	sf::Vector2f player_pos = player -> getPosition ();
	sf::Vector2f new_pos;

	bool done = false;
	while (!done)
	{
		double angle = (rand () % 360) / (M_PI*2);

		new_pos.x = player_pos.x + cos (angle) * s_spawn_range;
		new_pos.y = player_pos.y + sin (angle) * s_spawn_range;

		if (new_pos.x >= bounds.left && new_pos.x + m_r*2 < bounds.left + bounds.width && new_pos.y >= bounds.top && new_pos.y + m_r*2 < bounds.top + bounds.height)
			done = true;
	}

	m_pos = new_pos;
	m_counter = 0;
}

//------------------------

void Food::setCounterLimit (double limit)
{
	m_counter_limit = limit;
}

double Food::getCounterLimit ()
{
	return m_counter_limit;
}

//------------------------

void Food::update ()
{
	if (m_counter >= m_counter_limit)
		m_game -> onGameOver ();

	else m_counter += 1.0 / s_speed_modifier;
}

//------------------------

void Food::draw (sf::RenderWindow* window)
{
	static bool initialized = false;
	static sf::Shader circle_shader;
	static sf::Shader rect_shader;

	if (!initialized)
	{
		circle_shader.loadFromFile ("Resources/Shaders/RadialGradient.glsl", sf::Shader::Fragment);
		rect_shader.loadFromFile   ("Resources/Shaders/CounterBar.glsl",     sf::Shader::Fragment);

		initialized = true;
	}

	sf::Color center = m_color;
	sf::Color border = center;
	border.a = 0;

	sf::CircleShape circle;
	circle.setFillColor (m_color);
	circle.setRadius    (m_r);
	circle.setPosition  (m_pos);

	circle_shader.setUniform ("position",     circle.getPosition  ()                      );
	circle_shader.setUniform ("radius",       circle.getRadius    ()                      );
	circle_shader.setUniform ("center_color", sf::Glsl::Vec4      (center)                );
	circle_shader.setUniform ("border_color", sf::Glsl::Vec4      (border)                );
	circle_shader.setUniform ("y_bound",      static_cast <float> (window -> getSize ().y));
	circle_shader.setUniform ("shift",        0.7f                                        );

	window -> draw (circle, &circle_shader);

	sf::Vector2f size (window -> getSize ().x / 3, 20);

	static sf::RectangleShape rect;
	rect.setSize      (size);
	rect.setPosition  (5, window -> getSize ().y - 5 - size.y);
	rect.setFillColor (sf::Color (0, 0, 0, 100));

	rect_shader.setUniform ("lft_color",        sf::Glsl::Vec4 (sf::Color (0, 33, 164, 255)));
	rect_shader.setUniform ("rgt_color",        sf::Glsl::Vec4 (sf::Color (81, 181, 255, 255)));
	rect_shader.setUniform ("background_color", sf::Glsl::Vec4 (sf::Color (0, 0,   0,   100)));
	rect_shader.setUniform ("position",         rect.getPosition ());
	rect_shader.setUniform ("size",             rect.getSize     ());
	rect_shader.setUniform ("t",                static_cast <float> (1.f - (m_counter / m_counter_limit)));

	window -> draw (rect, &rect_shader);
}

//------------------------

ObjectType Food::type ()
{
	return ObjectType::Food;
}

//------------------------

std::string Food::getDebugString ()
{
	return Format ("counter: %.0lf/%.0lf\nposition: {%.2lf, %.2lf}", m_counter, m_counter_limit, m_pos.x, m_pos.y);
}

//------------------------

void Food::SetSpawnRange (int range)
{
	s_spawn_range = range;
}

int Food::GetSpawnRange ()
{
	return s_spawn_range;
}

//------------------------