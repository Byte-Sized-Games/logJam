#include "screen.hpp"
#include <iostream>

// -- Screen -- //

/// @brief Default constructor !DO NOT USE! it is useless and will not let you render anything meaningful
Screen::Screen(void) { background = raylib::Color::White(); }

/// @brief Secondary Constructor. Does not cover initialisation of background colour
/// @param Menu An array of references to Ui Elements of the menu
/// @param Length Length of the array
Screen::Screen(std::vector<UiElement *> Menu) {
  background = raylib::Color::White();
  for (UiElement *element : Menu) {
    menu.insert(menu.end(), element);
  }
}

/// @brief Primary Constructor. Does not cover initialisation of background colour
/// @param Menu An array of references to Ui Elements of the menu
/// @param Length Length of the array
/// @param Background Background colour of the menu
Screen::Screen(std::vector<UiElement *> Menu, raylib::Color Background) {
  background = Background;
  for (UiElement *element : Menu) {
    menu.insert(menu.end(), element);
  }
}

/// @brief Destructor for Screen. Deletes all references and complicated objects to free memory
Screen::~Screen(void) {
  /*for (UiElement *element : menu) {
    delete element;
  }*/
}

// -- Screen Methods -- //

/// @brief Renders all elements in Screen to the game
void Screen::Render(raylib::Window *Game) {
  Game->ClearBackground(background);
  for (UiElement *element : menu) {
    element->Render();
  }
}
