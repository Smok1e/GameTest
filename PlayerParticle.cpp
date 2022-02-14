#include "PlayerParticle.h"
#include "Game.h"
#include "Utils.hpp"

//------------------------

double PlayerParticle::s_counter_limit = 200;

const double RANDOM_ACURACY = 1000;

//------------------------

PlayerParticle::PlayerParticle ():
	PlayerParticle (sf::Vector2f (0, 0), sf::Vector2f (0, 0))
{}

PlayerParticle::PlayerParticle (sf::Vector2f pos, sf::Vector2f vel):
	PlayerParticle (pos, vel, sf::Color::White)
{}

PlayerParticle::PlayerParticle (sf::Vector2f pos, sf::Vector2f vel, sf::Color color):	
	Object (pos, vel, color, 3),
	m_counter (0)
{}

PlayerParticle::PlayerParticle (const PlayerParticle& copy):
	PlayerParticle (copy.m_pos, copy.m_vel, copy.m_color)
{}

//------------------------

void PlayerParticle::update ()
{
	m_counter += 0.2;

	if (m_counter >= s_counter_limit)
		m_game -> getManager () -> deleteObject (this);

	m_vel += sf::Vector2f (
		(((rand () % static_cast <int> (RANDOM_ACURACY*2)) - RANDOM_ACURACY) / RANDOM_ACURACY) / 10,
		(((rand () % static_cast <int> (RANDOM_ACURACY*2)) - RANDOM_ACURACY) / RANDOM_ACURACY) / 10
	);
}

//------------------------

void PlayerParticle::draw (sf::RenderWindow* window)
{
	static bool initialized = false;
	static sf::Shader shader;
	if (!initialized)
		shader.loadFromFile ("Resources/Shaders/RadialGradient.glsl", sf::Shader::Fragment);
	initialized = true;

	sf::Color center = Interpolate (m_color, sf::Color (m_color.r, m_color.g, m_color.b, 0), m_counter / s_counter_limit);
	sf::Color border = sf::Color (center.r, center.g, center.b, 0);

	sf::CircleShape circle;
	circle.setFillColor (center);
	circle.setRadius    (m_r  );
	circle.setPosition  (m_pos);

	shader.setUniform ("position",     circle.getPosition  ()                      );
	shader.setUniform ("radius",       circle.getRadius    ()                      );
	shader.setUniform ("center_color", sf::Glsl::Vec4      (center)                );
	shader.setUniform ("border_color", sf::Glsl::Vec4      (border)                );
	shader.setUniform ("y_bound",      static_cast <float> (window -> getSize ().y)); 

	window -> draw (circle, &shader);
}

//------------------------

std::string PlayerParticle::getDebugString ()
{
	return "di nah";
}

//------------------------

void PlayerParticle::setCounter (double value)
{
	m_counter = value;
}

double PlayerParticle::getCounter ()
{
	return m_counter;
}

//------------------------

ObjectType PlayerParticle::type ()
{
	return ObjectType::PlayerParticle;
}

//------------------------

void PlayerParticle::SetCounterLimit (int limit)
{
	s_counter_limit = limit;
}

int PlayerParticle::GetCounterLimit ()
{
	return s_counter_limit;
}

//------------------------