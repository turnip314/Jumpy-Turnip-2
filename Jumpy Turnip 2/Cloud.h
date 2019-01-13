#pragma once
#include "Obstacle.h"

class Cloud : public Obstacle
{
public:
	Cloud(Types::Obstacles thisType, Texture* thisTexture, GameScene* thisScene);
	~Cloud();

	void update(Time dt);
	void die();
	void doCollision(Player* player);

	void setStats(float speed, float newHealth, float newDamage, bool fades, bool regens);

private:
	const float height = 160.f;
	const float length = 300.f;
};

