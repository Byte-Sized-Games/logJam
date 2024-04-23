#include "raylib-cpp.hpp"

namespace entity {
enum direction { north, east, south, west };
class Entity {
  protected:
	raylib::Vector2 *position;
	int id;

  public:
	static unsigned int ActiveEntities;
	Entity(void);
	~Entity(void);
};

class Player : public Entity {
  private:
	raylib::Texture2D *spriteSheet;
	raylib::Texture2D *currentSprite;
	unsigned int speed;

  public:
	Player();
	Player(float, float);
	~Player();

	void Move(direction);
};
} // namespace entity
