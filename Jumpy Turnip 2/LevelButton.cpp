#include "LevelButton.h"
#include "Game.h"


LevelButton::LevelButton(int thisLevel, LevelScores thisStat, Game* thisGame, TextureManager* manager, Vector2f topLeftCoord, Vector2f bottomRightCoord) :
	ButtonObject(thisGame, manager, topLeftCoord, bottomRightCoord)
{
	levelStat = thisStat;
	level = thisLevel;

	// Sets button texture
	if (level < game->freeplayNoLoopLevel)
	{
		if (thisStat.levelStatus == 0)
		{
			mainButton.setTexture(*textureManager->getTexture(Textures::levelLockedButton));
			scaledButton.setTexture(*textureManager->getTexture(Textures::levelLockedButtonS));
		}
		else if (thisStat.levelStatus == 5)
		{
			mainButton.setTexture(*textureManager->getTexture(Textures::levelBeatenButton));
			scaledButton.setTexture(*textureManager->getTexture(Textures::levelBeatenButtonS));
		}
		else
		{
			mainButton.setTexture(*textureManager->getTexture(Textures::levelAvailableButton));
			scaledButton.setTexture(*textureManager->getTexture(Textures::levelAvailableButtonS));
		}

		setText(Fonts::Atarian, 100, "_" + to_string(thisLevel + 1));
		centerText();
	}
	else
	{
		if (thisStat.levelStatus == 0)
		{
			mainButton.setTexture(*textureManager->getTexture(Textures::freeplayLockedButton));
			scaledButton.setTexture(*textureManager->getTexture(Textures::freeplayLockedButtonS));
		}
		else
		{
			mainButton.setTexture(*textureManager->getTexture(Textures::freeplayAvailableButton));
			scaledButton.setTexture(*textureManager->getTexture(Textures::freeplayAvailableButtonS));
		}
		if (thisLevel == game->freeplayNoLoopLevel)
		{
			setText(Fonts::Atarian, 50, "31");
			setText(Fonts::Atarian, 50, "Freeplay mode");
			setText(Fonts::Atarian, 50, "No Loop");
			centerText();
		}
		else if (thisLevel == game->freeplayLoopLevel)
		{
			setText(Fonts::Atarian, 50, "32");
			setText(Fonts::Atarian, 50, "Freeplay mode");
			setText(Fonts::Atarian, 50, "Loop");
			centerText();
		}
		else if (thisLevel == game->freeplayTimeNoLoopLevel)
		{
			setText(Fonts::Atarian, 50, "33");
			setText(Fonts::Atarian, 50, "Timed mode");
			setText(Fonts::Atarian, 50, "No Loop");
			centerText();
		}
		else if (thisLevel == game->freeplayTimeLoopLevel)
		{
			setText(Fonts::Atarian, 50, "34");
			setText(Fonts::Atarian, 50, "Timed mode");
			setText(Fonts::Atarian, 50, "Loop");
			centerText();
		}
	}
}


LevelButton::~LevelButton()
{
}

void LevelButton::render(RenderWindow* handle, Vector2f scale)
{
	ButtonObject::render(handle, scale);

	if (level < game->freeplayNoLoopLevel)
	{
		int circleRadius = 15;
		if (levelStat.levelStatus > 1)
		{
			CircleShape shape(circleRadius);
			shape.setFillColor(Color(201, 174, 93));
			shape.setPosition(Vector2f(topLeft.x + size.x / 4 - circleRadius, bottomRight.y + circleRadius));
			handle->draw(shape);
		}
		if (levelStat.levelStatus > 2)
		{
			CircleShape shape(circleRadius);
			shape.setFillColor(Color(192, 192, 192));
			shape.setPosition(Vector2f(topLeft.x + 2 * size.x / 4 - circleRadius, bottomRight.y + circleRadius));
			handle->draw(shape);
		}
		if (levelStat.levelStatus > 3)
		{
			CircleShape shape(circleRadius);
			shape.setFillColor(Color(255, 215, 0));
			shape.setPosition(Vector2f(topLeft.x + 3 * size.x / 4 - circleRadius, bottomRight.y + circleRadius));
			handle->draw(shape);
		}
	}
}

void LevelButton::doButtonAction()
{
	if (levelStat.levelStatus == 0)
	{
		// locked
		MessagePanel* confirmPanel = new MessagePanel(MessagePanel::OK, this, game->getCurScene(),
			textureManager, Vector2f(270, 100));
		confirmPanel->setMessage("You haven't unlocked this level yet. Beat earlier levels first to unlock this level.");
		game->getCurScene()->addPanel(confirmPanel);
	}
	else
	{
		MessagePanel* levelPanel = new MessagePanel(MessagePanel::EASYMEDHARD, this, game->getCurScene(),
			textureManager, Vector2f(270, 100));
		levelPanel->setMessage("Select a difficulty");
		game->getCurScene()->addPanel(levelPanel);
	}
}

void LevelButton::confirmButtonAction(Types::Confirm confirm)
{
	if (confirm == Types::Confirm::Easy)
	{
		game->startNewGame(level, 1);
	}
	else if (confirm == Types::Confirm::Normal)
	{
		if (levelStat.levelStatus <= 1)
		{
			//haven't unlocked
			MessagePanel* confirmPanel = new MessagePanel(MessagePanel::OK, this, game->getCurScene(),
				textureManager, Vector2f(270, 100));
			confirmPanel->setMessage("You haven't unlocked this difficulty yet. Beat easier difficulties first to unlock this difficulty.");
			game->getCurScene()->addPanel(confirmPanel);
		}
		else
		{
			game->startNewGame(level, 2);
		}
	}
	else if (confirm == Types::Confirm::Hard)
	{
		if (levelStat.levelStatus <= 2)
		{
			//haven't unlocked
			MessagePanel* confirmPanel = new MessagePanel(MessagePanel::OK, this, game->getCurScene(),
				textureManager, Vector2f(270, 100));
			confirmPanel->setMessage("You haven't unlocked this difficulty yet. Beat easier difficulties first to unlock this difficulty.");
			game->getCurScene()->addPanel(confirmPanel);
		}
		else
		{
			game->startNewGame(level, 3);
		}
	}
	else if (confirm == Types::Confirm::Impossible)
	{
		if (levelStat.levelStatus <= 3)
		{
			//haven't unlocked
			MessagePanel* confirmPanel = new MessagePanel(MessagePanel::OK, this, game->getCurScene(),
				textureManager, Vector2f(270, 100));
			confirmPanel->setMessage("You haven't unlocked this difficulty yet. Beat easier difficulties first to unlock this difficulty.");
			game->getCurScene()->addPanel(confirmPanel);
		}
		else
		{
			game->startNewGame(level, 4);
		}
	}
	else if (confirm == Types::Confirm::HighScores)
	{
		MessagePanel* highScoresPanel = new MessagePanel(MessagePanel::OK, this, game->getCurScene(),
			textureManager, Vector2f(270, 100));
		highScoresPanel->setMessage("Easy: " + to_string(levelStat.easyScore) + " points");
		highScoresPanel->setMessage("Medium: " + to_string(levelStat.mediumScore) + " points");
		highScoresPanel->setMessage("Hard: " + to_string(levelStat.hardScore) + " points");
		highScoresPanel->setMessage("Impossible: " + to_string(levelStat.impossibleScore) + " points");
		game->getCurScene()->addPanel(highScoresPanel);
	}
}