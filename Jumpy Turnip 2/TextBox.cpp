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
	return TextBox::splitString(input, ' ');
}

vector<string> TextBox::splitString(string input, char splitChar)
{
	vector<string> acc;
	input += splitChar;

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