#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

using namespace sf;
using namespace std;

class Math
{
public:

	// Distance given two positions
	static float distance(Vector2f pos1, Vector2f pos2);

	// Returns magnitude of a vector
	static float magnitude(Vector2f vector);

	// Converts any vector into a unit vector with same direction
	static Vector2f unit(Vector2f vector);

	// Gives a unit vector representing the direction of pos
	// Relative to origin
	static Vector2f direction(Vector2f origin, Vector2f pos);

	// Given magnitude and direction, returns vector
	static Vector2f vector(int magnitude, Vector2f direction);

	static float random();

	static Vector2f rotate(Vector2f vector, float angle);

	static float angle(Vector2f vector);

	static float pi;

	static Vector2f scale(Vector2i vec, Vector2f factor);
	static Vector2f scale(Vector2f vec, Vector2f factor);

	static Vector2f scaleBack(Vector2i vec, Vector2f factor);
	static Vector2f scaleBack(Vector2f vec, Vector2f factor);

};
