#pragma once
#include <SFML\Graphics.hpp>

#include "TextureManager.h"
#include "ConfirmButton.h"

class Scene;
class ButtonObject;

class MessagePanel
{
public:
	static const int OK = 0;
	static const int CONFIRMBUTTON = 1;
	static const int CONFIRMACTION = 2;
	static const int EASYMEDHARD = 3;
	static const int CANCEL = 4;
	static const int PAUSE = 5;

	MessagePanel(int thisType, Scene* thisScene, TextureManager* manager, Vector2f topLeftCoord);
	MessagePanel(int thisType, ButtonObject* thisButton, Scene* thisScene, TextureManager* manager, Vector2f topLeftCoord);

	void update(Time dt);
	void render(RenderWindow* handle, Vector2f scale);
	void processInput(Keyboard::Key, bool isPressed);
	void processInput(Vector2i pos, bool isPressed);
	void doAction(Types::Confirm confirm);
	void setMessage(string text);
	void skipLine(int lineSize);
	void addToRemoveList();

	~MessagePanel();

private:
	// Adjust coordinates of everything relative to window based on location of panel
	void adjustPosition();
	void initializeButtons();
	int type;
	Scene* scene;
	TextureManager* textureManager;
	Vector2f topLeft;
	ButtonObject* button;
	Sprite backgroundImage;

	// Use Confirm Button
	vector<ConfirmButton*> buttons;

	// Default locations for the buttons
	Vector2f OKPos = Vector2f(300, 410);
	Vector2f YesPos = Vector2f(240, 410);
	Vector2f NoPos = Vector2f(380, 410);
	Vector2f buttonSize = Vector2f(100, 50);

	Vector2f EasyPos = Vector2f(63, 350);
	Vector2f MedPos = Vector2f(213, 350);
	Vector2f HardPos = Vector2f(363, 350);
	Vector2f ImpossiblePos = Vector2f(513, 350);
	Vector2f CancelPos = Vector2f(213, 410);
	Vector2f HighScoresPos = Vector2f(363, 410);
	Vector2f buttonSize2 = Vector2f(140, 50);
	


	// Properties of text on the panel
	Vector2f size = Vector2f(560, 400);
	const int fontSize = 25;
	const Fonts::ID fontID = Fonts::Calibri;
	vector<Text> textLines;
};

