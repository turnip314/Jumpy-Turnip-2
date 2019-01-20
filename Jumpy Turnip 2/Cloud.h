#pragma once
#include "Obstacle.h"

class Cloud : public Obstacle
{
public:
	Cloud(Types::Obstacles thisType, Texture* thisTexture, GameScene* thisScene);
	~Cloud();

	// Inhereted and overidden from base class, see implementation
	void update(Time dt);
	void die();
	void doCollision(Player* player);

	void setStats(float speed, float newHealth, float newDamage, bool fades, bool regens);

private:
	// Rectangular hit box dimensions
	const float height = 160.f;
	const float length = 300.f;
};

