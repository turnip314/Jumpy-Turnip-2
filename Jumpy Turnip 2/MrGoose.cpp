#include "MrGoose.h"
#include "Fireball.h"
#include "GameScene.h"

MrGoose::MrGoose(Types::Obstacles thisType, Texture* textureClosed, Texture* textureOpen, Texture* thisFireballTexture1, Texture* thisFireballTexture2, GameScene* thisScene) :
	Bird(thisType, textureClosed, thisScene)
{
	radius = 100;
	attackPhase = 10;
	fireballTexture1 = thisFireballTexture1;
	fireballTexture2 = thisFireballTexture2;
	attackTexture = textureOpen;
	sprite.setTextureRect(IntRect(0, 0, 638, 200));
}


MrGoose::~MrGoose()
{

}

void MrGoose::update(Time dt)
{
	float time = dt.asSeconds();

	if (!alive)
	{
		Obstacle::update(dt);
		return;
	}

	// cools down much faster than other obstacles
	if (frozen > 0)
	{
		frozen -= 5 * time;
	}
	if (slowed > 0)
	{
		slowed -= 5 * time;
	}
	if (stunned > 0)
	{
		stunned -= 5 * time;
	}
	if (poisoned > 0)
	{
		poisoned -= 5 * time;
	}

	if (position.y < 250.f)
	{
		velocity.y = -velocity.x * 0.8;
	}
	else if (position.y >= 470)
	{
		velocity.y = velocity.x * 0.8;
	}

	if (attackPhase > 0)
	{
		attackPhase -= time;
	}
	else
	{
		attackPhase = 4 + 6 * Math::random();
		attack();
	}

	// cooldown done, change back to main texture
	if (cooldown > 0)
	{
		cooldown -= time;
		if (cooldown <= 0)
		{
			sprite.setTexture(*texture);
			sprite.setOrigin(266.f, 96.f);
		}
	}

	Obstacle::update(dt);

	if (position.x <= 0)
	{
		// game over
		scene->endGame();
	}
}

void MrGoose::attack()
{
	// Update sprite to open beak
	sprite.setTexture(*attackTexture);
	sprite.setOrigin(348.f, 96.f);

	// Add fireballs
	for (int i = 0; i < 3; i++)
	{
		Texture* fireballTexture;
		if (Math::random() < 0.5f)
		{
			fireballTexture = fireballTexture1;
		}
		else
		{
			fireballTexture = fireballTexture2;
		}
		Fireball* fireball = new Fireball(Types::Obstacles::Fireball, fireballTexture, fireballTexture, scene);
		fireball->setStats(2.f, 1.f, 1.f, false, true);
		fireball->setPosition(Vector2f(position.x - 350, position.y - 40));
		fireball->setVelocity(Vector2f(fireball->getVelocity().x, 25 * (i - 1)));
		scene->pushObstacle(fireball);
	}

	cooldown = 2.f;
}

void MrGoose::die()
{
	Obstacle::die();
	scene->setBossKilled();
	scene->addMoney(100, position);
	scene->setBossKilled();
}

void MrGoose::setStats(float speed, float newHealth, float newDamage, bool fades, bool regens)
{
	Bird::setStats(speed, newHealth, newDamage, fades, regens);
	score *= 500.f;
	health *= 7500;
	initHealth = health;
	damage *= 1000;
	velocity.x *= 17;
	velocity.y = -velocity.x * 0.8;
	position.x = 1600;
	sprite.setOrigin(266.f, 96.f);
}

bool MrGoose::hasCollided(Projectile* projectile)
{
	return (fabs(projectile->getPosition().x - position.x) < length / 2 + projectile->getRadius()) &&
		(fabs(projectile->getPosition().y - position.y) < height / 2 + projectile->getRadius());
}

bool MrGoose::hasCollided(Player* player)
{
	return (fabs(player->getPosition().x - position.x) < length / 2 + player->getRadius()) &&
		(fabs(player->getPosition().y - position.y) < height / 2 + player->getRadius());
}

void MrGoose::knockback(Vector2f direction)
{
	Obstacle::knockback(Math::scale(direction, Vector2f(0.25f, 0.25f)));
}