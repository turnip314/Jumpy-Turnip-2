#pragma once
#include "Obstacle.h"
class UFO : public Obstacle
{
public:
	UFO(Types::Obstacles thisType, Texture* thisTexture, GameScene* thisScene);
	~UFO();

	void doCollision(Player* player);

	void setStats(float speed, float newHealth, float newDamage, bool fades, bool regens);
};

