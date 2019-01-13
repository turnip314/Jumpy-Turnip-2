#pragma once
#include <SFML\Graphics.hpp>

#include "ButtonObject.h"

using namespace sf;
using namespace std;

class MenuButton : public ButtonObject
{
public:

	MenuButton(Types::Buttons buttonAction, TextureManager* manager, Game* thisGame, Vector2f topLeftCoord, Vector2f bottomRightCoord);
	~MenuButton();

	void doButtonAction();
	void confirmButtonAction(Types::Confirm confirm);

	void processInput(Keyboard::Key, bool isPressed);
	void processInput(Vector2i pos, bool isPressed);

private:

	int action;
};

