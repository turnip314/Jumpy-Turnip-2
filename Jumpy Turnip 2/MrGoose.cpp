#include "MrGoose.h"
#include "Fireball.h"
#include "GameScene.h"

MrGoose::MrGoose(Types::Obstacles thisType, Texture* textureClosed, Texture* textureOpen, Texture* thisFireballTexture1, Texture* thisFireballTexture2, GameScene* thisScene) :
	Bird(thisType, textureClosed, thisScene)
{
	// Mr Goose shoots fireballs so it needs to store the fireball textures
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

	// To prevent the staying in center function from taking action
	if (!alive)
	{
		Obstacle::update(dt);
		return;
	}

	// Effects cool down much faster than other obstacles
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

	// Prevents Mr Goose from being knocked too far off course, will move up and down at
	// fixed height
	if (position.y < 250.f)
	{
		velocity.y = -velocity.x * 0.8;
	}
	else if (position.y >= 470)
	{
		velocity.y = velocity.x * 0.8;
	}

	// Will attack when attackPhase goes to 0
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

	// If goes too far
	if (position.x <= -40)
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

	// Add fireballs for attack
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
		fireball->setStats(2.f, 1.5f, 1.f, false, true);
		fireball->setPosition(Vector2f(position.x - 350, position.y - 40));
		fireball->setVelocity(Vector2f(fireball->getVelocity().x, 25 * (i - 1)));
		scene->pushObstacle(fireball);
	}

	cooldown = 2.f;
}

void MrGoose::die()
{
	Obstacle::die();
	scene->addMoney(100, position);
	scene->setBossKilled();
}

void MrGoose::setStats(float speed, float newHealth, float newDamage, bool fades, bool regens)
{
	// Adjusts stats
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

// Mr Goose has a rectangular hit box

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
	// Mr Goose only effected by knockback 25% as much
	Obstacle::knockback(Math::scale(direction, Vector2f(0.25f, 0.25f)));
}