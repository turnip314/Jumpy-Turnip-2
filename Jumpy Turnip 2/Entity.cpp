#include "Entity.h"
#include "GameScene.h"


Entity::Entity(Types::Entities thisType, Texture* thisTexture, GameScene* thisScene)
{
	type = thisType;
	texture = thisTexture;
	scene = thisScene;
}

Entity::~Entity()
{
}

void Entity::addToRemoveList()
{
	scene->addEntityToRemove(this);
}
