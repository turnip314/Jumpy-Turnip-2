#include "Flappy.h"
#include <iostream>


Flappy::Flappy(Types::Obstacles thisType, Texture* texture, GameScene* thisScene) :
	Bird(thisType, texture, thisScene)
{

}

Flappy::~Flappy()
{
}

void Flappy::update(Time dt)
{
	float time = dt.asMilliseconds();

	if (alive)
	{
		// r is random value 0.0 to 1.0
		// If bird is too low, will definitely jump
		// Otherwise, will jump when it reaches given random velocity, unless
		// it's already too high up
		float r = Math::random();
		if (position.y > 725)
		{
			jump();
			velToJump = (1000 - position.y)*r;
		}
		else if (velocity.y >= velToJump && position.y >= 40)
		{
			jump();
			velToJump = (1250 - position.y)*r;
		}

		velocity.y += 1.5*time;
	}

	Obstacle::update(dt);
}

void Flappy::render(RenderWindow* handle, Vector2f scale)
{
	float angle = atan2(velocity.y, velocity.x) + Math::pi;
	sprite.setRotation(angle * 180.f / Math::pi);
	Obstacle::render(handle, scale);
}

void Flappy::jump()
{
	velocity.y = -500;
}