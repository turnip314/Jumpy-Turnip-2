#include "ConfirmButton.h"
#include "MessagePanel.h"
#include <iostream>

ConfirmButton::ConfirmButton(MessagePanel* panel, Types::Confirm thisConfirm, TextureManager* manager, Vector2f topLeftCoord, Vector2f bottomRightCoord) :
	ButtonObject(NULL, manager, topLeftCoord, bottomRightCoord)
{
	confirm = thisConfirm;
	messagePanel = panel;

	if (thisConfirm == Types::Confirm::Easy ||
		thisConfirm == Types::Confirm::Normal ||
		thisConfirm == Types::Confirm::Hard ||
		thisConfirm == Types::Confirm::Impossible ||
		thisConfirm == Types::Confirm::CANCEL ||
		thisConfirm == Types::Confirm::HighScores)
	{
		mainButton.setTexture(*textureManager->getTexture(Textures::confirmButton2));
		scaledButton.setTexture(*textureManager->getTexture(Textures::confirmButton2S));
	}
	else
	{
		mainButton.setTexture(*textureManager->getTexture(Textures::confirmButton1));
		scaledButton.setTexture(*textureManager->getTexture(Textures::confirmButton1S));
	}
}


ConfirmButton::~ConfirmButton()
{
}

void ConfirmButton::doButtonAction()
{
	messagePanel->doAction(confirm);
}
