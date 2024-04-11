#include "inc/raylib-cpp.hpp"

class Entity {
protected:
  raylib::Vector2 position;
  int id;

public:
  static int ActiveEntities;
  Entity(void);
  ~Entity(void);
};
