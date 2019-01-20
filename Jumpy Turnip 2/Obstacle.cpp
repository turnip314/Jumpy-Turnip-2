#include "Obstacle.h"
#include "GameScene.h"
#include "Lightning.h"
#include "Game.h"
#include "Coin.h"

Obstacle::Obstacle(Types::Obstacles thisType, Texture* thisTexture, GameScene* thisScene)
{
	// Initializes sprite of obstacle
	texture = thisTexture;
	if (texture != nullptr)
	{
		sprite.setTexture(*thisTexture);
	}
	
	// defaults, may be replaced by specific obstacle's initialization
	position = Vector2f(1280, 360);
	radius = 15;

	type = thisType;
	alive = true;
	scene = thisScene;
	phase = 0;
}

Obstacle::~Obstacle()
{
}

void Obstacle::update(Time dt)
{
	float time = dt.asSeconds();

	if (alive)
	{
		// If damaged, will regenerate 10% of its health per second
		if (regen && health < initHealth)
		{
			health += initHealth * time / 10.f;
			// In case of lag, time might be big number
			if (health > initHealth)
			{
				health = initHealth;
			}
		}

		// EFFECTS AFFECTING MOVEMENT
		// If stunned, will not move and take damage
		if (stunned > 0)
		{
			health -= stunDamage * time;
			stunned -= time;
		}
		// If frozen, will not move
		if (frozen > 0)
		{
			frozen -= time;
		}
		// Moves 30% slower
		if (slowed > 0)
		{
			slowed -= time;
			if ((stunned <= 0) && (frozen <= 0))
			{
				position = Vector2f(position.x + velocity.x * time * 0.7, position.y + velocity.y * time * 0.7);
			}
		}
		// If not stunned, slowed, or frozen, moev at regular speed
		if ((stunned <= 0) && (frozen <= 0) && (slowed <= 0))
		{
			position = Vector2f(position.x + velocity.x * time, position.y + velocity.y * time);
		}

		// DAMAGE ONLY EFFECTS
		// If burned, take burn damage
		if (burned > 0)
		{
			burned -= time;
			health -= burnDamage * time;
		}
		// If poisoned, take poison damage
		if (poisoned > 0)
		{
			poisoned -= time;
			health -= poisonDamage * time;
		}

		// If not immobile, cannot be pushed offscreen
		if (alive && stunned <= 0 && frozen <= 0)
		{
			if (position.y < 10)
			{
				position.y += 50 * time;
			}
			else if (position.y > 710)
			{
				position.y -= 50 * time;
			}
		}

		// Handles offscreen and death related stuff
		if (health <= 0)
		{
			velocity.y = -250;
			die();
		}

		// Checks for collisions with player
		if (hasCollided(scene->player))
		{
			doCollision(scene->player);
		}
	}
	else
	{
		// If dead, falls
		velocity.y += 2000 * time;
		position.y += velocity.y * time;
		position.x += velocity.x * time;
	}

	// Checks projectiles to see if it has collided with any
	// and if it's not immune to that projectile
	// Works whether or not obstacle is alive
	for (unsigned i = 0; i < scene->playerProjectiles.size(); i++) {
		Projectile* projectile = scene->playerProjectiles.at(i);
		if (hasCollided(projectile))
		{
			projectile->doDamage(this);
		}
		projectile->doNuclearDamage(this, dt);
	}

	//HANDLING OFFSCREEN
	// If it has fallen offscreen, dies
	if (position.y > 1000)
	{
		if (alive)
		{
			die();
		}
		addToRemoveList();
	}
	// If it has moved past the player, reset it if it's a looping level, or delete it otherwise
	else if (position.x < -200)
	{
		if (scene->isLooping())
		{
			position.x = 2000;
		}
		else
		{
			addToRemoveList();
		}
	}

	// Shocked by lightning - only occurs for one frame for the purpose of making a chain
	// Dead objects can pass on lightning
	if (shocked)
	{
		shocked = false;
	}

	// MOVEMENT EFFECTS THAT DO NOT DEPEND ON OBSTACLE BEING ALIVE
	// Blown by wind move back respective of its size
	// Clouds are affected much more by this
	if (blownBack > 0)
	{
		if (position.x > scene->player->getPosition().x)
		{
			position.x -= 2.f * velocity.x * time * 30.f / radius;

			if (type == Types::Obstacles::Cloud)
			{
				position.x -= 3.f * velocity.x * time;
			}
		}
		blownBack -= time;
	}
	// Knockback, move back depending on size
	if (knockedBack > 0)
	{
		knockedBack -= time;
		position.x += 60 * knockbackDirection.x*time * 30.f / radius;
		position.y += 60 * knockbackDirection.y*time * 30.f / radius;
	}

}

void Obstacle::render(RenderWindow* handle, Vector2f scale)
{
	// Obstacle appears slightly blue if slowed or frozen
	if (frozen > 0)
	{
		sprite.setColor(Color(100, 100, 230));
	}
	else if (slowed > 0)
	{
		sprite.setColor(Color(150, 150, 255));
	}

	// Draws main obstacle image
	if (texture == nullptr)
	{
		CircleShape shape(radius);
		shape.setFillColor(sf::Color(100, 250, 50));
		shape.setPosition(Vector2f(position.x - radius, position.y - radius));
		handle->draw(shape);
	}
	else
	{
		sprite.setPosition(position);
		handle->draw(sprite);
	}

	// Healthbar
	RectangleShape healthBar(Vector2f(max(0.f, 2 * radius*health / initHealth), radius / 10));
	if (type != Types::Obstacles::SupplyCrate)
	{
		healthBar.setPosition(Vector2f(position.x - radius, position.y - 1.25*radius));
	}
	healthBar.setFillColor(Color(50, 150, 50));

	// Stunned makes health bar yellow, burned makes health bar red, poisoned makes
	// health bar green. Order of precedence is as shown
	if (stunned > 0)
	{
		healthBar.setFillColor(Color(255, 255, 100));
	}
	else if (burned > 0)
	{
		healthBar.setFillColor(Color(255, 100, 100));
	}
	else if (poisoned > 0)
	{
		healthBar.setFillColor(Color(0, 50, 0));
	}

	handle->draw(healthBar);
}

void Obstacle::processInput(Keyboard::Key, bool isPressed)
{

}

void Obstacle::processInput(Vector2i pos, bool isPressed)
{

}

void Obstacle::addToRemoveList()
{
	if (removed)
	{
		return;
	}
	scene->addObstacleToRemove(this);
	removed = true;
}

bool Obstacle::hasCollided(Projectile* projectile)
{
	return (Math::distance(position, projectile->getPosition()) < radius + projectile->getRadius());
}

bool Obstacle::inVincinity(Projectile* projectile)
{
	return (Math::distance(position, projectile->getPosition()) < radius + 6*projectile->getRadius());
}

bool Obstacle::hasCollided(Player* player)
{
	return (Math::distance(position, player->getPosition()) < radius + player->getRadius());
}

void Obstacle::doCollision(Player* player)
{
	// Player handles the taking damage from obstacle, but obstacle
	// loses the amount of health player has
	if (alive && player->isAlive())
	{
		player->takeDamage(this);
		if (!player->isInvincible())
			health -= max(0.f, player->getHealth());
		if (health <= 0)
		{
			die();
		}
	}
}

float Obstacle::getHealth()
{
	return health;
}

void Obstacle::setHealth(float newHealth)
{
	health = newHealth;
}

float Obstacle::getDamage()
{
	return damage;
}

Types::Obstacles Obstacle::getType()
{
	return type;
}

void Obstacle::addImmune(Projectile* projectile)
{
	immune.push_back(projectile);
}

void Obstacle::setPosition(Vector2f newPos)
{
	position = newPos;
}

void Obstacle::setVelocity(Vector2f newVel)
{
	velocity = newVel;
}

Vector2f Obstacle::getVelocity()
{
	return velocity;
}

void Obstacle::die()
{
	// To prevent this from being called twice
	if (!alive)
	{
		return;
	}

	alive = false;
	scene->addScore(score);

	// Chance to drop coins
	if (Math::random() < dropChance)
	{
		if (Math::random() < 0.01)
		{
			scene->addMoney(250, position);
		}
		else if (Math::random() < 0.1)
		{
			scene->addMoney(50, position);
		}
		else
		{
			scene->addMoney(10, position);
		}
	}
}

Vector2f Obstacle::getPosition()
{
	return position;
}

void Obstacle::setStats(float speed, float newHealth, float newDamage, bool fades, bool regens)
{
	// THESE ARE MULTIPLIERS
	velocity = Vector2f(-speed, 0);
	health = newHealth;
	damage = newDamage;
	fade = fades;
	regen = regens;

	// Increase transparency of sprites for faded obstacles
	if (fades)
	{
		sprite.setColor(Color(255, 255, 255, 25));
	}
}

void Obstacle::stun(float time, float damage)
{
	stunned = time;
	stunDamage = damage;
}

void Obstacle::slow(float time)
{
	slowed = time;
}

void Obstacle::freeze(float time)
{
	frozen = time;
}

void Obstacle::burn(float time, float damage)
{
	burned = time;
	burnDamage = damage;
}

void Obstacle::poison(float time, float damage)
{
	// Do nothing, only bird class needs to worry about this
}

void Obstacle::removeFade()
{
	fade = false;
	sprite.setColor(Color(255, 255, 255, 255));
}

void Obstacle::removeRegen()
{
	regen = false;
}

void Obstacle::knockback(Vector2f direction)
{
	knockedBack = 0.15f;
	knockbackDirection = direction;
}

void Obstacle::shock()
{
	// Prevents infinite recursion loops
	if (shocked)
	{
		return;
	}
	shocked = true;

	// Takes damage from lightning, bosses affected less
	if (type == Types::Obstacles::MrGoose || type == Types::Obstacles::FireballBoss)
	{
		health -= initHealth * 0.015;
	}
	else
	{
		health -= initHealth * 0.4;
	}

	// Checks every obstacle and shocks the ones that haven't been shocked
	for (int i = 0; i < scene->obstacles.size(); i++)
	{
		Obstacle* obstacle = scene->obstacles.at(i);
		if (Math::distance(position, obstacle->getPosition()) < max(radius*20, 400))
		{
			if (!obstacle->isShocked())
			{
				obstacle->shock();
				Entity* lightning = new Lightning(Types::Entities::Lightning, NULL, scene,
					position, obstacle->getPosition());
				scene->addEntity(lightning);
			}
		}
	}
}

void Obstacle::blowBack(float time)
{
	blownBack = time;
}

bool Obstacle::isShocked()
{
	return shocked;
}

void Obstacle::setDropChance(float newDropChance)
{
	dropChance = newDropChance;
}