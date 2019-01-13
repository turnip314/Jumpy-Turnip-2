#pragma once
#include "GameObject.h"
//#include "GameScene.h"

class GameScene;
class Obstacle;

class Projectile : public GameObject
{
public:
	Projectile(Texture* texture, GameScene* thisScene, Vector2f pos);
	Projectile(Color color, GameScene* thisScene, Vector2f pos);
	void setStats(Vector2f newVelocity, int newRadius, int newDamage, int newPierce, float newLifespan);
	void setSplit(int newSplitNum);
	void setContinuousSplit();
	void setFollow();
	void setStun(float time);
	void setSlow(float time);
	void setFreeze(float time);
	void setBurn(float time);
	void setPoison(float time);
	void setFadeRemove();
	void setKnockback(float power);
	void setBounce();
	void setNuclear();
	void setResilience(float newResilience);

	void setCloudImmunity();
	void setGoldReleaseChance(float chance);

	void clearEffects(Obstacle* obstacle);

	void setPosition(Vector2f newPos);

	void update(Time dt);
	void render(RenderWindow* handle, Vector2f scale);
	void processInput(Keyboard::Key, bool isPressed);
	void processInput(Vector2i pos, bool isPressed);
	Vector2f getPosition();
	Vector2f getVelocity();
	void setVelocity(Vector2f vel);
	int getRadius();

	// The following two are different
	// getDamage only returns a value while
	// doDamage also lowers pierce by 1
	int getDamage();
	void doDamage(Obstacle* obstacle);
	void doNuclearDamage(Obstacle* obstacle, Time dt);

	Projectile(const Projectile &p2);
	~Projectile();

private:
	void addToRemoveList();

	bool hasTexture;
	Color color;
	Sprite sprite;

	Vector2f position;
	Vector2f velocity;
	Vector2f lastMousePos;


	GameScene* scene;

	int radius;
	int pierce = 1;
	int damage = 0;
	float lifespan = 0;

	int splitNum = 0;
	bool continuousSplit = false;
	bool follow = false;
	float stun;
	float slow;
	float freeze;
	float burn;
	float poison;
	bool fadeRemove;
	bool knockback;
	float knockbackPower;
	bool bounce;
	bool nuclear;
	bool cloudImmunity;
	float goldReleaseChance;
	float speed;

	float resilience = 1.f;

	bool removed;

	vector<Obstacle*> alreadyHit;
};
