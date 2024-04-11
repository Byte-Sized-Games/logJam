#include "entities.hpp"

Entity::Entity(void) {
  position = raylib::Vector2(0, 0);
  id = ActiveEntities + 1;
  ActiveEntities++;
}

Entity::~Entity(void) { ActiveEntities--; }
