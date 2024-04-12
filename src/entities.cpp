#include "entities.hpp"

int Entity::ActiveEntities = 0;

Entity::Entity(void) {
  position = raylib::Vector2(0, 0);
  id = ActiveEntities + 1;
  ActiveEntities++;
}

Entity::~Entity(void) { ActiveEntities--; }
