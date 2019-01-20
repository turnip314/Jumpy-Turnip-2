#pragma once
#include "Entity.h"

class Coin : public Entity
{
public:
	Coin(Types::Entities thisType, Texture* texture, GameScene* thisScene);
	~Coin();

	// Inhereted and overidden from base class, see implementation
	void update(Time dt);
	void render(RenderWindow* handle, Vector2f scale);
	void processInput(Keyboard::Key, bool isPressed);
	void processInput(Vector2i pos, bool isPressed);
	void setPosition(Vector2f pos);

private:
	// Spin phase and time to new phase
	int phase;
	float phaseTimer = 0.05f;

	Sprite sprite;
	Vector2f position;
	float radius;
};

