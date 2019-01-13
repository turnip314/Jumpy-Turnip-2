#pragma once
#include "Entity.h"
class Healthpack : public Entity
{
public:
	Healthpack(Types::Entities thisType, Texture* thisTexture, GameScene* thisScene);
	~Healthpack();

	void update(Time dt);
	void render(RenderWindow* handle, Vector2f scale);
	void processInput(Keyboard::Key, bool isPressed);
	void processInput(Vector2i pos, bool isPressed);
	void setPosition(Vector2f pos);

private:
	float lifespan = 2.f;
	Sprite sprite;
	Vector2f position;
};

