#include "SupplyCrate.h"
#include "Player.h"
#include "GameScene.h"
#include "Healthpack.h"

SupplyCrate::SupplyCrate(Types::Obstacles thisType, Texture* texture, GameScene* thisScene, Texture* hpTexture) :
	Obstacle(thisType, texture, thisScene)
{
	radius = 25;
	healthpackTexture = hpTexture;
}


SupplyCrate::~SupplyCrate()
{
}

void SupplyCrate::update(Time dt)
{
	// Falls down slowly (like parachute)
	float time = dt.asSeconds();
	velocity.y += 10 * time;
	if (position.y > 750)
	{
		alive = false;
		addToRemoveList();
	}
	Obstacle::update(dt);
}

void SupplyCrate::die()
{
	alive = false;

	// Supply crates have 60% chance of dropping health, 40% of dropping gold
	if (Math::random() < 0.6f)
	{
		scene->player->gainHealth(damage);
		// Paramedics get extra health from supply crates
		if (scene->player->isParamedic())
		{
			scene->player->gainHealth(damage * 0.25);
		}
		Healthpack* healthpack = new Healthpack(Types::Entities::Healthpack, healthpackTexture, scene);
		healthpack->setPosition(position);
		scene->addEntity(healthpack);
		
	}
	else
	{
		scene->addMoney(2*damage, position);
	}
}

void SupplyCrate::setStats(float speed, float newHealth, float newDamage, bool fades, bool regens)
{
	Obstacle::setStats(speed, newHealth, newDamage, fades, regens);
	sprite.setOrigin(25.f, 110.f);
	health *= 150;
	initHealth = health;
	damage *= 20;
	velocity.y = 40;
	velocity.x = 0;
}