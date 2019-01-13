#include "Projectile.h"
#include "Obstacle.h"
#include "GameScene.h"

Projectile::Projectile(Texture* texture, GameScene* thisScene, Vector2f pos)
{
	hasTexture = true;
	sprite.setTexture(*texture);
	sprite.setOrigin(radius, radius);

	position = pos;
	scene = thisScene;

	// Testing stage
	damage = 50;
	pierce = 1;
}

Projectile::Projectile(Color newColor, GameScene* thisScene, Vector2f pos)
{
	hasTexture = false;

	position = pos;
	scene = thisScene;

	// Testing stage
	damage = 50;
	pierce = 1;

	color = newColor;
}

Projectile::~Projectile()
{
}

Projectile::Projectile(const Projectile &p2)
{
	color = p2.color;
	sprite = p2.sprite;
	hasTexture = p2.hasTexture;

	position = p2.position;
	velocity = p2.velocity;

	scene = p2.scene;

	radius = p2.radius;
	pierce = p2.pierce;
	damage = p2.damage;
	lifespan = p2.lifespan;

	if (p2.continuousSplit)
	{
		splitNum = p2.splitNum - 1;
	}
	continuousSplit = p2.continuousSplit;
	follow = p2.follow;

	resilience = p2.resilience;
	if (Math::random() < resilience)
	{
		stun = p2.stun;
	}
	if (Math::random() < resilience)
	{
		slow = p2.slow;
	}
	if (Math::random() < resilience)
	{
		freeze = p2.freeze;
	}
	if (Math::random() < resilience)
	{
		burn = p2.burn;
	}
	if (Math::random() < resilience)
	{
		poison = p2.poison;
	}
	if (Math::random() < resilience)
	{
		fadeRemove = p2.fadeRemove;
	}
	if (Math::random() < resilience)
	{
		nuclear = p2.nuclear;
	}

	knockback = p2.knockback;
	bounce = p2.bounce;

	alreadyHit = p2.alreadyHit;
}

void Projectile::update(Time dt)
{
	float time = dt.asSeconds();

	if (!follow)
	{
		velocity.y += 1200.f * time;
	}
	else
	{
		velocity.x += 10 * time * (lastMousePos.x - position.x);
		velocity.y += 10 * time * (lastMousePos.y - position.y);
	}


	lifespan -= time;
	if (lifespan <= 0 || position.x > 1300 || position.y > 1200 || position.x < -20)
	{
		addToRemoveList();
	}
	else if (position.x > 1350)
	{
		addToRemoveList();
	}

	position = Vector2f(position.x + velocity.x * time, position.y + velocity.y * time);
}

void Projectile::render(RenderWindow* handle, Vector2f scale)
{
	if (hasTexture)
	{
		sprite.setPosition(position);
		handle->draw(sprite);
	}
	else
	{
		CircleShape shape(radius);
		shape.setFillColor(color);
		shape.setPosition(position);
		handle->draw(shape);
	}
}

void Projectile::processInput(Keyboard::Key, bool isPressed)
{

}

void Projectile::processInput(Vector2i pos, bool isPressed)
{
	lastMousePos = Vector2f(pos.x, pos.y);
}

void Projectile::setPosition(Vector2f newPos)
{
	position = newPos;
}

void Projectile::setStats(Vector2f newVelocity, int newRadius, int newDamage, int newPierce, float newLifespan)
{
	velocity = newVelocity;
	speed = Math::magnitude(velocity);
	radius = newRadius;
	damage = newDamage;
	pierce = newPierce;
	lifespan = newLifespan;
}

void Projectile::setSplit(int newSplitNum)
{
	// Pierce gets converted to how many splits a projectile gets (otherwise pierce is harmful)
	splitNum = newSplitNum + pierce - 1;
	pierce = 1;
}

void Projectile::setContinuousSplit()
{
	continuousSplit = true;
}

void Projectile::setFollow()
{
	follow = true;
}

void Projectile::setStun(float time)
{
	stun = time;
}

void Projectile::setSlow(float time)
{
	slow = time;
}

void Projectile::setFreeze(float time)
{
	freeze = time;
}

void Projectile::setBurn(float time)
{
	burn = time;
}

void Projectile::setPoison(float time)
{
	poison = time;
}

void Projectile::setFadeRemove()
{
	fadeRemove = true;
}

void Projectile::setKnockback(float power)
{
	knockback = true;
	knockbackPower = power;
}

void Projectile::setBounce()
{
	bounce = true;
}

void Projectile::setNuclear()
{
	nuclear = true;
}

void Projectile::setResilience(float newResilience)
{
	resilience = newResilience;
}

void Projectile::setCloudImmunity()
{
	cloudImmunity = true;
}

void Projectile::setGoldReleaseChance(float chance)
{
	goldReleaseChance = chance;
}


Vector2f Projectile::getPosition()
{
	return position;
}

Vector2f Projectile::getVelocity()
{
	return velocity;
}

void Projectile::setVelocity(Vector2f vel)
{
	velocity = vel;
}

int Projectile::getRadius()
{
	return radius;
}

int Projectile::getDamage()
{
	return damage;
}

void Projectile::doDamage(Obstacle* obstacle)
{
	// Since each projectile should only affect each obstacle once
	if (find(alreadyHit.begin(), alreadyHit.end(), obstacle) != alreadyHit.end())
		return;

	alreadyHit.push_back(obstacle);

	if (pierce == -1)
	{
		return;
	}
	if (pierce > 0)
	{
		damage *= resilience;
		if (obstacle->getType() == Types::Obstacles::Cloud)
		{
			clearEffects(obstacle);
			obstacle->setHealth(obstacle->getHealth() - damage * 0.1);
			return;
		}
		else
		{
			obstacle->setHealth(obstacle->getHealth() - damage);
		}

		// Projectile damages obstacle and passes on any effects
		//obstacle->addImmune(this);
		pierce -= 1;
		if (stun)
		{
			obstacle->stun(stun, damage*0.4f);
		}
		if (slow)
		{
			obstacle->slow(slow);
		}
		if (freeze)
		{
			obstacle->freeze(freeze);
		}
		if (burn)
		{
			obstacle->burn(burn, damage*0.25f);
		}
		if (poison)
		{
			obstacle->poison(poison, damage*0.25f);
		}
		if (knockback)
		{
			obstacle->knockback(Vector2f(Math::unit(velocity).x*knockbackPower, Math::unit(velocity).y * knockbackPower));
		}
		if (splitNum)
		{
			// Do splitting action here
		}
		if (fadeRemove)
		{
			obstacle->removeFade();
		}
		if (obstacle->getType() != Types::Obstacles::Cloud)
		{
			if (bounce && velocity.y > 0)
			{
				velocity.y = -velocity.y;
			}
		}
	}
	
	if (pierce == 0)
	{
		// For projectiles that split
		for (int i = 0; i < splitNum; i++)
		{
			if (radius >= 1)
			{
				Vector2f vel = Math::rotate(velocity, Math::pi / 5 * (i - splitNum / 2.0) / splitNum);
				Projectile* shot = new Projectile(*this);
				if (bounce)
				{
					vel = Math::rotate(vel, -Math::pi / 5);
				}
				shot->setStats(vel, radius*sqrt(resilience), damage*resilience, 1, 5);
				scene->addPlayerProjectile(shot);
			}
		}
		pierce = -1;
		addToRemoveList();

		if (Math::random() < goldReleaseChance)
		{
			scene->addMoney(20, position);
		}
	}

}

void Projectile::doNuclearDamage(Obstacle* obstacle, Time dt)
{
	if (nuclear)
	{
		float dist = Math::distance(position, obstacle->getPosition());
		if (dist < 2)
			obstacle->setHealth(obstacle->getHealth() - 4 * damage * dt.asSeconds());
		else
			obstacle->setHealth(obstacle->getHealth() - 4 * damage * dt.asSeconds() / pow(dist/2, 0.85f));
	}
}

void Projectile::addToRemoveList()
{
	if (removed)
	{
		return;
	}
	removed = true;
	scene->addProjectileToRemove(this);
}

void Projectile::clearEffects(Obstacle* obstacle)
{
	if (!cloudImmunity)
	{
		if (Math::random() < 0.8)
		{
			follow = false;
			stun = false;
			slow = false;
			freeze = false;
			poison = false;
			nuclear = false;
			resilience *= 0.5;
			damage *= 0.8;
			radius *= 0.75;
		}

		fadeRemove = false;
		burn = false;
		velocity.x *= 0.9;
		velocity.y *= 0.9;
	}
}
