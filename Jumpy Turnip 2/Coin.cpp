#include "Coin.h"
#include <iostream>

Coin::Coin(Types::Entities thisType, Texture* texture, GameScene* thisScene):
	Entity(thisType, texture, thisScene)
{
	sprite.setTexture(*texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 30, 30));
}

Coin::~Coin()
{
}

void Coin::update(Time dt)
{
	float time = dt.asSeconds();
	phaseTimer -= time;

	// Coin image is switched to one in a different period of rotation every
	// "phase" to make it look like it's spinning
	if (phaseTimer <= 0)
	{
		phase++;
		phaseTimer = 0.05f;

		int coinPhase = phase % 6;
		sprite.setTextureRect(sf::IntRect(30*coinPhase, 0, 30, 30));
	}

	// Disappears after 25 phases
	if (phase >= 25)
	{
		addToRemoveList();
	}
}

void Coin::render(RenderWindow* handle, Vector2f scale)
{
	sprite.setPosition(Vector2f(position.x - radius, position.y - radius));
	handle->draw(sprite);
}

void Coin::processInput(Keyboard::Key, bool isPressed)
{

}
void Coin::processInput(Vector2i pos, bool isPressed)
{

}

void Coin::setPosition(Vector2f pos)
{
	position = pos;
}