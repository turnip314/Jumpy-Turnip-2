#include "Cloud.h"
#include "GameScene.h"



Cloud::Cloud(Types::Obstacles thisType, Texture* thisTexture, GameScene* thisScene):
	Obstacle(thisType, thisTexture, thisScene)
{
}

Cloud::~Cloud()
{
}

void Cloud::update(Time dt)
{
	float time = dt.asSeconds();

	//clouds take one damage per second on their own
	health -= time;

	Obstacle::update(dt);
}

void Cloud::die()
{
	addToRemoveList();
}

void Cloud::setStats(float speed, float newHealth, float newDamage, bool fades, bool regens)
{
	Obstacle::setStats(speed, newHealth, newDamage, fades, regens);

	// adjusts stats for cloud
	sprite.setOrigin(150.f, 80.f);

	velocity.x *= 50;
	health *= 120;
	initHealth = health;
	damage *= 0;

	radius = 80;
}

void Cloud::doCollision(Player* player)
{
	//do nothing
}
