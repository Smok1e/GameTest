#include <SFML/Audio.hpp>
#include <algorithm>
#include "Utils.hpp"
#include "Collision.h"
#include "Logging.h"
#include "Game.h"

//------------------------

hit_func_t* Hit_None = reinterpret_cast <hit_func_t*> (0xDEADC0DE);

//------------------------

hit_func_t* HitTable[ObjectsTypesAmount][ObjectsTypesAmount] =
{
	// 0          1                          2                                  3
	// Object     Player                     PlayerParticle                     Food
	{  Hit_Error, Hit_Error,                 Hit_Error,                         Hit_Error       }, // 0 Object
	{  Hit_Error, Hit_None,                  Hit_Player_PlayerParticle,         Hit_Player_Food }, // 1 Player
	{  Hit_Error, Hit_Player_PlayerParticle, Hit_None,                          Hit_None        }, // 2 PlayerParticle
	{  Hit_Error, Hit_Player_Food,           Hit_None,                          Hit_None        }  // 3 Food
};

//------------------------

void Hit (Object* a, Object* b)
{
	int type_a = static_cast <int> (a -> type ());
	int type_b = static_cast <int> (b -> type ());
	if (type_a > type_b) std::swap (a, b);

	hit_func_t* hit_proc = HitTable[type_a][type_b];
	if (hit_proc != Hit_None)
		hit_proc (a, b);
}

//------------------------

void Hit_Error (Object* a, Object* b)
{
	DebugMessage ("Collision error: Trying to collide object '%s' with object '%s'", StrObjectType (a -> type ()), StrObjectType (b -> type ()));
}

//------------------------

void Hit_Player_Food (Object* a, Object* b)
{
	Food* food = dynamic_cast <Food*> (b);
	GameAssert (food);

	Game* game = food -> getGame ();

	double score = game -> getScore ();
	if (!score) score = 1;

	food -> reset ();
	food -> setCounterLimit (food -> getCounterLimit () * (0.98));
	game -> setSpeed (game -> getSpeed () * (1.01));
	game -> setScore (game -> getScore () + 1);
}

//------------------------

void Hit_Player_PlayerParticle (Object* a, Object* b)
{
	Player*         player   = dynamic_cast <Player*>         (a);
	PlayerParticle* particle = dynamic_cast <PlayerParticle*> (b);

	GameAssert (player);
	GameAssert (particle);

	sf::Vector2f diff = particle -> getPosition () - player -> getPosition ();
	sf::Vector2f point = GetHitPoint (player -> getPosition (), player -> getRadius (), particle -> getPosition ());

	double c = (diff.x*diff.x + diff.y*diff.y);
	diff.x /= c;
	diff.y /= c;

	diff.x *= player -> getRadius ();
	diff.y *= player -> getRadius ();

    float im1 = 10; 
    float im2 = 10;

	sf::Vector2f v = player -> getVelocity () - particle -> getVelocity ();

	c = sqrt (v.x*v.x + v.y*v.y);
	v.x /= c;
	v.y /= c;

	float vn = sqrt (v.x*v.x + v.y*v.y);
    float i = (-(1.0f + 1) * vn) / (im1 + im2);
    sf::Vector2f impulse (diff.x*i, diff.y*i);
	particle -> setVelocity (particle -> getVelocity () - sf::Vector2f (impulse.x*im2/10, impulse.y*im2/10));
}

//------------------------