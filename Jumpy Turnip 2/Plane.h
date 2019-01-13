#pragma once
#include "Obstacle.h"

class Plane : public Obstacle
{
public:
	~Plane();

	Plane(Types::Obstacles thisType, Texture* texture, GameScene* thisScene);

	void setStats(float speed, float newHealth, float newDamage, bool fades, bool regens);

	bool hasCollided(Projectile* projectile);
	bool hasCollided(Player* projectile);

private:
	const float height = 60.f;
	const float length = 320.f;
};

