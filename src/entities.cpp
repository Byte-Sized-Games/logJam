#include "entities.hpp"

unsigned int entity::Entity::ActiveEntities = 0; // Set Initial Amount of active entities (gameObjects)

entity::Entity::Entity() {
	position = new raylib::Vector2(0, 0);
	id = ActiveEntities++;
}

entity::Entity::~Entity() { ActiveEntities--; }
