#include <SFML/Graphics.hpp>

#include "TextureManager.h"
#include <iostream>
#include <string>

using namespace sf;
using namespace std;

void TextureManager::load(Textures::ID ID, String filePath)
{
	// Loads texture from filePath and stores it as a pointer
	Texture* ptexture = new Texture;
	while (!ptexture->loadFromFile(filePath))
	{
		cout << (string) filePath;
	}
	images[ID] = ptexture;
}

void TextureManager::load(Fonts::ID ID, String filePath)
{
	// Loads font from filePath and stores it as a pointer
	Font* pFont = new Font;
	while (!pFont->loadFromFile(filePath))
	{
		throw runtime_error("Texture Manager: Failed to load '" + filePath + "'");
	}
	fonts[ID] = pFont;
}

Texture* TextureManager::getTexture(Textures::ID ID)
{
	// Retrieves texture pointer based on ID
	return images[ID];
}

Font* TextureManager::getFont(Fonts::ID ID)
{
	// Retrieves font pointer based on ID
	return fonts[ID];
}
