#pragma once

#include "ButtonObject.h"
#include "Game.h"

using namespace std;
using namespace sf;

ButtonObject::ButtonObject(Game* thisGame, TextureManager* manager, Vector2f topLeftCoord, Vector2f bottomRightCoord)
{
	topLeft = topLeftCoord;
	bottomRight = bottomRightCoord;
	size.x = bottomRight.x - topLeft.x;
	size.y = bottomRight.y - topLeft.y;
	game = thisGame;
	textureManager = manager;
	// only temporary, make this depend on input later
	//functionPtr = funcMap[MenuFunction::FuncName::NewGame];

	mainButton.setPosition(topLeft);
	scaledButton.setPosition(Vector2f(topLeft.x - size.x * 0.05f, topLeft.y - size.y * 0.05f));

}

ButtonObject::~ButtonObject()
{
}

void ButtonObject::update(Time dt)
{

}

void ButtonObject::render(RenderWindow* handle, Vector2f scale)
{
	// Draws a larger button image if the mouse is hovering over it
	if (mainButton.getTexture() != nullptr && scaledButton.getTexture() != nullptr)
	{
		if (hover)
		{
			handle->draw(scaledButton);
		}
		else
		{
			handle->draw(mainButton);
		}
	}
	// If the button doesn't have a texture, use a rectangle as placeholder
	else
	{
		Vector2f topLeftCorner = topLeft;
		Vector2f displaySize = size;
		if (hover)
		{
			topLeftCorner.x -= size.x * 0.05;
			topLeftCorner.y -= size.y * 0.05;
			displaySize.x *= 1.1;
			displaySize.y *= 1.1;
		}
		RectangleShape rectangle(displaySize);
		rectangle.setPosition(topLeftCorner);
		rectangle.setFillColor(Color::Green);
		handle->draw(rectangle);
	}


	// Draws text line by line

	if (hover)
	{
		for (Text text : scaledTextLines)
		{
			handle->draw(text);
		}
	}
	else
	{
		for (Text text : textLines)
		{
			handle->draw(text);
		}
	}

}

void ButtonObject::processInput(Keyboard::Key key, bool isPressed)
{

}

void ButtonObject::processInput(Vector2i position, bool isPressed)
{
	// If mouse if clicked while over button
	hover = isHovered(Vector2i(position.x, position.y));
	if (isPressed && hover)
	{
		doButtonAction();
	}
}

bool ButtonObject::isHovered(Vector2i hoverPos)
{
	// Checks if mouse is hovering over button
	return (topLeft.x < hoverPos.x && hoverPos.x < bottomRight.x &&
		topLeft.y < hoverPos.y && hoverPos.y < bottomRight.y);
}

void ButtonObject::doButtonAction()
{
	// Overwritten Function
	// Do nothing
}

void ButtonObject::confirmButtonAction(Types::Confirm confirm)
{

}

void ButtonObject::setText(Fonts::ID fontID, int fontSize, string text)
{
	// Stores the style and location of each line of text in a vector to be drawn
	// when the render method is called
	// scaledTextLines is the scaled version for when the mouse is hovering over the button
	vector<string> lines = TextBox::getTextLines(Vector2f(size.x*0.95, size.y*0.95), textureManager->getFont(fontID), fontSize, text);

	for (int i = 0; i < lines.size(); i++)
	{
		Text newLine;
		newLine.setFont(*textureManager->getFont(fontID));
		newLine.setCharacterSize(fontSize);
		if (textLines.size() == 0)
		{
			newLine.setPosition(Vector2f((int)(topLeft.x + size.x*0.05), (int)(topLeft.y + fontSize * 1.1 * textLines.size() + size.y*0.05)));
		}
		else
		{
			newLine.setPosition(Vector2f((int)(topLeft.x + size.x*0.05),
				(int)(topLeft.y + (textLines.at(textLines.size() - 1)).getCharacterSize() * 1.1 * textLines.size() + size.y*0.05)));
		}
		newLine.setString(lines[i]);
		newLine.setFillColor(Color(255, 255, 255, 192));
		textLines.push_back(newLine);
	}
	scaledTextLines.clear();
	for (int i = 0; i < textLines.size(); i++)
	{
		Text newLine;
		newLine.setFont(*textLines[i].getFont());
		newLine.setCharacterSize((int)(textLines[i].getCharacterSize() * 1.1));
		Vector2f pos = textLines.at(i).getPosition();
		float X = pos.x - size.x * 0.025;
		float Y = pos.y - size.y * 0.025;
		newLine.setPosition(Vector2f((int)X, (int)Y));
		newLine.setString(textLines[i].getString());
		newLine.setFillColor(Color(255, 255, 255, 192));
		scaledTextLines.push_back(newLine);
	}
}

void ButtonObject::centerText()
{
	for (int i = 0; i < textLines.size(); i++)
	{
		FloatRect textSize = textLines[i].getLocalBounds();
		textLines[i].setPosition(Vector2f(
			(int)(topLeft.x + size.x / 2 - textSize.width / 2),
			textLines[i].getPosition().y));

		textSize = scaledTextLines[i].getLocalBounds();
		scaledTextLines[i].setPosition(Vector2f(
			(int)(topLeft.x + size.x / 2 - textSize.width / 2),
			scaledTextLines[i].getPosition().y));
	}
}
