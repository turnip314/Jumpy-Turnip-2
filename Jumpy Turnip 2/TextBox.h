#pragma once
#include "Math.h"
#include <SFML\Graphics.hpp>
#include "Textures.h"

class TextBox
{
public:
	TextBox();
	~TextBox();

	// Splits it by space by default
	static vector<string>  splitString(string input);
	// Splits string based on character
	static vector<string> splitString(string input, char splitChar);
	// Given rectangle size, font, and fontSize, will create lines of text that fit inside the rectangle
	static vector<string> getTextLines(Vector2f size, Font* font, int fontSize, string text);
};

