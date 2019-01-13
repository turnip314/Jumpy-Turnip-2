#pragma once

#include "MenuButton.h"
#include "Game.h"
#include "GameScene.h"

using namespace std;
using namespace sf;

MenuButton::MenuButton(Types::Buttons buttonAction, TextureManager* manager, Game* thisGame, Vector2f topLeftCoord, Vector2f bottomRightCoord) :
	ButtonObject(thisGame, manager, topLeftCoord, bottomRightCoord)
{
	action = buttonAction;

	if (buttonAction == Types::Buttons::AdventureButton ||
		buttonAction == Types::Buttons::UpgradeMenuButton ||
		buttonAction == Types::Buttons::TeamButton || 
		buttonAction == Types::Buttons::CreditButton)
	{
		mainButton.setTexture(*textureManager->getTexture(Textures::menuButton));
		scaledButton.setTexture(*textureManager->getTexture(Textures::menuButtonS));
	}
	else if (buttonAction == Types::Buttons::BackButton ||
		buttonAction == Types::Buttons::NextLevelsButton ||
		buttonAction == Types::Buttons::PrevLevelsButton ||
		buttonAction == Types::Buttons::NextUpgradesButton ||
		buttonAction == Types::Buttons::PrevUpgradesButton ||
		buttonAction == Types::Buttons::PauseButton)
	{
		mainButton.setTexture(*textureManager->getTexture(Textures::BackButton));
		scaledButton.setTexture(*textureManager->getTexture(Textures::BackButtonS));
	}
}

MenuButton::~MenuButton()
{
	// Destroy all buttons objects here

}

void MenuButton::processInput(Keyboard::Key key, bool isPressed)
{
	if (isPressed && key == Keyboard::Escape)
	{
		if (action == Types::Buttons::PauseButton)
		{
			doButtonAction();
		}
	}
	ButtonObject::processInput(key, isPressed);
}

void MenuButton::processInput(Vector2i pos, bool isPressed)
{
	if (isPressed && (pos.x < 0 || pos.x > 1280 || pos.y < 0 || pos.y > 720))
	{
		if (action == Types::Buttons::PauseButton)
		{
			doButtonAction();
		}
	}
	ButtonObject::processInput(pos, isPressed);
}

void MenuButton::doButtonAction()
{
	if (action == Types::Buttons::AdventureButton)
	{
		game->openLevelSelection();
	}
	else if (action == Types::Buttons::UpgradeMenuButton)
	{
		game->openUpgradeMenu();
	}
	else if (action == Types::Buttons::TeamButton)
	{
		game->openTeamMenu();
	}
	else if (action == Types::Buttons::CreditButton)
	{
		game->openCreditMenu();
	}
	else if (action == Types::Buttons::BackButton)
	{
		game->closeScene();
	}
	else if (action == Types::Buttons::PrevUpgradesButton)
	{
		game->prevUpgradePage();
	}
	else if (action == Types::Buttons::NextUpgradesButton)
	{
		game->nextUpgradePage();
	}
	else if (action == Types::Buttons::PrevLevelsButton)
	{
		game->prevLevelPage();
	}
	else if (action == Types::Buttons::NextLevelsButton)
	{
		game->nextLevelPage();
	}
	else if (action == Types::Buttons::PauseButton)
	{
		MessagePanel* pausePanel = new MessagePanel(MessagePanel::PAUSE, this, game->getCurScene(), textureManager, Vector2f(300, 100));
		pausePanel->setMessage("The game is paused. What would you like to do?");
		game->getCurScene()->addPanel(pausePanel);
	}
}

void MenuButton::confirmButtonAction(Types::Confirm confirm)
{
	// Pause button can only exist for game scene
	if (action == Types::Buttons::PauseButton)
	{
		if (confirm == Types::Confirm::Leave)
		{
			MessagePanel* exitPanel = new MessagePanel(MessagePanel::CONFIRMBUTTON, this, game->getCurScene(), textureManager, Vector2f(300, 100));
			exitPanel->setMessage("Are you sure you want to leave?");
			game->getCurScene()->addPanel(exitPanel);
		}
		else if (confirm == Types::Confirm::Yes)
		{
			GameScene* gameScene = static_cast<GameScene*>(game->getCurScene());
			gameScene->endGame();
		}
	}
}



