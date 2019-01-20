#pragma once
#include "Bird.h"

class Flappy : public Bird
{
public:
	Flappy(Types::Obstacles thisType, Texture* texture, GameScene* thisScene);

	// Inhereted and overidden from base class, see implementation
	void update(Time dt);
	void render(RenderWindow* handle, Vector2f scale);

	~Flappy();

private:
	// Only obstacle that jumps
	void jump();
	float velToJump;

};

