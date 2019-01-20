#pragma once
#include "Obstacle.h"

class Bird : public Obstacle
{
public:

	Bird(Types::Obstacles thisType, Texture* texture, GameScene* thisScene);

	// Inhereted and overidden from base class, see implementation
	virtual void update(Time dt);
	void render(RenderWindow* handle, Vector2f scale);
	void processInput(Keyboard::Key, bool isPressed);
	void processInput(Vector2i pos, bool isPressed);

	// Gives the bird a new position and resets equilibrium y position (it moves as a sine wave)
	virtual void setPosition(Vector2f newPos);

	// Overriden from obstacle since only birds can get poisoned
	void poison(float time, float damage);

	virtual void setStats(float speed, float newHealth, float newDamage, bool fades, bool regens);

	~Bird();

private:
	float yPos;
};

