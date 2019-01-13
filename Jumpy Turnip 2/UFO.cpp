#include "UFO.h"
#include "Player.h"


UFO::UFO(Types::Obstacles thisType, Texture* thisTexture, GameScene* thisScene):
	Obstacle(thisType, thisTexture, thisScene)
{
	radius = 40;
}

UFO::~UFO()
{
}

void UFO::doCollision(Player* player)
{
	player->setParalysis();
}

void UFO::setStats(float speed, float newHealth, float newDamage, bool fades, bool regens)
{
	Obstacle::setStats(speed, newHealth, newDamage, fades, regens);

	sprite.setOrigin(60.f, 45.f);

	score = 25 * sqrt(speed*newHealth*newDamage);
	if (fades)
	{
		score *= 1.3;
	}
	if (regens)
	{
		score *= 1.3;
	}
	dropChance = 0.1f;

	velocity.x *= 50;
	health *= 500;
	initHealth = health;
	damage *= 0;
}