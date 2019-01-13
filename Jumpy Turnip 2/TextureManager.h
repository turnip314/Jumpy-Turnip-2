#pragma once

#include <SFML/Graphics.hpp>

#include "Textures.h"
#include "TextBox.h"

using namespace std;
using namespace sf;

class TextureManager
{
public:
	void load(Textures::ID ID, String filePath);
	void load(Fonts::ID ID, String filePath);
	Texture* getTexture(Textures::ID ID);
	Font* getFont(Fonts::ID ID);

private:
	map<Textures::ID, Texture*> images;
	map<Fonts::ID, Font*> fonts;
};
