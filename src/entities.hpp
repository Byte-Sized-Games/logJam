#include "raylib-cpp.hpp"

class Entity {
protected:
  raylib::Vector2 *position;
  int id;

public:
  static unsigned int ActiveEntities;
  Entity(void);
  ~Entity(void);
};
