#pragma once

#include <SFML\Graphics.hpp>
#include "Math.h"
#include "Types.h"

using namespace sf;

class GameObject
{
public:
	virtual void update(Time dt) = 0;
	virtual void render(RenderWindow* handle, Vector2f scale) = 0;
	virtual void processInput(Keyboard::Key, bool isPressed) = 0;
	virtual void processInput(Vector2i pos, bool isPressed) = 0;
};