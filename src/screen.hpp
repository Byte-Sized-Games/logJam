#include "game.cpp"
#include <vector>
#include "raylib-cpp.hpp"

/// @brief Collection of Ui Elements.
/// Used to collect and simplify elements of a menu or screen, and save lines of code when rendering
class Screen {
  private:
	raylib::Color background;
	std::vector<ui::UiElement *> menu;

  public:
	Screen(void);
	~Screen(void);
	Screen(const std::vector<ui::UiElement *>&);
	Screen(const std::vector<ui::UiElement *>&, raylib::Color);
	void Render(raylib::Window *);
};
