#include "Player.h"
#include "Logging.h"
#include "Utils.hpp"	  
#include "Game.h"
#include "PlayerParticle.h"

//------------------------

sf::Vector2f Player::s_max_velocity = sf::Vector2f (6, 6);

//------------------------

Player::Player ():
	Player (sf::Vector2f (0, 0))
{
	Trace ("Player constructed with default constructor");
}

Player::Player (sf::Vector2f pos):
	Object         (pos, sf::Vector2f (0, 0), sf::Color::White, 20),
	m_border_color (sf::Color::Blue)
{
	Trace ("Player constructed with full constructor");
}

Player::Player (const Player& copy):
	Player (copy.m_pos)
{
	Trace ("Player constructed with copy constructor");
}

//------------------------

void Player::draw (sf::RenderWindow* window)
{
	static bool initialized = false;
	static sf::Shader shader;
	if (!initialized)
		shader.loadFromFile ("Resources/Shaders/RadialGradient.glsl", sf::Shader::Fragment);
	initialized = true;

	sf::Color center = m_color;
	sf::Color border = m_border_color;
	border.a = 0;

	sf::CircleShape circle;
	circle.setFillColor (m_color);
	circle.setRadius    (m_r);
	circle.setPosition  (m_pos);

	shader.setUniform ("position",     circle.getPosition  ()                      );
	shader.setUniform ("radius",       circle.getRadius    ()                      );
	shader.setUniform ("center_color", sf::Glsl::Vec4      (center)                );
	shader.setUniform ("border_color", sf::Glsl::Vec4      (border)                );
	shader.setUniform ("y_bound",      static_cast <float> (window -> getSize ().y));
	shader.setUniform ("shift",        0.9f                                        );

	window -> draw (circle, &shader);
}

//------------------------

void Player::update ()
{
	if (sf::Mouse::isButtonPressed (sf::Mouse::Left))
	{
		sf::Vector2f mouse = m_game -> getMousePosition ();

		double a = mouse.x - (m_pos.x + m_r);
		double b = mouse.y - (m_pos.y + m_r);

		double c = sqrt (a*a + b*b);

		m_vel.x += (a/c) / 5;
		m_vel.y += (b/c) / 5;
	}

	m_vel.x *= 0.98;
	m_vel.y *= 0.98;

	if (fabs (m_vel.x) <= 0.01) m_vel.x = 0;
	if (fabs (m_vel.y) <= 0.01) m_vel.y = 0;

	m_vel.x = Clamp (m_vel.x, -s_max_velocity.x, s_max_velocity.x);
	m_vel.y = Clamp (m_vel.y, -s_max_velocity.y, s_max_velocity.y);	

	if (Chance (sqrt (m_vel.x*m_vel.x + m_vel.y*m_vel.y) * 20.0 * s_speed_modifier * 0))
	{
		double angle = static_cast <double> (rand () % 360) / (M_PI*2);
		sf::Vector2f position = m_pos + sf::Vector2f (m_r + cos (angle)*(m_r-3), m_r + sin (angle)*(m_r-3));
		sf::Vector2f velocity = sf::Vector2f (cos (angle) * m_r / 20, sin (angle) * m_r / 20);

		PlayerParticle* particle = new PlayerParticle (position, velocity, m_color);
		particle -> setRadius (Random (2, 4));

		m_game -> addObject (particle);
	}
}

//------------------------

void Player::setBorderColor (sf::Color color)
{
	m_border_color = color;
}

sf::Color Player::getBorderColor ()
{
	return m_border_color;
}

//------------------------

ObjectType Player::type ()
{
	return ObjectType::Player;
}

//------------------------

void Player::SetMaxVelocity (sf::Vector2f velocity)
{
	s_max_velocity = velocity;
}

sf::Vector2f Player::GetMaxVelocity ()
{
	return s_max_velocity;
}

//------------------------