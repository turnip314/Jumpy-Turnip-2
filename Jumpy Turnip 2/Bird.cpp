#include "Bird.h"
#include "GameScene.h"

Bird::Bird(Types::Obstacles thisType, Texture* texture, GameScene* thisScene) :
	Obstacle(thisType, texture, thisScene)
{
	
}

Bird::~Bird()
{
}

void Bird::update(Time dt)
{
	float time = dt.asMilliseconds();
	
	// Because all birds move in a wave-like motion, of course
	if (alive)
	{
		phase = fmod(phase + 0.02 / radius * time, 2 * Math::pi);
		float yShift = 2 * radius*sin(phase);
		position.y = yPos + yShift;
	}

	Obstacle::update(dt);
}

void Bird::render(RenderWindow* handle, Vector2f scale)
{
	Obstacle::render(handle, scale);
}

void Bird::processInput(Keyboard::Key, bool isPressed)
{

}

void Bird::processInput(Vector2i pos, bool isPressed)
{

}

void Bird::poison(float time, float damage)
{
	poisoned = time;
	poisonDamage = damage;
}

void Bird::setStats(float speed, float newHealth, float newDamage, bool fades, bool regens)
{
	// Initializes the stats specific to each bird type based on the generic obstacle stats given
	// Mr Goose is the main exception to this, and partially handled its stats itself
	Obstacle::setStats(speed, newHealth, newDamage, fades, regens);
	score = 15 * sqrt(speed*newHealth*newDamage);
	if (fades)
	{
		score *= 1.2;
	}
	if (regens)
	{
		score *= 1.2;
	}
	dropChance = 0.03f;

	if (type == Types::Obstacles::Bird)
	{
		radius = 20;
		health *= 30;
		damage *= 30;
		initHealth = health;
		velocity.x *= 200;

		sprite.setOrigin(Vector2f(32.f, 20.f));
	}
	else if (type == Types::Obstacles::Eagle)
	{
		radius = 25;
		score *= 1.25f;
		health *= 50;
		initHealth = health;
		damage *= 60;
		velocity.x *= 120;

		sprite.setOrigin(Vector2f(38.f, 33.f));
	}
	else if (type == Types::Obstacles::Flappy)
	{
		score *= 1.5f;
		health *= 40;
		initHealth = health;
		damage *= 15;
		velocity.x *= 150;
		
		sprite.setOrigin(Vector2f(20.f, 15.f));
	}
}

void Bird::setPosition(Vector2f newPos)
{
	// Changes "equilibrium point" of the bird's wave-like motion
	yPos = newPos.y;
	Obstacle::setPosition(newPos);
}
