#pragma once
#include "Bird.h"

class Flappy : public Bird
{
public:
	Flappy(Types::Obstacles thisType, Texture* texture, GameScene* thisScene);
	void update(Time dt);
	void render(RenderWindow* handle, Vector2f scale);

	~Flappy();

private:
	void jump();
	float velToJump;

};

