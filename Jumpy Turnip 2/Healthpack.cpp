#include "Healthpack.h"



Healthpack::Healthpack(Types::Entities thisType, Texture* thisTexture, GameScene* thisScene):
	Entity(thisType, thisTexture, thisScene)
{
	sprite.setTexture(*texture);
	sprite.setOrigin(15.f, 15.f);
}

Healthpack::~Healthpack()
{
}

void Healthpack::update(Time dt)
{
	float time = dt.asSeconds();
	lifespan -= time;
	if (lifespan <= 0)
	{
		addToRemoveList();
	}
	sprite.setScale(1.8 - lifespan / 2.f, 1.8 - lifespan / 2.f);
	sprite.setColor(Color(255, 255, 255, 128 * lifespan));
}

void Healthpack::render(RenderWindow* handle, Vector2f scale)
{
	handle->draw(sprite);
}

void Healthpack::processInput(Keyboard::Key, bool isPressed)
{

}

void Healthpack::processInput(Vector2i pos, bool isPressed)
{

}

void Healthpack::setPosition(Vector2f pos)
{
	position = pos;
	sprite.setPosition(pos);
}
