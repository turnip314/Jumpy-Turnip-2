#pragma once
#include "Obstacle.h"
class Fireball : public Obstacle
{
public:
	// Main contrusctor for first fireball, use different constructor for
	// splitter fireballs
	Fireball(Types::Obstacles thisType, Texture* texture, Texture* splitTexture, GameScene* thisScene);
	void update(Time dt);
	void render(RenderWindow* handle, Vector2f scale);
	void processInput(Keyboard::Key, bool isPressed);
	void processInput(Vector2i pos, bool isPressed);
	~Fireball();

	void doCollision(Player* player);
	void die();

	void setRadius(int newRadius);

	void burn(float time, float damage);

	void setStats(float speed, float newHealth, float newDamage, bool fades, bool regens);

private:
	bool split;
	Texture* splitTexture;
};

