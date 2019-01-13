#pragma once
#include "ButtonObject.h"

class PlayerButton : public ButtonObject
{
public:
	PlayerButton(Game* thisGame, TextureManager* manager, Vector2f topLeftCoord, Vector2f bottomRightCoord);
	~PlayerButton();
	const int locked = -1;
	const int notInTeam = 0;

	void render(RenderWindow* handle, Vector2f scale);
	void doButtonAction();
	void confirmButtonAction(Types::Confirm confirm);
	
	void initialize(Types::Buttons thisButtonType, Types::Players thisPlayerType, int thisTeamStatus);
	Types::Buttons getType();

private:
	Types::Buttons buttonType;
	Types::Players playerType;
	int teamStatus;

	Sprite sprite;
	Sprite scaledSprite;

	Text nameText;
};

