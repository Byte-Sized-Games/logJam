#include "entities.hpp"

unsigned int Entity::ActiveEntities = 0;

Entity::Entity() {
  position = new raylib::Vector2(0, 0);
  id = ActiveEntities++;
}

Entity::~Entity() { ActiveEntities--; }
