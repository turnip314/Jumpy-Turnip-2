#pragma once
#include "Obstacle.h"
class Fireball : public Obstacle
{
public:
	Fireball(Types::Obstacles thisType, Texture* texture, Texture* splitTexture, GameScene* thisScene);

	// Inhereted and overidden from base class, see implementation
	void update(Time dt);
	void render(RenderWindow* handle, Vector2f scale);
	~Fireball();

	void doCollision(Player* player);
	void die();

	// Radius changes when it splits
	void setRadius(int newRadius);

	// Immune to burn
	void burn(float time, float damage);

	void setStats(float speed, float newHealth, float newDamage, bool fades, bool regens);

private:
	bool split;
	Texture* splitTexture;
};

