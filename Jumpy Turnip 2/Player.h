#pragma once

#include <SFML\Graphics.hpp>

#include "GameObject.h"
#include "TextureManager.h"

class GameScene;
class Projectile;
class Obstacle;

using namespace sf;
using namespace std;

class Player : public GameObject
{
public:
	typedef void(Player::*shootFunction)(Vector2i pos);
	typedef void(Player::*initializeFunction)();

	Player(TextureManager* manager, GameScene* thisScene);
	void initializeStats(Types::Players thisType, int playerUpgrades[12], int rank);
	void update(Time dt);
	void render(RenderWindow* handle, Vector2f scale);
	void renderFrontView(RenderWindow* handle, Vector2f scale, Vector2f pos);
	void processInput(Keyboard::Key key, bool isPressed);
	void processInput(Vector2i pos, bool isPressed);

	void shoot(Vector2i pos);
	int getRadius();
	Vector2f getPosition();

	float getHealth();
	void setHealth(float newHealth);
	void resetInitHealth();
	float getDamage();
	void setDamage(float newDamage);
	Types::Players getType();
	void resetMovement();

	void takeDamage(Obstacle* obstacle);
	void useAbility();
	void cancelAbility();
	void setParalysis();

	// Initialize functions by type
	void normieInitialize();
	void mathInitialize();
	void physInitialize();
	void chemInitialize();
	void bioInitialize();
	void geoInitialize();
	void engInitialize();
	void natureInitialize();

	// Shoot functions by type
	void normieShoot(Vector2i pos);
	void mathShoot(Vector2i pos);
	void physShoot(Vector2i pos);
	void chemShoot(Vector2i pos);
	void bioShoot(Vector2i pos);
	void geoShoot(Vector2i pos);
	void engShoot(Vector2i pos);
	void natureShoot(Vector2i pos);

	void doSwitchDamage();

	bool isAlive();
	bool isInvincible();

	int getXp();
	void gainXp(float xp);
	void loseHealth(float amount);
	void gainHealth(float amount);
	bool isParamedic();

	void drain(Time dt);

private:
	void initializeFunctionMaps();

	bool alive = true;

	// Effects the player can have
	bool paralyzed;
	float paralyzeTime;
	bool canParachute;
	bool parachute;
	bool canJetPack;
	bool jetPack;

	// Speed health damage stuff
	float initHealth;
	float health;
	float healthCap;
	bool regen;
	float reloadTime;
	float reload;
	float autoReloadTime;
	float autoReload;
	
	float resistance = 1;
	float birdResistance = 1;
	float fireballResistance = 1;
	bool paralysisImmunity = false;
	bool paramedic;
	float tooHighResistance = 1;
	float tooLowResistance = 1;
	float switchResistance = 1;
	float shield;

	// Ability
	bool abilityAvailable;
	float abilityReloadTime; // max
	float abilityReload;
	bool abilityActivated;
	float abilityTimeLeft;

	// The following are for the purposes of creating projectiles only
	float damage;
	int shotPierce = 1;
	float bulletSize;
	float shotSpeed = 1500;
	float shotLifeSpan = 5;
	bool autoShootAvailable;
	bool autoShoot;
	bool autoJumpAvailable;
	bool autoJump;
	Vector2i mouseHoverLocation;

	// Other upgrade stuff
	bool multitask;

	// starting out invincible
	float invincibility;

	// Random facts about turnip, not interesting
	bool jump;
	const float radius = 30;
	Vector2f velocity;
	Vector2f position;
	Texture* playerTexture;
	TextureManager* textureManager;
	Sprite sprite;
	Sprite frontSprite;
	Sprite parachuteSprite;
	GameScene* scene;
	Types::Players type;

	Text autoShootText;
	Text autojumpText;

	// Maps of type to respective function
	map<Types::Players, shootFunction> shootMap;
	map<Types::Players, initializeFunction> initializeMap;

	int upgrades[12];
	int rank;
	
	float xpGain;
};