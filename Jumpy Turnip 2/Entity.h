#pragma once
#include "GameObject.h"
#include "TextureManager.h"

class GameScene;
class Entity : public GameObject
{
public:
	Entity(Types::Entities thisType, Texture* texture, GameScene* thisScene);
	~Entity();

	virtual void update(Time dt) = 0;
	virtual void render(RenderWindow* handle, Vector2f scale) = 0;
	virtual void processInput(Keyboard::Key, bool isPressed) = 0;
	virtual void processInput(Vector2i pos, bool isPressed) = 0;
	virtual void addToRemoveList();

protected:
	Types::Entities type;
	Texture* texture;
	GameScene* scene;
};

