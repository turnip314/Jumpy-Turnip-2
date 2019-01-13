#include "Math.h"


float Math::distance(Vector2f pos1, Vector2f pos2)
{
	return sqrt(pow((pos1.x - pos2.x), 2) + pow((pos1.y - pos2.y), 2));
}

float Math::magnitude(Vector2f vector)
{
	return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

Vector2f Math::unit(Vector2f vector)
{
	float mag = Math::magnitude(vector);
	return Vector2f(vector.x / mag, vector.y / mag);
}

Vector2f Math::direction(Vector2f origin, Vector2f pos)
{
	return Math::unit(Vector2f(pos.x - origin.x, pos.y - origin.y));
}

Vector2f Math::vector(int magnitude, Vector2f direction)
{
	return Vector2f(direction.x*magnitude, direction.y*magnitude);
}

float Math::random()
{
	return (rand() / (RAND_MAX + 1.0));
}

Vector2f Math::rotate(Vector2f vector, float angle)
{
	return Vector2f(vector.x * cos(angle) - vector.y * sin(angle),
		vector.x*sin(angle) + vector.y*cos(angle));
}

float Math::angle(Vector2f vector)
{
	return atan2(vector.y, vector.x);
}

float Math::pi = 4 * atan(1);

Vector2f Math::scale(Vector2i vec, Vector2f factor)
{
	return Vector2f((vec.x*factor.x), (vec.y*factor.y));
}

Vector2f Math::scale(Vector2f vec, Vector2f factor)
{
	return Vector2f((vec.x*factor.x), (vec.y*factor.y));
}

Vector2f Math::scaleBack(Vector2i vec, Vector2f factor)
{
	return Vector2f((vec.x/factor.x), vec.y/factor.y);
}

Vector2f Math::scaleBack(Vector2f vec, Vector2f factor)
{
	return Vector2f((vec.x / factor.x), vec.y / factor.y);
}