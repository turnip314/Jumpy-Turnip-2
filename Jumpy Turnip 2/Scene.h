#pragma once

#include <SFML\Graphics.hpp>
#include "Math.h"
#include "Types.h"
#include <iostream>
#include "MessagePanel.h"

using namespace sf;
using namespace std;

class Scene
{
public:
	virtual ~Scene() = 0 { };

	virtual void update(Time dt) = 0;
	virtual void render(RenderWindow* handle, Vector2f scale) = 0;
	virtual void processInput(Keyboard::Key, bool isPressed) = 0;
	virtual void processInput(Vector2i pos, bool isPressed) = 0;
	virtual void deleteObjects() = 0;

	virtual void addPanel(MessagePanel* panel) = 0;
	virtual void addPanelToRemove(MessagePanel* panel) = 0;
};