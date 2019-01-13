#pragma once

#include <SFML\Graphics.hpp>

#include "TextureManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "ButtonObject.h"
#include "MessagePanel.h"

using namespace sf;

class Menu : public Scene
{
public:
	Menu(TextureManager* manager);
	~Menu();

public:
	void update(Time dt);
	void render(RenderWindow* handle, Vector2f scale);
	void processInput(Keyboard::Key key, bool isPressed);
	void processInput(Vector2i pos, bool isPressed);
	void deleteObjects();

	void addButton(ButtonObject* button);
	void addImage(Textures::ID ID, Vector2f pos);
	void addImage(Textures::ID ID, Vector2f pos, Color color);
	void addPanel(MessagePanel* panel);
	void setText(string text, Vector2f pos, Fonts::ID fontID, int fontSize, Color color);

	void addPanelToRemove(MessagePanel* panel);

	vector<ButtonObject*> buttons;
	vector<MessagePanel*> panels;

private:
	vector<Sprite> sprites;
	vector<Text> texts;

	Sprite background;

	TextureManager* textureManager;

	vector<MessagePanel*> panelRemoveList;
	// should have different class for in game stuff
	// for collisions and stuff to be easier to handle
};