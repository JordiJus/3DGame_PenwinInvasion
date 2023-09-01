#include "Entity.h"


Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::render() {

	// Render code if EntityMesh
	// ...

	for (int i = 0; i < children.size(); i++)
		children[i]->render();
}

void Entity::update(float elapsed_time)
{
}



void Entity::addChild(Entity* child)
{
	children.push_back(child);
	child->parent = this;
}

void Entity::removeChild(Entity* child)
{
	children.erase(std::remove(children.begin(), children.end(), child));
	child->~Entity();
}

Matrix44 Entity::getGlobalMatrix() {

	if (parent)
		return model * parent->getGlobalMatrix();

	return model;
}
