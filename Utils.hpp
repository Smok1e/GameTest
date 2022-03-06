#pragma once
#pragma warning (disable: 4244)
#pragma warning (disable: 4305)

#include <SFML/Graphics.hpp>
#include <string>
#include <cstdio>
#include <cstdarg>

//------------------------

#define BUFFSIZE 2048

#ifdef _DEBUG
	#define BUILD_TYPE "Debug"
#else
	#define BUILD_TYPE "Release"
#endif

#ifdef _WIN64
	#define BUILD_BITNESS "x64"
#else
	#define BUILD_BITNESS "x32"
#endif

#define BUILD BUILD_TYPE " (" BUILD_BITNESS ")"

//------------------------

template <typename TypeValue, typename TypeMin, typename TypeMax>
TypeValue Clamp (TypeValue value, const TypeMin& min, const TypeMax& max);

std::string  Format      (const char* format, ...);
std::string  Format      (const char* format, va_list args);
bool         Chance      (size_t probability);
sf::Color    Interpolate (sf::Color lft, sf::Color rgt, double t);
sf::Color    HSV         (int h, int s, int v, int a = 255);
int          Random      (int min, int max);
sf::Vector2f GetHitPoint (sf::Vector2f pos1, double r1, sf::Vector2f pos2);

template <typename T>
double VectorDistance (const sf::Vector2 <T>& a, const sf::Vector2 <T>& b);

template <typename T>
double VectorLength (const sf::Vector2 <T>& vector);

template <typename T>
sf::Vector2 <T> NormalizeVector (const sf::Vector2 <T>& vector);

//------------------------

template <typename TypeValue, typename TypeMin, typename TypeMax>
TypeValue Clamp (TypeValue value, const TypeMin& min, const TypeMax& max)
{
	if (value < min) value = min;
	if (value > max) value = max;

	return value;
}

//------------------------

template <typename T>
T VectorDistance (const sf::Vector2 <T>& a, const sf::Vector2 <T>& b)
{
	double distance_x = static_cast <double> (b.x - a.x);
	double distance_y = static_cast <double> (b.y - a.y);

	return sqrt (distance_x*distance_x + distance_y*distance_y); 
}

//------------------------

template <typename T>
double VectorLength (const sf::Vector2 <T>& vector)
{
	return sqrt (vector.x*vector.x + vector.y*vector.y);
}

//------------------------

template <typename T>
sf::Vector2 <T> NormalizeVector (const sf::Vector2 <T>& vector)
{
	double len = VectorLength (vector);
	return sf::Vector2 <T> (vector.x / len, vector.y / len);
}

//------------------------