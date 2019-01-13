#pragma once
#include "Entity.h"
class Lightning : public Entity
{
public:
	Lightning(Types::Entities thisType, Texture* texture, GameScene* thisScene, Vector2f source, Vector2f target);
	~Lightning();

	void update(Time dt);
	void render(RenderWindow* handle, Vector2f scale);
	void processInput(Keyboard::Key, bool isPressed);
	void processInput(Vector2i pos, bool isPressed);

private:
	Vector2f source;
	Vector2f target;
	float lifeSpan = 0.15f;
	RectangleShape line;
};

