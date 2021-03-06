#include "UpgradeButton.h"
#include "MessagePanel.h"
#include "Game.h"


UpgradeButton::UpgradeButton(Types::Players thisPlayerType, int ID, Game* thisGame, TextureManager* manager, Vector2f topLeftCoord, Vector2f bottomRightCoord) :
	ButtonObject(thisGame, manager, topLeftCoord, bottomRightCoord)
{
	playerType = thisPlayerType;
	upgradeID = ID;
}


UpgradeButton::~UpgradeButton()
{
}

void UpgradeButton::doButtonAction()
{
	// If upgrade button is clicked, do its corresponding action
	// If unlocked is locked, will tell player to unlock it
	// If unlocked, will give player option to buy (whether they can afford it is checked in confirm)
	// If bought, will describe upgrade and say it's already bought
	if (upgradeStatus == locked)
	{
		MessagePanel* confirmPanel = new MessagePanel(MessagePanel::OK, this, game->getCurScene(), textureManager, Vector2f(350, 95));
		confirmPanel->setMessage("You must buy previous upgrades or unlock this turnip to buy this upgrade.");
		game->getCurScene()->addPanel(confirmPanel);
	}
	else if (upgradeStatus == unlocked)
	{
		MessagePanel* confirmPanel = new MessagePanel(MessagePanel::CONFIRMBUTTON, this, game->getCurScene(), textureManager, Vector2f(350, 95));
		confirmPanel->setMessage(upgradeName);
		confirmPanel->skipLine(10);
		confirmPanel->setMessage(confirmText);
		confirmPanel->skipLine(10);
		confirmPanel->setMessage("Buy for $" + to_string(upgradeCost) + "? You currently have $" + to_string(game->getMoney()) + ".");
		game->getCurScene()->addPanel(confirmPanel);
	}
	else if (upgradeStatus == bought)
	{
		MessagePanel* confirmPanel = new MessagePanel(MessagePanel::OK, this, game->getCurScene(), textureManager, Vector2f(350, 95));
		confirmPanel->setMessage(upgradeName);
		confirmPanel->skipLine(10);
		confirmPanel->setMessage(confirmText);
		confirmPanel->skipLine(10);
		confirmPanel->setMessage("You already bought this upgrade");
		game->getCurScene()->addPanel(confirmPanel);
	}
}

void UpgradeButton::confirmButtonAction(Types::Confirm confirm)
{
	// Confirming to buy an upgrade
	// Will check if player can afford it here, and upgrade it if yes
	// Otherwise, give a cannot afford warning
	if (confirm == Types::Confirm::Yes)
	{
		if (game->spendMoney(upgradeCost))
		{
			upgradeStatus = bought;
			game->updateUpgrade(playerType, upgradeID, bought);
		}
		else
		{
			MessagePanel* confirmPanel = new MessagePanel(MessagePanel::OK, this, game->getCurScene(), textureManager, Vector2f(350, 95));
			confirmPanel->setMessage("You cannot afford this");
			game->getCurScene()->addPanel(confirmPanel);
		}
	}
}

void UpgradeButton::setUpgradeStatus(int newStatus, int cost)
{
	// Tells the upgrade button if its corresponding upgrade is unlocked/bought/etc and 
	// its upgrade cost, called once upon initialization
	upgradeStatus = newStatus;
	upgradeCost = cost;

	if (upgradeStatus == locked)
	{
		mainButton.setTexture(*textureManager->getTexture(Textures::UpgradeLockedButton));
		scaledButton.setTexture(*textureManager->getTexture(Textures::UpgradeLockedButtonS));
	}
	else if (upgradeStatus == unlocked)
	{
		mainButton.setTexture(*textureManager->getTexture(Textures::UpgradeUnlockedButton));
		scaledButton.setTexture(*textureManager->getTexture(Textures::UpgradeUnlockedButtonS));
	}
	else if (upgradeStatus == bought)
	{
		mainButton.setTexture(*textureManager->getTexture(Textures::UpgradeBoughtButton));
		scaledButton.setTexture(*textureManager->getTexture(Textures::UpgradeBoughtButtonS));
	}
}

void UpgradeButton::setText(Fonts::ID fontID, int fontSize, string text, string confirm)
{
	// Sets text of button, like button object, but slightly different style (has confirm text)
	ButtonObject::setText(fontID, fontSize, text);
	for (unsigned i = 0; i < textLines.size(); i++)
	{
		textLines[i].setFillColor(Color(0, 0, 0, 192));
	}
	for (unsigned i = 0; i < scaledTextLines.size(); i++)
	{
		scaledTextLines[i].setFillColor(Color(0, 0, 0, 192));
	}
	upgradeName = text;
	confirmText = confirm;
}
