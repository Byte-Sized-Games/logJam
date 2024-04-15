#include "entities.hpp"

Entity::Entity() {
  position = new raylib::Vector2(0, 0);
  id = ActiveEntities++;
}

Entity::~Entity() { ActiveEntities--; }
