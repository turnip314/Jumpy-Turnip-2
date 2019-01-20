#include "TextBox.h"
#include <iostream>


TextBox::TextBox()
{
}


TextBox::~TextBox()
{
}

vector<string> TextBox::splitString(string input)
{
	// Splits string into a list of strings by each appearance of a space
	return TextBox::splitString(input, ' ');
}

vector<string> TextBox::splitString(string input, char splitChar)
{
	// Splits string into a list of strings by each appearance of splitChar
	vector<string> acc;
	input += splitChar;

	// Keeps track of current consecutive char appearance without splitChar
	// When splitChar is reached, adds curWord to string list and restart
	string curWord;
	for (char c : input)
	{
		if (c == splitChar && curWord.length() > 0)
		{
			acc.push_back(curWord);
			curWord.clear();
		}
		else if (c == splitChar)
		{
			// do nothing
		}
		else
		{
			curWord += c;
		}
	}

	return acc;
}

vector<string> TextBox::getTextLines(Vector2f size, Font* font, int fontSize, string text)
{
	// Given a rectangle with a font, font size, and text, will try to fit that text inside
	// the rectangle and return the string list of each line of text

	vector<string> acc;
	
	string indent;
	while (text.find("_") == 0)
	{
		text.erase(text.begin());
		indent += " ";
	}

	vector<string> words = splitString(text);
	Text textImage;
	textImage.setCharacterSize(fontSize);
	textImage.setFont(*font);
	
	// curFit is guaranteed to fit the box, test is the next word to try
	string curFit;
	string test;

	// Will try to keep appending words to curFit (sotred in test)
	// until they no longer fit, then add the last curFit that works into the list
	while (words.size() > 0)
	{
		test += words.at(0) + ' ';
		textImage.setString(test);
		while (textImage.getLocalBounds().width < size.x && words.size() > 0)
		{
			curFit = test;
			words.erase(words.begin());
			if (words.size() > 0)
			{
				test += words.at(0) + ' ';
				textImage.setString(test);
			}
		}

		if (curFit.size() == 0)
		{
			curFit = words.at(0);
			words.erase(words.begin());
		}

		acc.push_back(indent + curFit);
		test.erase();
		curFit.erase();
	}

	return acc;
}