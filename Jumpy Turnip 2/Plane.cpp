#include "Plane.h"
#include "Projectile.h"
#include "Player.h"


Plane::Plane(Types::Obstacles thisType, Texture* texture, GameScene* thisScene) :
	Obstacle(thisType, texture, thisScene)
{
	radius = 45;
}


Plane::~Plane()
{
}

void Plane::setStats(float speed, float newHealth, float newDamage, bool fades, bool regens)
{
	Obstacle::setStats(speed, newHealth, newDamage, fades, regens);

	// Center of hitbox
	sprite.setOrigin(160.f, 60.f);

	// Adjusts stats of plane
	score = 30 * sqrt(speed*newHealth*newDamage);
	if (fades)
	{
		score *= 1.3;
	}
	if (regens)
	{
		score *= 1.3;
	}
	dropChance = 0.15;

	velocity.x *= 280;
	health *= 150;
	initHealth = health;
	damage *= 80;
	
}

 // Plane has a rectangular hit box

bool Plane::hasCollided(Projectile* projectile)
{
	return (fabs(projectile->getPosition().x - position.x) < length / 2 + projectile->getRadius()) &&
		(fabs(projectile->getPosition().y - position.y) < height / 2 + projectile->getRadius());
}

bool Plane::hasCollided(Player* player)
{
	return (fabs(player->getPosition().x - position.x) < length / 2.5 + player->getRadius()) &&
		(fabs(player->getPosition().y - position.y) < height / 2.5 + player->getRadius());
}