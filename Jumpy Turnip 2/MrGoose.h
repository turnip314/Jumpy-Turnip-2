#pragma once
#include "Bird.h"
class MrGoose : public Bird
{
public:
	MrGoose(Types::Obstacles thisType, Texture* textureClosed, Texture* textureOpen, Texture* fireballTexture1, Texture* fireballTexture2, GameScene* thisScene);
	~MrGoose();

	void update(Time dt);
	void die();
	void setStats(float speed, float newHealth, float newDamage, bool fades, bool regens);
	bool hasCollided(Projectile* projectile);
	bool hasCollided(Player* projectile);
	void knockback(Vector2f direction);

private:
	float attackPhase;
	void attack();
	Texture* fireballTexture1;
	Texture* fireballTexture2;
	Texture* attackTexture;
	float cooldown;

	const float length = 500.f;
	const float height = 70.f;
};

