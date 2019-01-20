#include "Fireball.h"
#include "GameScene.h"


Fireball::Fireball(Types::Obstacles thisType, Texture* texture, Texture* thisSplitTexture, GameScene* thisScene) :
	Obstacle(thisType, texture, thisScene)
{
	// A newly spawned fireball can split into smaller fireballs
	split = true;
	splitTexture = thisSplitTexture;

	// Default sizes for fireball and fireball boss
	if (thisType == Types::Obstacles::FireballBoss)
	{
		setRadius(150);
	}
	else
	{
		setRadius(35);
	}
	
}


Fireball::~Fireball()
{
}

void Fireball::update(Time dt)
{
	float time = dt.asMilliseconds();

	// Fireball boss corrects itself to stay in the center
	if (type == Types::Obstacles::FireballBoss)
	{
		if (position.y < 250.f)
		{
			velocity.y = (250.f - position.y) / 2.f;
		}
		else if (position.y >= 470)
		{
			velocity.y = (470.f - position.y) / 2.f;
		}
	}

	Obstacle::update(dt);
}

void Fireball::render(RenderWindow* handle, Vector2f scale)
{
	Obstacle::render(handle, scale);
}


void Fireball::setRadius(int newRadius)
{
	// Unlike other obstacles, fireballs change size upon splitting
	// Therefore, its sprite and origin need to be rescaled
	// For smooth image scaling, fireballs with radius greater than 37.5 use
	// Boss sprite, all others use normal sprite
	radius = newRadius;
	sprite.setOrigin(radius, radius);
	if (radius > 37.5)
	{
		sprite.setScale(Vector2f(radius / 150.f, radius / 150.f));
	}
	else
	{
		sprite.setScale(Vector2f(radius / 35.f, radius / 35.f));
	}
}

void Fireball::doCollision(Player* player)
{
	Obstacle::doCollision(player);
}

void Fireball::die()
{
	Obstacle::die();
	
	// Fireballs with radius at least 35 split into smaller fireballs
	if (radius >= 35)
	{
		addToRemoveList(); // Splitter fireballs don't fall when killed
		for (int i = 0; i < 3; i++)
		{
			Fireball* fireball;
			// To make sure the right texture is being used based on size (see setRadius)
			if (radius > 50) // new fireball radius > 37.5
			{
				fireball = new Fireball(Types::Obstacles::Fireball, texture, splitTexture, scene);
			}
			else
			{
				fireball = new Fireball(Types::Obstacles::Fireball, splitTexture, splitTexture, scene);
			}
			// New fireballs have about 1/3 the stats of the parent fireball, but move faster, and 75% radius
			fireball->setPosition(Vector2f(position.x, position.y + min(2.1*radius, 100.0)*(i - 1)));
			fireball->setStats((-1.2f*velocity.x + 20 * (i % 2)) / 70.f, initHealth*0.34f / 100.f,
				damage*0.32f / 70.f, fade, regen);
			fireball->setRadius(radius*0.75);
			fireball->setDropChance(dropChance * 0.3);
			scene->addObstacle(fireball);
		}
	}
}

void Fireball::burn(float time, float damage)
{
	// do nothing, immune
}

void Fireball::setStats(float speed, float newHealth, float newDamage, bool fades, bool regens)
{
	Obstacle::setStats(speed, newHealth, newDamage, fades, regens);

	// adjusts fireball stats
	score = 20 * sqrt(speed*newHealth*newDamage);
	if (fades)
	{
		score *= 1.2;
	}
	if (regens)
	{
		score *= 1.2;
	}

	if (type == Types::Obstacles::Fireball)
	{
		health *= 100;
		initHealth = health;
		damage *= 70;
		velocity.x *= 70;
		dropChance = 0.07f;
	}
	else if (type == Types::Obstacles::FireballBoss)
	{
		health *= 1000;
		initHealth = health;
		damage *= 1000;
		velocity.x *= 40;
		score *= 50;
		dropChance = 1.f;
	}
}