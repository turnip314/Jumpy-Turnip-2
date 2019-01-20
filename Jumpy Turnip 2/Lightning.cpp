#include "Lightning.h"



Lightning::Lightning(Types::Entities thisType, Texture* texture, GameScene* thisScene, Vector2f thisSource, Vector2f thisTarget):
	Entity(thisType, texture, thisScene)
{
	// Lightning is basically a yellow line connecting the source and target
	source = thisSource;
	target = thisTarget;
	float length = Math::distance(source, target);
	float angle = Math::angle(Math::direction(source, target));
	line = RectangleShape(Vector2f(length, 2));
	line.setPosition(source);
	line.rotate(angle*180/Math::pi);
	line.setFillColor(Color::Yellow);
}


Lightning::~Lightning()
{
}

void Lightning::update(Time dt)
{
	float time = dt.asSeconds();
	lifeSpan -= time;
	if (lifeSpan <= 0)
	{
		addToRemoveList();
	}
}

void Lightning::render(RenderWindow* handle, Vector2f scale)
{
	handle->draw(line);
}

void Lightning::processInput(Keyboard::Key, bool isPressed)
{

}

void Lightning::processInput(Vector2i pos, bool isPressed)
{

}


