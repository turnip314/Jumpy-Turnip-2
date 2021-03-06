#pragma once

#include <SFML\Graphics.hpp>
#include "Math.h"
#include "Types.h"
#include "TextureManager.h"

class Game;
class ButtonObject
{
public:
	ButtonObject(Game* thisGame, TextureManager* manager, Vector2f topLeftCoord, Vector2f bottomRightCoord);
	~ButtonObject();

	virtual void update(Time dt);
	virtual void render(RenderWindow* handle, Vector2f scale);
	virtual void processInput(Keyboard::Key, bool isPressed);
	virtual void processInput(Vector2i pos, bool isPressed);

	// Called when button is needed
	virtual void doButtonAction();

	// Called in case a confirmation is needed from message panel
	virtual void confirmButtonAction(Types::Confirm confirm);

	// Sets text on the button
	virtual void setText(Fonts::ID fontID, int fontSize, string text);

	// Centers text horizontally
	virtual void centerText();

protected:
	Vector2f topLeft;
	Vector2f bottomRight;
	Vector2f size;
	Game* game;
	TextureManager* textureManager;

	// Hover means mouse is over button area, not necessarily clicked
	bool isHovered(Vector2i hoverPos);
	bool hover;
	//bool isClicked(Vector2f clickPos);

	// Usually 0 is no, 1 is yes
	// Sometimes 0 is back, 1 is easy, 2 is medium, 3 is hard
	int confirm = 0;

	// Display text
	vector<Text> textLines;
	// For when button is hovered
	vector<Text> scaledTextLines;

	// Stores a main sprite and sprite to be displayed if mouse hovers over it
	Sprite mainButton;
	Sprite scaledButton;
	

};

