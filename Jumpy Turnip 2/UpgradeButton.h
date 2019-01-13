#pragma once
#include "ButtonObject.h"

class UpgradeButton : public ButtonObject
{
public:
	UpgradeButton(Types::Players thisPlayerType, int ID, Game* thisGame, TextureManager* manager, Vector2f topLeftCoord, Vector2f bottomRightCoord);

	~UpgradeButton();

	void doButtonAction();
	void confirmButtonAction(Types::Confirm confirm);
	void setUpgradeStatus(int newStatus, int cost);
	void setText(Fonts::ID fontID, int fontSize, string text, string confirm);

private:
	Types::Players playerType;
	int upgradeID;
	int upgradeStatus;
	int upgradeCost;
	const int locked = 0;
	const int unlocked = 1;
	const int bought = 2;

	string upgradeName;
	string confirmText;
};

