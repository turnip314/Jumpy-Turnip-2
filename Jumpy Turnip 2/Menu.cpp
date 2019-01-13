#include <SFML\Graphics.hpp>

#include "Menu.h"
#include "Player.h"
#include "Textures.h"
#include "MenuButton.h"
#include "Game.h"

using namespace sf;

Menu::Menu(TextureManager* manager)
{
	textureManager = manager;
	background.setTexture(*textureManager->getTexture(Textures::Background2));
}

Menu::~Menu()
{
	while (buttons.size() > 0)
	{
		// ADD destructor of object
		delete buttons.at(0);
		buttons.erase(buttons.begin());
	}
	while (panels.size() > 0)
	{
		// ADD destructor of object
		delete panels.at(0);
		panels.erase(panels.begin());
	}
}

void Menu::update(Time dt)
{
	if (panels.size() > 0)
	{
		for (unsigned i = 0; i < panels.size(); i++)
		{
			panels[i]->update(dt);
		}
		return;
	}

	for (unsigned i = 0; i < buttons.size(); i++)
	{
		buttons[i]->update(dt);
	}
}

void Menu::render(RenderWindow* handle, Vector2f scale)
{
	handle->draw(background);

	for (unsigned i = 0; i < buttons.size(); i++)
	{
		buttons[i]->render(handle, scale);
	}
	for (unsigned i = 0; i < sprites.size(); i++)
	{
		handle->draw(sprites[i]);
	}
	for (unsigned i = 0; i < panels.size(); i++)
	{
		panels[i]->render(handle, scale);
	}
	for (unsigned i = 0; i < texts.size(); i++)
	{
		handle->draw(texts[i]);
	}
}

void Menu::processInput(Keyboard::Key key, bool isPressed)
{
	if (panels.size() > 0)
	{
		for (unsigned i = 0; i < panels.size(); i++)
		{
			panels[i]->processInput(key, isPressed);
		}
		return;
	}

	for (unsigned i = 0; i < buttons.size(); i++) {
		buttons[i]->processInput(key, isPressed);
	}
}

void Menu::processInput(Vector2i pos, bool isPressed)
{
	if (panels.size() > 0)
	{
		panels[panels.size() - 1]->processInput(pos, isPressed);
		return;
	}

	for (unsigned i = 0; i < buttons.size(); i++) {
		buttons[i]->processInput(pos, isPressed);
	}
}

void Menu::deleteObjects()
{
	// Deletes message panels, button only deleted with destructor
	while (panelRemoveList.size() > 0)
	{
		auto it = find(panels.begin(), panels.end(), panelRemoveList.at(0));
		if (it != panels.end())
		{
			panels.erase(it);
			delete panelRemoveList.at(0);
			panelRemoveList.erase(panelRemoveList.begin());
		}
		else
		{
			throw runtime_error("Panel does not exist");
		}
	}
}

void Menu::addButton(ButtonObject* button)
{
	buttons.push_back(button);
}

void Menu::addImage(Textures::ID ID, Vector2f pos)
{
	Sprite newSprite;
	newSprite.setTexture(*textureManager->getTexture(ID));
	newSprite.setPosition(pos);
	sprites.push_back(newSprite);
}

void Menu::addImage(Textures::ID ID, Vector2f pos, Color color)
{
	Sprite newSprite;
	newSprite.setTexture(*textureManager->getTexture(ID));
	newSprite.setPosition(pos);
	newSprite.setColor(color);
	sprites.push_back(newSprite);
}

void Menu::addPanel(MessagePanel* panel)
{
	panels.push_back(panel);
}

void Menu::addPanelToRemove(MessagePanel* panel)
{
	panelRemoveList.push_back(panel);
}

void Menu::setText(string text, Vector2f pos, Fonts::ID fontID, int fontSize, Color color)
{
	Text line;
	line.setString(text);
	line.setPosition(pos);
	line.setFont(*textureManager->getFont(fontID));
	line.setCharacterSize(fontSize);
	line.setFillColor(color);
	texts.push_back(line);
}