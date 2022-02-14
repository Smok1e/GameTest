#define _CRT_SECURE_NO_WARNINGS
#include "Utils.hpp"

//------------------------

std::string Format (const char* format, ...)
{
	va_list args = {};
	va_start (args, format);
	size_t buffsize = vsnprintf (nullptr, 0, format, args) + 1;
	va_end (args);

	char* str = new char[buffsize];

	va_start (args, format);
	vsprintf_s (str, buffsize, format, args);
	va_end (args);

	std::string string = str;
	delete[] (str);

	return string;
}

//------------------------

std::string Format (const char* format, va_list args)
{
	size_t buffsize = vsnprintf (nullptr, 0, format, args) + 1;
	
	char* str = new char[buffsize];
	vsprintf_s (str, buffsize, format, args);

	std::string string = str;
	delete[] (str);

	return string;
}

//------------------------

bool Chance (size_t probability)
{
	return probability > static_cast <size_t> (rand () % 100);
}

//------------------------

sf::Color Interpolate (sf::Color lft, sf::Color rgt, double t)
{
	return sf::Color (
		lft.r + static_cast <double> (rgt.r - lft.r) * t, 
		lft.g + static_cast <double> (rgt.g - lft.g) * t,
		lft.b + static_cast <double> (rgt.b - lft.b) * t,
		lft.a + static_cast <double> (rgt.a - lft.a) * t
		);
}

//------------------------

sf::Color HSV (int h, int s, int v, int a /*= 255*/)
{
	h = Clamp (h, 0, 255);
	s = Clamp (s, 0, 255);
	v = Clamp (v, 0, 255);
	a = Clamp (a, 0, 255);

	sf::Color rgb;
	if (s == 0)
    {
        rgb.r = static_cast <unsigned char> (v);
        rgb.g = static_cast <unsigned char> (v);
        rgb.b = static_cast <unsigned char> (v);

        return rgb;
    }

    int region = h / 43;
    int remainder = (h - (region * 43)) * 6;

    int p = (v * (255 - s)) >> 8;
    int q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    int t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region)
    {
        case 0:
            rgb.r = static_cast <unsigned char> (v);
			rgb.g = static_cast <unsigned char> (t);
			rgb.b = static_cast <unsigned char> (p);
            break;

        case 1:
            rgb.r = static_cast <unsigned char> (q);
			rgb.g = static_cast <unsigned char> (v);
			rgb.b = static_cast <unsigned char> (p);
            break;

        case 2:
			rgb.r = static_cast <unsigned char> (p);
			rgb.g = static_cast <unsigned char> (v);
			rgb.b = static_cast <unsigned char> (t);
            break;

        case 3:
            rgb.r = static_cast <unsigned char> (p);
			rgb.g = static_cast <unsigned char> (q);
			rgb.b = static_cast <unsigned char> (v);
            break;

        case 4:
            rgb.r = static_cast <unsigned char> (t);
			rgb.g = static_cast <unsigned char> (p);
			rgb.b = static_cast <unsigned char> (v);
            break;

        default:
            rgb.r = static_cast <unsigned char> (v);
			rgb.g = static_cast <unsigned char> (p);
			rgb.b = static_cast <unsigned char> (q);
            break;

    }

	rgb.a = a;
    return rgb;
}

//------------------------

int Random (int min, int max)
{
	return min + rand () % (max-min);
}

//------------------------