#include <SFML\Graphics.hpp>

#include "Player.h"
#include <cmath>
#include "Game.h"
#include "Projectile.h"
#include "GameScene.h"
#include "Entity.h"
#include "Lightning.h"

using namespace sf;

Player::Player(TextureManager* manager, GameScene* thisScene)
{
	// Texture should be in initialize as well
	scene = thisScene;

	jump = false;
	velocity = Vector2f(0, 0);
	position.x = 170.f;
	position.y = 100.f;

	textureManager = manager;

	// Initializes function maps based on Player type
	initializeFunctionMaps();

	parachuteSprite.setTexture(*textureManager->getTexture(Textures::Parachute));
	parachuteSprite.setOrigin(31.f, 128.f);

	autoShootText.setString("A");
	autoShootText.setCharacterSize(30);
	autoShootText.setFont(*textureManager->getFont(Fonts::Calibri));
	autoShootText.setPosition(1, 117);

	autojumpText.setString("J");
	autojumpText.setCharacterSize(30);
	autojumpText.setFont(*textureManager->getFont(Fonts::Calibri));
	autojumpText.setPosition(55, 117);

}

void Player::update(Time dt)
{
	float time = dt.asSeconds();

	if (!alive)
	{
		if (position.y >= 730 && !scene->isGameOver())
		{
			if (type == Types::Players::Biology && upgrades[9] == 2 && Math::random() < 0.15)
			{
				alive = true;
				health = initHealth * 0.5;
				position.y = 100;
			}
			else
			{
				scene->playerSwap();
			}
		}
		else
		{
			velocity.y += 1500 * time;
			position = Vector2f(position.x + velocity.x * time, position.y + velocity.y * time);
		}
		return;
	}

	// Managing health
	if (position.y <= 0)
	{
		
		loseHealth(20 * time * tooHighResistance);
	}
	else if (position.y >= 720)
	{
		loseHealth(20 * time * tooLowResistance);
	}

	if (invincibility > 0)
	{
		invincibility -= time;
	}

	if (regen)
	{
		gainHealth(0.005 * time * (healthCap - health));
	}

	// In charge of shooting related stuff
	if ((multitask || !autoShoot) && (reload > 0))
	{
		reload -= 1000*time;
	}
	if (autoShoot && (autoReload > 0))
	{
		autoReload -= 1000 * time;
	}
	else if (autoShoot)
	{
		shoot(mouseHoverLocation);
		autoReload = autoReloadTime;
	}

	// Ability
	// Note: Should not check ability available as normie turnip could theoretically activate
	// ability before unlocking it
	if (abilityActivated)
	{
		abilityTimeLeft -= time;
	}
	else if (abilityReload > 0)
	{
		abilityReload -= time;
	}

	if (abilityActivated && abilityTimeLeft <= 0)
	{
		cancelAbility();
	}
	else
	{
		abilityTimeLeft -= time;
	}

	// Movement
	if (!paralyzed || paralysisImmunity)
	{
		if (jump)
		{
			velocity.y = -750;
			jump = false;
		}
		else if (autoJump && position.y > 400)
		{
			velocity.y = -800;
			jump = false;
		}
		else
		{
			if (parachute && velocity.y > 0 && !jetPack)
			{
				velocity.y += 20 * (50 - velocity.y) * time;
			}
			if (jetPack)
			{
				velocity.y -= 1000 * time;
			}
			else
			{
				velocity.y += 1500 * time;
			}
		}

		position = Vector2f(position.x + velocity.x * time, position.y + velocity.y * time);
	}
	if (paralyzed)
	{
		paralyzeTime -= time;
		if (paralyzeTime <= 0)
		{
			paralyzed = false;
		}
	}
	
	if (health <= 0)
	{
		alive = false;
		velocity.y = -250;
	}
}

void Player::render(RenderWindow* handle, Vector2f scale)
{

	// Shield
	sf::CircleShape shape(radius * 2);
	shape.setPosition(position.x - radius, position.y-radius);
	shape.setOrigin(radius, radius);
	shape.setFillColor(Color(0, 0, 0, 0));
	shape.setOutlineColor(Color(128, 128, 128, 200 * shield));
	shape.setOutlineThickness(3);

	handle->draw(shape);

	if (invincibility > 0)
	{
		sprite.setColor(Color(255, 255, 255, 128));
	}
	else
	{
		sprite.setColor(Color(255, 255, 255, 255));
	}
	sprite.setPosition(position);
	handle->draw(sprite);

	if (parachute)
	{
		parachuteSprite.setPosition(Vector2f(position.x, position.y - radius));
		handle->draw(parachuteSprite);
	}

	// Health bar of player
	RectangleShape healthBar(Vector2f(300.f*max(0.f, getHealth() / 250), 30.f));
	healthBar.setFillColor(Color(50, 150, 50));
	handle->draw(healthBar);

	RectangleShape abilityBar(Vector2f(300.f, 30.f));
	abilityBar.setFillColor(Color(200, 200, 210));
	abilityBar.setPosition(Vector2f(0, 30));
	handle->draw(abilityBar);
	
	if (abilityAvailable)
	{
		RectangleShape abilityBar(Vector2f(300.f*max(0.f, 1.f - abilityReload / abilityReloadTime), 30.f));
		abilityBar.setFillColor(Color(50, 50, 255));
		abilityBar.setPosition(Vector2f(0, 30));
		handle->draw(abilityBar);
	}

	// Abilities bar
	
	
	if (!autoShootAvailable)
		autoShootText.setFillColor(Color(128, 128, 128));
	else if (autoShoot)
		autoShootText.setFillColor(Color(20, 150, 20));
	else
		autoShootText.setFillColor(Color(150, 20, 20));
	handle->draw(autoShootText);

	if (!autoJumpAvailable)
		autojumpText.setFillColor(Color(128, 128, 128));
	else if (autoJump)
		autojumpText.setFillColor(Color(20, 150, 20));
	else
		autojumpText.setFillColor(Color(150, 20, 20));
	handle->draw(autojumpText);
}

void Player::renderFrontView(RenderWindow* handle, Vector2f scale, Vector2f pos)
{
	if (!alive)
	{
		frontSprite.setColor(Color(255, 255, 255, 128));
	}
	frontSprite.setPosition(pos);
	handle->draw(frontSprite);
}

void Player::processInput(Keyboard::Key key, bool isPressed)
{
	if (!alive)
	{
		return;
	}

	if (isPressed)
	{
		if (key == Keyboard::Space)
		{
			jump = true;
		}
		else if (key == Keyboard::D)
		{
			if (abilityReload <= 0 && abilityAvailable)
			{
				useAbility();
			}
		}
		else if (key == Keyboard::A)
		{
			if (autoShootAvailable)
				autoShoot = !autoShoot;
		}
		else if (key == Keyboard::J)
		{
			if (autoJumpAvailable)
				autoJump = !autoJump;
		}
		else if (key == Keyboard::W)
		{
			if (canParachute)
			{
				parachute = true;
			}
		}
		else if (key == Keyboard::S)
		{
			if (canJetPack)
			{
				jetPack = true;
				sprite.setTexture(*textureManager->getTexture(Textures::PhysSideJetPack));
			}
		}
	}
	else
	{
		if (key == Keyboard::W)
		{
			parachute = false;
		}
		else if (key == Keyboard::S)
		{
			jetPack = false;
			sprite.setTexture(*textureManager->getTexture(Textures::PhysSide));
		}
	}
}

void Player::processInput(Vector2i pos, bool isPressed)
{
	if (!alive)
	{
		return;
	}
	
	if ((multitask || !autoShoot) && isPressed && reload <= 0)
	{
		shoot(pos);
		reload = reloadTime;
	}
	mouseHoverLocation = pos;
}

int Player::getRadius()
{
	return radius;
}

Vector2f Player::getPosition()
{
	return position;
}

float Player::getHealth()
{
	return health;
}

void Player::setHealth(float newHealth)
{
	health = newHealth;
}

void Player::resetInitHealth()
{
	initHealth = health;
}

void Player::resetMovement()
{
	velocity.y = 0;
	position.y = 100;
	if (rank >= 15)
	{
		// Turnips over rank 15 get 2 second invincibility by default
		invincibility = max(invincibility, 2.f);
	}
}

void Player::setParalysis()
{
	if (!paralysisImmunity)
	{
		paralyzed = true;
		paralyzeTime = 6;
	}
}

Types::Players Player::getType()
{
	return type;
}

bool Player::isParamedic()
{
	return paramedic;
}

void Player::initializeFunctionMaps()
{
	initializeMap[Types::Players::Normie] = &Player::normieInitialize;
	initializeMap[Types::Players::Math] = &Player::mathInitialize;
	initializeMap[Types::Players::Physics] = &Player::physInitialize;
	initializeMap[Types::Players::Chemistry] = &Player::chemInitialize;
	initializeMap[Types::Players::Biology] = &Player::bioInitialize;
	initializeMap[Types::Players::Geology] = &Player::geoInitialize;
	initializeMap[Types::Players::Engineer] = &Player::engInitialize;
	initializeMap[Types::Players::Nature] = &Player::natureInitialize;

	shootMap[Types::Players::Normie] = &Player::normieShoot;
	shootMap[Types::Players::Math] = &Player::mathShoot;
	shootMap[Types::Players::Physics] = &Player::physShoot;
	shootMap[Types::Players::Chemistry] = &Player::chemShoot;
	shootMap[Types::Players::Biology] = &Player::bioShoot;
	shootMap[Types::Players::Geology] = &Player::geoShoot;
	shootMap[Types::Players::Engineer] = &Player::engShoot;
	shootMap[Types::Players::Nature] = &Player::natureShoot;
}

void Player::initializeStats(Types::Players thisType, int playerUpgrades[12], int thisRank)
{

	type = thisType;
	for (int i = 0; i < 12; i++)
	{
		upgrades[i] = playerUpgrades[i];
	}

	// initializes sprite
	if (type == Types::Players::Normie)
	{
		sprite.setTexture(*textureManager->getTexture(Textures::NormieSide));
		frontSprite.setTexture(*textureManager->getTexture(Textures::NormieSmall));
	}
	else if (type == Types::Players::Math)
	{
		sprite.setTexture(*textureManager->getTexture(Textures::MathSide));
		frontSprite.setTexture(*textureManager->getTexture(Textures::MathSmall));
	}
	else if (type == Types::Players::Physics)
	{
		sprite.setTexture(*textureManager->getTexture(Textures::PhysSide));
		frontSprite.setTexture(*textureManager->getTexture(Textures::PhysSmall));
	}
	else if (type == Types::Players::Chemistry)
	{
		sprite.setTexture(*textureManager->getTexture(Textures::ChemSide));
		frontSprite.setTexture(*textureManager->getTexture(Textures::ChemSmall));
	}
	else if (type == Types::Players::Biology)
	{
		sprite.setTexture(*textureManager->getTexture(Textures::BioSide));
		frontSprite.setTexture(*textureManager->getTexture(Textures::BioSmall));
	}
	else if (type == Types::Players::Geology)
	{
		sprite.setTexture(*textureManager->getTexture(Textures::GeoSide));
		frontSprite.setTexture(*textureManager->getTexture(Textures::GeoSmall));
	}
	else if (type == Types::Players::Engineer)
	{
		sprite.setTexture(*textureManager->getTexture(Textures::EngSide));
		frontSprite.setTexture(*textureManager->getTexture(Textures::EngSmall));
	}
	else if (type == Types::Players::Nature)
	{
		sprite.setTexture(*textureManager->getTexture(Textures::NatureSide));
		frontSprite.setTexture(*textureManager->getTexture(Textures::NatureSmall));
	}
	sprite.setOrigin(45.f, 45.f);

	regen = false;
	canJetPack = false;
	canParachute = false;
	paralyzed = false;
	abilityAvailable = false;

	(this->*initializeMap[type])();

	rank = thisRank;

	if (rank >= 5)
	{
		autoShootAvailable = true;
		autoReloadTime = reloadTime * 1.4;
		autoReload = autoReloadTime;
	}
	if (rank >= 10)
	{
		damage *= 1.25;
	}
	if (rank >= 15)
	{
		invincibility = 2.f;
	}
	if (rank >= 20)
	{
		reloadTime /= 1.25;
		autoReloadTime = reloadTime * 1.4;
	}
	if (rank >= 25)
	{
		autoJumpAvailable = true;
	}
	if (rank >= 30)
	{
		shotSpeed *= 1.25;
	}
	if (rank >= 35)
	{
		switchResistance *= 0.66f;
	}
	if (rank >= 40)
	{
		health *= 1.25;
		initHealth *= health;
	}
	if (rank >= 45)
	{
		invincibility = 5.f;
	}
	if (rank >= 50)
	{
		autoReloadTime = 1.2*reloadTime;
		autoReload = autoReloadTime;
	}
	if (rank >= 60)
	{
		bulletSize *= 1.25;
	}
	if (rank >= 70)
	{
		abilityReloadTime *= 0.75;
	}
	if (rank >= 80)
	{
		health *= 1.25;
	}
	if (rank >= 90)
	{
		reloadTime /= 1.25;
		autoReloadTime = reloadTime * 1.2;
	}
	if (rank >= 100)
	{
		switchResistance *= 0.5f;
	}
	if (rank >= 110)
	{
		damage *= 1.25;
	}
	if (rank >= 120)
	{
		autoReloadTime = reloadTime;
	}

	damage *= (1 + (rank - 1) / 120.0);
	shotSpeed *= (1 + (rank - 1) / 120.0);
	reloadTime /= (1 + (rank - 1) / 120.0);
	shotSpeed *= (1 + (rank - 1) / 120.0);

	if (!(type == Types::Players::Biology && upgrades[6] == 2))
	{
		healthCap = 1.5 * health;
	}
}

void Player::shoot(Vector2i pos)
{	
	(this->*shootMap[type])(pos);
}

void Player::takeDamage(Obstacle* obstacle)
{
	if (isInvincible())
		return;

	float thisDamage = obstacle->getDamage();

	shield *= 0.92;

	thisDamage *= resistance;
	thisDamage *= 1 - shield;

	if (obstacle->getType() == Types::Obstacles::Bird ||
		obstacle->getType() == Types::Obstacles::Eagle ||
		obstacle->getType() == Types::Obstacles::Flappy ||
		obstacle->getType() == Types::Obstacles::MrGoose)
	{
		thisDamage *= birdResistance;
	}
	else if (obstacle->getType() == Types::Obstacles::Fireball)
	{
		thisDamage *= fireballResistance;
	}
	else if (obstacle->getType() == Types::Obstacles::Cloud)
	{

	}
	loseHealth(min(thisDamage, obstacle->getHealth()));

	if (type == Types::Players::Normie && upgrades[11] == 2 && !abilityActivated && abilityReload >= 3.f)
	{
		float tempReload = abilityReload;
		useAbility();
		abilityTimeLeft = 0.7f;
		abilityReload = max(5.f, tempReload);
	}
}

void Player::useAbility()
{
	abilityActivated = true;
	abilityReload = abilityReloadTime;

	if (type == Types::Players::Normie)
	{
		abilityTimeLeft = 4;
		reloadTime /= 2;
		autoReloadTime /= 2;
		damage *= 1.5f;
		resistance *= 0.7;

	}
	else if (type == Types::Players::Math)
	{
		damage *= 1.25f;
		abilityTimeLeft = 8;
	}
	else if (type == Types::Players::Physics)
	{
		shoot(mouseHoverLocation);
		abilityActivated = false;
	}
	else if (type == Types::Players::Chemistry)
	{
		for (int i = 0; i < scene->obstacles.size(); i++)
		{
			Obstacle* obstacle = scene->obstacles[i];
			obstacle->freeze(8);
			obstacle->removeFade();
			if (obstacle->getType() == Types::Obstacles::Fireball)
			{
				obstacle->setHealth(obstacle->getHealth() / 2);
			}
			else
			{
				obstacle->burn(3, damage);
			}
		}
		abilityActivated = false;
	}
	else if (type == Types::Players::Biology)
	{
		for (int i = 0; i < scene->obstacles.size(); i++)
		{
			Obstacle* obstacle = scene->obstacles[i];
			if (obstacle->getType() == Types::Obstacles::Bird ||
				obstacle->getType() == Types::Obstacles::Eagle ||
				obstacle->getType() == Types::Obstacles::Flappy)
			{
				health += 6;
				obstacle->setHealth(obstacle->getHealth() * 0.6);
				obstacle->poison(8.f, 3 * damage);
			}
		}
		abilityActivated = false;
	}
	else if (type == Types::Players::Geology)
	{
		for (int i = 0; i < 40; i++)
		{
			Vector2f vel = Vector2f(10.f * (0.5 - Math::random()), 0.f);
			Vector2f pos = Vector2f(20 + 32 * i, -50 - Math::random() * 50);
			Projectile* shot = new Projectile(Color(128, 128, 128), scene, pos);
			shot->setStats(vel, bulletSize, 4*damage, shotPierce, shotLifeSpan);
			scene->addPlayerProjectile(shot);
		}
		abilityActivated = false;
	}
	else if (type == Types::Players::Engineer)
	{
		reloadTime /= 10;
		autoReloadTime /= 10;
		abilityTimeLeft = 4;
	}
	else if (type == Types::Players::Nature)
	{
		// Blow back
		if (upgrades[3] == 2)
		{
			for (int i = 0; i < scene->obstacles.size(); i++)
			{
				Obstacle* obstacle = scene->obstacles.at(i);
				obstacle->blowBack(3);
			}
		}

		// Lightning
		if (upgrades[7] == 2)
		{
			if (scene->obstacles.size() > 0)
			{
				// Find closest obstacle
				float minDist = Math::distance(position, scene->obstacles.at(0)->getPosition());
				Obstacle* closestObstacle = scene->obstacles.at(0);
				for (int i = 1; i < scene->obstacles.size(); i++)
				{
					Obstacle* obstacle = scene->obstacles.at(i);
					if (Math::distance(position, obstacle->getPosition()) < minDist)
					{
						minDist = Math::distance(position, obstacle->getPosition());
						closestObstacle = obstacle;
					}
				}

				// shock obstacle
				closestObstacle->shock();
				Entity* lightning = new Lightning(Types::Entities::Lightning, NULL, scene,
					position, closestObstacle->getPosition());
				scene->addEntity(lightning);
			}
		}

		// Storm
		if (upgrades[11] == 2)
		{
			for (int i = 0; i < 40; i++)
			{
				Vector2f vel = Vector2f(0, 0.f);
				Vector2f pos = Vector2f(20 + 32 * i, -50 - Math::random() * 50);
				Projectile* shot = new Projectile(textureManager->getTexture(Textures::Raindrop), scene, pos);
				shot->setKnockback(5);
				shot->setStats(vel, 6, 3*damage, shotPierce, shotLifeSpan);
				scene->addPlayerProjectile(shot);
			}
		}

		abilityActivated = false;
	}
}

void Player::cancelAbility()
{
	abilityActivated = false;
	if (type == Types::Players::Normie)
	{
		reloadTime *= 2;
		autoReloadTime *= 2;
		damage /= 1.5f;
		resistance /= 0.7;
	}
	else if (type == Types::Players::Math)
	{
		damage /= 1.25f;
	}
	else if (type == Types::Players::Physics)
	{
		abilityTimeLeft = 100;
	}
	else if (type == Types::Players::Engineer)
	{
		reloadTime *= 10;
		autoReloadTime *= 10;
		abilityTimeLeft = 4;
	}
}

// INITIALIZE MAPS
void Player::normieInitialize()
{
	// Base stats
	health = 200;
	damage = 12;
	reloadTime = 180;
	reload = reloadTime;
	bulletSize = 6;

	if (upgrades[0] == 2)
	{
		bulletSize *= 1.25;
	}
	if (upgrades[1] == 2)
	{
		shotSpeed *= 1.2;
	}
	if (upgrades[2] == 2)
	{
		reloadTime /= 1.3;
	}
	if (upgrades[3] == 2)
	{
		reloadTime /= 1.2;
		damage *= 1.3;
		bulletSize *= 1.25;
	}
	if (upgrades[4] == 2)
	{
		health *= 1.4;
	}
	if (upgrades[5] == 2)
	{
		tooHighResistance = 0.6;
	}
	if (upgrades[6] == 2)
	{
		multitask = true;
	}
	if (upgrades[7] == 2)
	{
		abilityAvailable = true;
		abilityReloadTime = 25;
	}
	if (upgrades[8] == 2)
	{
		shotPierce = 3;
	}
	if (upgrades[9] == 2)
	{
		birdResistance *= 0.8;
	}
	if (upgrades[10] == 2)
	{
		// implemented
	}
	if (upgrades[11] == 2)
	{
		// implemented
	}

	initHealth = health;

}

void Player::mathInitialize()
{
	// Base stats
	health = 200;
	damage = 16;
	reloadTime = 160;
	reload = reloadTime;
	bulletSize = 7;

	if (upgrades[0] == 2)
	{
		reloadTime /= 1.3;
	}
	if (upgrades[1] == 2)
	{
		reloadTime /= 1.3;
	}
	if (upgrades[2] == 2)
	{
		damage *= 1.3;
		bulletSize *= 2;
	}
	if (upgrades[3] == 2)
	{
		// implemented
	}
	if (upgrades[4] == 2)
	{
		health *= 1.5;
		reloadTime *= 1.1;
	}
	if (upgrades[5] == 2)
	{
		shotSpeed *= 1.25;
	}
	if (upgrades[6] == 2)
	{
		damage *= 1.5;
		reloadTime *= 1.1;
	}
	if (upgrades[7] == 2)
	{
		abilityAvailable = true;
		abilityReloadTime = 12;
	}
	if (upgrades[8] == 2)
	{
		shotSpeed *= 1.35;
	}
	if (upgrades[9] == 2)
	{
		damage *= 0.5;
		// implemented
	}
	if (upgrades[10] == 2)
	{
		// implemented
	}
	if (upgrades[11] == 2)
	{
		damage *= (1 + (rank - 1) / 120.0);
		shotSpeed *= (1 + (rank - 1) / 120.0);
		reloadTime /= (1 + (rank - 1) / 120.0);
		shotSpeed *= (1 + (rank - 1) / 120.0);
	}

	initHealth = health;
}

void Player::physInitialize()
{
	// Base stats
	health = 180;
	damage = 14;
	reloadTime = 150;
	reload = reloadTime;
	bulletSize = 8;
	shotPierce = 2;

	if (upgrades[0] == 2)
	{
		shotSpeed *= 1.3;
		damage *= 1.2;
		bulletSize *= 1.1;
	}
	if (upgrades[1] == 2)
	{
		// implemented
	}
	if (upgrades[2] == 2)
	{
		shotPierce += 2;
		// implemented
	}
	if (upgrades[3] == 2)
	{
		canParachute = true;
	}
	if (upgrades[4] == 2)
	{
		reloadTime /= 1.3;
	}
	if (upgrades[5] == 2)
	{
		reloadTime /= 1.5;
	}
	if (upgrades[6] == 2)
	{
		// implemented
	}
	if (upgrades[7] == 2)
	{
		abilityAvailable = true;
		abilityReloadTime = 12;
	}
	if (upgrades[8] == 2)
	{
		shotPierce += 8;
	}
	if (upgrades[9] == 2)
	{
		resistance *= 0.75;
	}
	if (upgrades[10] == 2)
	{
		// implemented
	}
	if (upgrades[11] == 2)
	{
		canJetPack = true;
	}

	initHealth = health;
}

void Player::chemInitialize()
{
	// Base stats
	health = 240;
	damage = 11;
	reloadTime = 180;
	reload = reloadTime;
	bulletSize = 6;

	if (upgrades[0] == 2)
	{
		//implemented
	}
	if (upgrades[1] == 2)
	{
		//implemented
	}
	if (upgrades[2] == 2)
	{
		//implemented
	}
	if (upgrades[3] == 2)
	{
		//implemented
	}
	if (upgrades[4] == 2)
	{
		damage *= 1.5;
		fireballResistance *= 1.25;
	}
	if (upgrades[5] == 2)
	{
		//implemented
	}
	if (upgrades[6] == 2)
	{
		shotSpeed *= 1.2;
	}
	if (upgrades[7] == 2)
	{
		abilityAvailable = true;
		abilityReloadTime = 12;
	}
	if (upgrades[8] == 2)
	{
		// implemented
	}
	if (upgrades[9] == 2)
	{
		resistance *= 0.75;
	}
	if (upgrades[10] == 2)
	{
		reloadTime *= 1.1;
	}
	if (upgrades[11] == 2)
	{
		// implemented
	}

	initHealth = health;
}

void Player::bioInitialize()
{
	// Base stats
	health = 300;
	damage = 10;
	reloadTime = 190;
	reload = reloadTime;
	bulletSize = 6;

	if (upgrades[0] == 2)
	{
		health *= 1.5;
	}
	if (upgrades[1] == 2)
	{
		shotSpeed *= 1.2;
	}
	if (upgrades[2] == 2)
	{
		damage *= 1.25;
		shotSpeed *= 1.5;
		reloadTime /= 1.2;
	}
	if (upgrades[3] == 2)
	{
		regen = true;
	}
	if (upgrades[4] == 2)
	{
		birdResistance *= 0.6;
	}
	if (upgrades[5] == 2)
	{
		// implemented
	}
	if (upgrades[6] == 2)
	{
		healthCap = 4 * health;
	}
	if (upgrades[7] == 2)
	{
		abilityAvailable = true;
		abilityReloadTime = 25;
	}
	if (upgrades[8] == 2)
	{
		paramedic = true;
	}
	if (upgrades[9] == 2)
	{
		// Handled by death
		// Not implemented
	}
	if (upgrades[10] == 2)
	{
		// implemented
	}
	if (upgrades[11] == 2)
	{
		damage *= (1.f + (rank - 1) / 120.0);
		shotSpeed *= (1.f + (rank - 1) / 120.0);
		reloadTime /= (1.f + (rank - 1) / 120.0);
	}

	initHealth = health;
}

void Player::geoInitialize()
{
	// Base stats
	health = 230;
	damage = 14;
	reloadTime = 200;
	reload = reloadTime;
	bulletSize = 9;

	if (upgrades[0] == 2)
	{
		bulletSize *= 1.5;
	}
	if (upgrades[1] == 2)
	{
		damage *= 1.35;
	}
	if (upgrades[2] == 2)
	{
		shotSpeed *= 1.25;
	}
	if (upgrades[3] == 2)
	{
		// implemented
	}
	if (upgrades[4] == 2)
	{
		// implemented
	}
	if (upgrades[5] == 2)
	{
		resistance *= 0.8;
		fireballResistance *= 0.7;
	}
	if (upgrades[6] == 2)
	{
		// implemented
	}
	if (upgrades[7] == 2)
	{
		abilityAvailable = true;
		abilityReloadTime = 10;
	}
	if (upgrades[8] == 2)
	{
		shotPierce = 2;
	}
	if (upgrades[9] == 2)
	{
		tooLowResistance = 0.6;
	}
	if (upgrades[10] == 2)
	{
		// implemented
	}
	if (upgrades[11] == 2)
	{
		damage *= 1.3;
		// implemented
	}

	initHealth = health;
}

void Player::engInitialize()
{
	// Base stats
	health = 200;
	damage = 13;
	reloadTime = 160;
	reload = reloadTime;
	bulletSize = 7;

	if (upgrades[0] == 2)
	{
		resistance *= 0.85;
	}
	if (upgrades[1] == 2)
	{
		reloadTime /= 1.3;
	}
	if (upgrades[2] == 2)
	{
		reloadTime /= 1.3;
		damage *= 1.3;
	}
	if (upgrades[3] == 2)
	{
		paralysisImmunity = true;
	}
	if (upgrades[4] == 2)
	{
		shotSpeed *= 1.15;
		bulletSize *= 1.1;
	}
	if (upgrades[5] == 2)
	{
		switchResistance *= 0.5;
	}
	if (upgrades[6] == 2)
	{
		shield = 1;
	}
	if (upgrades[7] == 2)
	{
		abilityAvailable = true;
		abilityReloadTime = 25;
	}
	if (upgrades[8] == 2)
	{
		// implemented
	}
	if (upgrades[9] == 2)
	{
		// implemented
	}
	if (upgrades[10] == 2)
	{
		shotSpeed *= 0.2;
		shotLifeSpan = 2;
	}
	if (upgrades[11] == 2)
	{
		shotLifeSpan = 5;
		damage *= 1.4;
		bulletSize *= 1.25;
	}

	initHealth = health;
}

void Player::natureInitialize()
{
	// Base stats
	health = 260;
	damage = 12;
	reloadTime = 180;
	reload = reloadTime;
	bulletSize = 6;

	if (upgrades[0] == 2)
	{
		tooHighResistance *= 0.5;
	}
	if (upgrades[1] == 2)
	{
		birdResistance *= 0.7;
		fireballResistance *= 0.7;
	}
	if (upgrades[2] == 2)
	{
		damage *= 1.4;
		shotSpeed *= 1.3;
		shotPierce++;
	}
	if (upgrades[3] == 2)
	{
		abilityAvailable = true;
		abilityReloadTime = 20;
	}
	if (upgrades[4] == 2)
	{
		switchResistance *= 0.5;
	}
	if (upgrades[5] == 2)
	{
		// implemented
	}
	if (upgrades[6] == 2)
	{
		health *= 1.4;
	}
	if (upgrades[7] == 2)
	{
		if (abilityAvailable)
		{
			abilityReloadTime *= 0.9;
		}
		else
		{
			abilityAvailable = true;
			abilityReloadTime = 20;
		}
	}
	if (upgrades[8] == 2)
	{
		tooLowResistance = 0.5;
	}
	if (upgrades[9] == 2)
	{
		reloadTime *= 1.1;
		damage *= 1.25;
	}
	if (upgrades[10] == 2)
	{
		reloadTime /= 1.2;
		bulletSize *= 1.2;
	}
	if (upgrades[11] == 2)
	{
		if (abilityAvailable)
		{
			abilityReloadTime *= 0.9;
		}
		else
		{
			abilityAvailable = true;
			abilityReloadTime = 20;
		}
	}

	initHealth = health;
}

// SHOT MAPS

void Player::normieShoot(Vector2i pos)
{

	Vector2f vel = Math::vector(shotSpeed, Math::direction(position, Vector2f(pos.x, pos.y)));
	Projectile* shot = new Projectile(Color(50, 150, 50), scene,position);
	shot->setStats(vel, bulletSize, damage, shotPierce, shotLifeSpan);
	if (upgrades[10] == 2)
	{
		if (rand() / (RAND_MAX + 1.0) < 0.15)
		{
			shot->setStun(2);
		}
	}
	scene->addPlayerProjectile(shot);
}

void Player::mathShoot(Vector2i pos)
{
	Vector2f vel = Math::vector(shotSpeed, Math::direction(position, Vector2f(pos.x, pos.y)));
	Projectile* shot = new Projectile(Color(255, 192, 203), scene, position);
	float thisDamage = damage;
	if (upgrades[3] == 2)
	{
		shot->setSplit(3);
		shot->setContinuousSplit();
		shot->setResilience(0.67);
	}
	if (upgrades[9] == 2)
	{
		if (rand() / (RAND_MAX + 1.0) < 0.25)
		{
			thisDamage *= 10;
		}
	}
	if (upgrades[10] == 2)
	{
		if (rand() / (RAND_MAX + 1.0) < 0.25)
		{
			shot->setStun(2);
		}
	}

	shot->setStats(vel, bulletSize, thisDamage, shotPierce, shotLifeSpan);
	scene->addPlayerProjectile(shot);
	if (abilityActivated)
	{
		Projectile* shot2 = new Projectile(*shot);
		shot2->setPosition(Vector2f(position.x + 15, position.y));
		shot->setVelocity(Math::rotate(shot->getVelocity(), -0.15));
		scene->addPlayerProjectile(shot2);
	}
}

void Player::physShoot(Vector2i pos)
{
	Vector2f vel = Math::vector(shotSpeed, Math::direction(position, Vector2f(pos.x, pos.y)));
	Projectile* shot;
	if (abilityActivated)
	{
		shot = new Projectile(textureManager->getTexture(Textures::WreckingBall), scene, position);
		shot->setStats(vel, 80, 20*damage, 2*shotPierce, shotLifeSpan);
		shot->setKnockback(50);
	}
	else
	{
		shot = new Projectile(Color(255, 255, 100), scene, position);
		shot->setStats(vel, bulletSize, damage, shotPierce, shotLifeSpan);
	}
	
	if (upgrades[1] == 2 && !abilityActivated) // to not overwrite wrecking ball knockback
	{
		shot->setKnockback(bulletSize/5.f);
	}
	if (upgrades[2] == 2)
	{
		shot->setBounce();
		shot->setResilience(0.9);
	}
	if (upgrades[6] == 2)
	{
		shot->setNuclear();
	}
	if (upgrades[10] == 2)
	{
		shot->setFadeRemove();
	}
	scene->addPlayerProjectile(shot);
}

void Player::chemShoot(Vector2i pos)
{
	Vector2f vel = Math::vector(shotSpeed, Math::direction(position, Vector2f(pos.x, pos.y)));
	Projectile* shot = new Projectile(Color(100, 100, 255), scene, position);
	shot->setStats(vel, bulletSize, damage, shotPierce, shotLifeSpan);

	float burnTime = 0;
	float poisonTime = 0;
	float slowTime = 0;
	float freezeTime = 0;
	float chanceMultiplier = 1;

	if (upgrades[10] == 2)
	{
		chanceMultiplier = 1.5;
	}
	if (upgrades[0] == 2)
	{
		if (Math::random() < 0.2 * chanceMultiplier)
		{
			burnTime = 8;
		}
	}
	if (upgrades[1] == 2)
	{
		if (Math::random() < 0.4 * chanceMultiplier)
		{
			slowTime = 7;
		}
	}
	if (upgrades[2] == 2)
	{
		if (Math::random() < 0.15 * chanceMultiplier)
		{
			freezeTime = 3.5;
		}
	}
	if (upgrades[5] == 2)
	{
		shot->setFadeRemove();
	}
	if (upgrades[8] == 2)
	{
		if (Math::random() < 0.2 * chanceMultiplier)
		{
			poisonTime = 8;
		}
	}
	if (upgrades[11] == 2)
	{
		shot->setSplit(3);
	}
	if (upgrades[3] == 2)
	{
		burnTime *= 1.3;
		poisonTime *= 1.3;
		slowTime *= 1.3;
		freezeTime *= 1.3;
	}
	shot->setBurn(burnTime);
	shot->setPoison(poisonTime);
	shot->setSlow(slowTime);
	shot->setFreeze(freezeTime);
	scene->addPlayerProjectile(shot);
}

void Player::bioShoot(Vector2i pos)
{
	Vector2f vel = Math::vector(shotSpeed, Math::direction(position, Vector2f(pos.x, pos.y)));
	Projectile* shot = new Projectile(Color(50, 150, 50), scene, position);
	float thisDamage = damage;

	if (upgrades[5] == 2)
	{
		if (Math::random() < 0.25)
		{
			shot->setPoison(5);
		}
	}
	if (upgrades[10] == 2)
	{
		if (health < initHealth && health > 0.01)
		{
			thisDamage *= min(50.f, initHealth / health);
		}
	}

	shot->setStats(vel, bulletSize, thisDamage, shotPierce, shotLifeSpan);
	scene->addPlayerProjectile(shot);
}

void Player::geoShoot(Vector2i pos)
{
	Vector2f vel = Math::vector(shotSpeed, Math::direction(position, Vector2f(pos.x, pos.y)));
	Projectile* shot = new Projectile(Color(128, 128, 128), scene, position);
	int thisPierce = shotPierce;
	int thisSize = bulletSize;
	int thisDamage = damage;
	
	if (upgrades[3] == 2)
	{
		if (Math::random() < 0.05)
		{
			thisSize *= 3;
			thisDamage *= 3;
		}
	}
	if (upgrades[4] == 2)
	{
		shot->setGoldReleaseChance(0.03);
	}
	if (upgrades[6] == 2)
	{
		if (Math::random() < 0.4)
		{
			shot->setSplit(5);
			thisPierce = 1;
		}
	}
	if (upgrades[10] == 2)
	{
		if (Math::random() < 0.2)
		{
			shot->setBurn(5);
		}
	}
	if (upgrades[11] == 2)
	{
		if (Math::random() < 0.4)
		{
			shot->setBurn(5);
		}
	}

	shot->setStats(vel, thisSize, thisDamage, thisPierce, shotLifeSpan);
	scene->addPlayerProjectile(shot);
}

void Player::engShoot(Vector2i pos)
{
	Vector2f vel = Math::vector(shotSpeed, Math::direction(position, Vector2f(pos.x, pos.y)));

	if (upgrades[1] == 2)
	{
		// reduced accuracy
		vel = Math::rotate(vel, (Math::random() - 1) * 0.5);
	}
	Projectile* shot = new Projectile(Color(255, 200, 200), scene, position);
	if (upgrades[8] == 2)
	{
		shot->setSplit(3);
		shot->setResilience(0.7);
	}
	if (upgrades[9] == 2)
	{
		if (Math::random() < 0.2)
		{
			shot->setStun(1.5);
		}
	}
	if (upgrades[10] == 2)
	{
		shot->setFollow();
	}
	shot->setStats(vel, bulletSize, damage, shotPierce, shotLifeSpan);
	scene->addPlayerProjectile(shot);
}

void Player::natureShoot(Vector2i pos)
{
	Vector2f vel = Math::vector(shotSpeed, Math::direction(position, Vector2f(pos.x, pos.y)));
	Projectile* shot = new Projectile(Color(200, 255, 200), scene, position);
	shot->setStats(vel, bulletSize, damage, shotPierce, shotLifeSpan);
	
	if (upgrades[5] == 2)
	{
		shot->setCloudImmunity();
	}

	scene->addPlayerProjectile(shot);
}

void Player::doSwitchDamage()
{
	// depends on rank
	health -= 30 * switchResistance;
}

bool Player::isAlive()
{
	return alive;
}

bool Player::isInvincible()
{
	if (invincibility > 0)
		return true;
	else
		return false;
}

int Player::getXp()
{
	return (int)xpGain;
}

void Player::gainXp(float xp)
{
	xpGain += xp;
}

float Player::getDamage()
{
	return damage;
}

void Player::setDamage(float newDamage)
{
	damage = newDamage;
}

void Player::drain(Time dt)
{
	if (health >= 5)
	{
		loseHealth(0.02 * (health)* dt.asSeconds());
	}
}

void Player::loseHealth(float amount)
{
	if (invincibility > 0)
	{
		return;
	}
	health -= amount;
}

void Player::gainHealth(float amount)
{
	health += amount;
	if (health >= healthCap)
	{
		health = healthCap;
	}
}