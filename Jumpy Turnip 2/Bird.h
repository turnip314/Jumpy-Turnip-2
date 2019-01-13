#pragma once
#include "Obstacle.h"

class Bird : public Obstacle
{
public:

	Bird(Types::Obstacles thisType, Texture* texture, GameScene* thisScene);
	virtual void update(Time dt);
	void render(RenderWindow* handle, Vector2f scale);
	void processInput(Keyboard::Key, bool isPressed);
	void processInput(Vector2i pos, bool isPressed);

	virtual void setPosition(Vector2f newPos);

	void poison(float time, float damage);

	virtual void setStats(float speed, float newHealth, float newDamage, bool fades, bool regens);

	~Bird();

private:
	float yPos;
};

