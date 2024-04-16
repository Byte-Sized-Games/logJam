// -- Headers -- //
#include <iostream>
#include "raylib-cpp.hpp"
#include "string"
#include <cstring>
#include "entities.hpp"
// #include "ui.hpp"
#include "functional"
#include "vector"
// #include "GameState.hpp"
#include "screen.hpp"

// -- Definitions -- //
#define VERSION "dev"

using namespace std;

namespace console {
bool level;
void log(const string &target) { cout << target << endl; }
void debug(const string &target) {
  if (level)
    log(target);
}
} // namespace console

/// @brief Enum used to manage different stages of the game.
/// gameState enum is used to determine what game logic should be running at a given moment.
/// Includes menus, levels and settings.
enum gameState { loading = 0, menu, levelSelect, level };

int main() {
  //  Persistent callstacks. Probably not a good idea to use these, but they're here.
  const vector<pair<function<void(void)>, bool>> LogicStack, DrawStack;
  console::level = true;
  const unsigned int screenWidth = 800, screenHeight = 800;
  const string title = "Logger - Version: " + ((string)VERSION); // in game title
  // GameState CurrentState = GameState();
  //  CurrentState.DrawStack.insert({[](){console::debug("balls");}, true})

  gameState currentState = loading; // set initial scene for game
  int delta = 60;                   // deltaTime/framerate target

  raylib::Window window(screenWidth, screenHeight, title); // Initialise window and window title

  int frameCounter = 0; // Frame utility. Used to check time

  // Set Target fps to deltaTime
  SetTargetFPS(delta);

  Button playButton = Button(20, 250, "Play", raylib::Color::DarkGray(), raylib::Color::LightGray());
  Screen mainMenu = Screen(std::vector<UiElement *>{&playButton}, raylib::Color::SkyBlue());

  while (!WindowShouldClose()) {
    // ---------------------------------
    // Game Logic
    // ---------------------------------

    switch (currentState) {
    case loading:
      frameCounter++;
      // Catch 2 seconds passing
      if (frameCounter > 2 * delta) {
        currentState = menu;
      }
      break;
    case menu:
      frameCounter = 0;
      if (playButton.IsPressed()) {
        currentState = loading;
      }
      break;
    case levelSelect:
      break; // TODO
    case level:
      break; // TODO
    default:
      break;
    }

    // ---------------------------------
    // Draw
    // ---------------------------------
    BeginDrawing();
    { // Drawing is done in a separate scope for isolation and organization purposes
      switch (currentState) {
      case loading:
        //                window.ClearBackground(raylib::Color::LightGray());
        raylib::DrawText("Made with Raylib", GetScreenWidth() / 2 - 60, GetScreenHeight() / 2, 20, raylib::Color::Maroon());
        break;
      case menu:
        mainMenu.Render(&window);

        raylib::DrawText(title.c_str(), GetScreenWidth() / 10 - 64, GetScreenHeight() / 10, 40, raylib::Color::White());
        // playButton.Render();
        break;
      default:
        exit(1);
        break;
      }
    }

    EndDrawing();
  }

  return 0;
}
