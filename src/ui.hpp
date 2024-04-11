#include "inc/raylib-cpp.hpp"
#include <string>

class UiElement {
protected:
  raylib::Vector2 position;

public:
  UiElement(void);
  ~UiElement(void);
};
