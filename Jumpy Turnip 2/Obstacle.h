#pragma once
#include "GameObject.h"

class GameScene;
class Projectile;
class Player;

class Obstacle : public GameObject
{
public:
	// Might not need rad or pos in param, have it decided in constructor
	Obstacle(Types::Obstacles thisType, Texture* thisTexture, GameScene* thisScene);
	virtual void update(Time dt);
	virtual void render(RenderWindow* handle, Vector2f scale);
	virtual void processInput(Keyboard::Key, bool isPressed);
	virtual void processInput(Vector2i pos, bool isPressed);

	// Overloaded with projectile and player collisions
	virtual bool hasCollided(Projectile* projectile);
	virtual bool inVincinity(Projectile* projectile);
	virtual bool hasCollided(Player* player);
	virtual void doCollision(Player* player);
	virtual void die();

	float getHealth();
	void setHealth(float health);
	float getDamage();
	Types::Obstacles getType();

	bool isShocked();

	void addImmune(Projectile* projectile);
	virtual void setPosition(Vector2f newPos);
	Vector2f getPosition();
	virtual void setStats(float speed, float newHealth, float newDamage, bool fades, bool regens);
	void setVelocity(Vector2f newVel);
	Vector2f getVelocity();

	// Effects that projectile can pass on
	void stun(float time, float damage);
	void slow(float time);
	void freeze(float time);
	// Fireballs are immune
	virtual void burn(float time, float damage);
	// Non bird objects are immune
	virtual void poison(float time, float damage);
	void removeFade();
	virtual void knockback(Vector2f direction);
	void shock();
	void blowBack(float time);

	void setDropChance(float dropChance);

	~Obstacle();

protected:
	void addToRemoveList();

	Vector2f position;
	Vector2f velocity;

	GameScene* scene;
	Texture* texture;
	Sprite sprite;

	Types::Obstacles type;

	// Either already hit or immune in general
	vector<Projectile*> immune;

	int radius;
	float phase;
	bool fade; // has not been used
	bool regen;

	// Effects
	float stunned;
	float stunDamage;
	float slowed;
	float frozen;
	float burned;
	float burnDamage;
	float poisoned;
	float poisonDamage;
	float knockedBack;
	Vector2f knockbackDirection;
	bool shocked;
	float blownBack;

	// Normal stats
	float health;
	float initHealth;
	bool alive;
	float damage;

	// Deploy value will be an integer from 1 to 1000
	// Will determine probability that it gets deployed in endless
	// modes. Probability is (value - next highest value) / 1000
	int deployValue;

	float score;
	float dropChance;
	bool removed;

};

