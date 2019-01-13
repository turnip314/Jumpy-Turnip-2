#include "PlayerButton.h"
#include "Game.h"
#include "MessagePanel.h"



PlayerButton::PlayerButton(Game* thisGame, TextureManager* manager, Vector2f topLeftCoord, Vector2f bottomRightCoord):
	ButtonObject(thisGame, manager, topLeftCoord, bottomRightCoord)
{

}


PlayerButton::~PlayerButton()
{
}

void PlayerButton::render(RenderWindow* handle, Vector2f scale)
{
	ButtonObject::render(handle, scale);

	if (hover || (game->getCurSelection() == playerType && playerType != Types::Players::PlayersEND))
	{
		handle->draw(scaledSprite);
	}
	else
	{
		handle->draw(sprite);
	}

	if ((game->getCurSelection() == Types::Players::PlayersEND && hover) ||
		((!(game->getCurSelection() == Types::Players::PlayersEND) && game->getCurSelection() == playerType)))
	{
		handle->draw(nameText);
	}
}

void PlayerButton::doButtonAction()
{
	if (buttonType == Types::Buttons::PlayerButton)
	{
		if (teamStatus == locked)
		{
			// option to unlock
			MessagePanel* confirmPanel = new MessagePanel(MessagePanel::CONFIRMBUTTON, this,
				game->getCurScene(), textureManager, Vector2f(100, 100));
			confirmPanel->setMessage("Would you like to unlock this turnip for $500? You currently have $" + to_string(game->getMoney()) + ".");
			game->getCurScene()->addPanel(confirmPanel);
		}
			
		// Change game selected, unselect other options
		game->setCurSelection(playerType);
	}
	else if (buttonType == Types::Buttons::TeamSlotButton)
	{
		// Might be option to unlock new slot
		if (playerType == Types::Players::PlayersEND)
		{
			// Buy new slot if enough money
			MessagePanel* confirmPanel = new MessagePanel(MessagePanel::CONFIRMBUTTON, this,
				game->getCurScene(), textureManager, Vector2f(100, 100));
			confirmPanel->setMessage("Would you like to unlock this slot for $" +
				to_string((int)(10 * ((int)(pow((game->getTeamSize() + 2), 5) / 10)))) + "?" +
				" You currently have $" + to_string(game->getMoney()) + ".");
			game->getCurScene()->addPanel(confirmPanel);
		}
		else
		{
			Types::Players newPlayerType = game->getCurSelection();

			// Case that nothing is selected, make this button the new selection
			if (newPlayerType == Types::Players::PlayersEND)
			{
				game->setCurSelection(playerType);
			}

			// Switch teams if selected
			game->swapTeamPosition(newPlayerType, playerType);
		}
	}
}

void PlayerButton::confirmButtonAction(Types::Confirm confirm)
{
	if (buttonType == Types::Buttons::PlayerButton)
	{
		if (confirm == Types::Confirm::Yes)
		{
			game->buyNewPlayer(playerType);
		}
		else
		{
			game->setCurSelection(Types::Players::PlayersEND);
			// do nothing
		}
	}
	else
	{
		if (confirm == Types::Confirm::Yes)
		{
			game->buyNewSlot();
		}
		else
		{
			game->setCurSelection(Types::Players::PlayersEND);
			// do nothing
		}
	}
	
}

void PlayerButton::initialize(Types::Buttons thisButtonType, Types::Players thisPlayerType, int thisTeamStatus)
{
	buttonType = thisButtonType;
	playerType = thisPlayerType;
	teamStatus = thisTeamStatus;

	Texture* mainTexture;
	Texture* scaledTexture;

	// Main button texture
	if (thisButtonType == Types::Buttons::PlayerButton)
	{
		if (thisTeamStatus != locked)
		{
			mainTexture = textureManager->getTexture(Textures::UnlockPlayerButton);
			scaledTexture = textureManager->getTexture(Textures::UnlockPlayerButtonS);
		}
		else
		{
			mainTexture = textureManager->getTexture(Textures::LockPlayerButton);
			scaledTexture = textureManager->getTexture(Textures::LockPlayerButtonS);
		}
	}
	else
	{
		if (thisTeamStatus != locked)
		{
			mainTexture = textureManager->getTexture(Textures::UnlockSlotButton);
			scaledTexture = textureManager->getTexture(Textures::UnlockSlotButtonS);
		}
		else
		{
			mainTexture = textureManager->getTexture(Textures::LockSlotButton);
			scaledTexture = textureManager->getTexture(Textures::LockSlotButtonS);
		}
	}

	mainButton.setTexture(*mainTexture);
	scaledButton.setTexture(*scaledTexture);
	

	// Makes the turnip sprite to be displayed on button
	Types::Sizes mainSize;
	Types::Sizes scaledSize;

	if (thisButtonType == Types::Buttons::PlayerButton)
	{
		mainSize = Types::Sizes::Big;
		scaledSize = Types::Sizes::BigScaled;
	}
	else
	{
		mainSize = Types::Sizes::Medium;
		scaledSize = Types::Sizes::MediumScaled;
	}


	sprite.setTexture(*textureManager->getTexture(game->getPlayerTextureID(thisPlayerType, mainSize)));
	sprite.setPosition(topLeft);

	scaledSprite.setTexture(*textureManager->getTexture(game->getPlayerTextureID(thisPlayerType, scaledSize)));
	scaledSprite.setPosition(Vector2f(topLeft.x - size.x * 0.05, topLeft.y - size.y * 0.05));
	
	nameText.setString(game->getPlayerName(thisPlayerType));
	nameText.setFont(*textureManager->getFont(Fonts::Calibri));
	nameText.setPosition(Vector2f(600, 440));
	nameText.setFillColor(Color::White);
	nameText.setCharacterSize(25);
}

Types::Buttons PlayerButton::getType()
{
	return buttonType;
}