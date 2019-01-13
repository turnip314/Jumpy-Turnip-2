#pragma once
#include "Obstacle.h"

class SupplyCrate : public Obstacle
{
public:
	SupplyCrate(Types::Obstacles thisType, Texture* texture, GameScene* thisScene, Texture* hpTexture);
	~SupplyCrate();

	void die();
	void update(Time dt);
	void setStats(float speed, float newHealth, float newDamage, bool fades, bool regens);

private:
	int bonus;
	Texture* healthpackTexture;
};
