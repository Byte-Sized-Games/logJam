// -- Headers -- //
#include "inc/raylib-cpp.hpp"
#include "string"
#include "string.h"
#include "entities.hpp"
#include "ui.hpp"

// -- Definitions -- //
#define VERSION "dev"

using namespace std;

/// @brief Enum used to manage different stages of the game.
/// gameState enum is used to determine what game logic should be running at a given moment.
/// Includes menus, levels and settings.
enum gameState { loading = 0, menu, levelSelect, level };

int Entity::ActiveEntities = 0; // Set Initial Amount of active entities (gameObjects)

int main(void) {
  // ---------------------------------
  // Initialise Game
  // ---------------------------------
  int screenWidth = 800;               // screen height
  int screenHeight = 800;              // screen width
  string title = "Logger - Version: "; // in game title
  title.append(VERSION);               // set game version in title
  gameState currentState = loading;    // set initial scene for game
  int delta = 60;                      // deltaTime/framerate target

  raylib::Window window(screenWidth, screenHeight, "Logger"); // Initialise window and window title

  int frameCounter = 0; // Frame utility. Used to check time

  // Set Target fps to deltaTime
  SetTargetFPS(delta);

  Button playButton = Button(20, 250, "Play", raylib::Color::DarkGray(), raylib::Color::LightGray());

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
        window.ClearBackground(raylib::Color::LightGray());
        raylib::DrawText("Made in raylib", GetScreenWidth() / 2 - 60, GetScreenHeight() / 2, 20, raylib::Color::Maroon());
        break;
      case menu:
        window.ClearBackground(raylib::Color::SkyBlue());

        raylib::DrawText(title.c_str(), GetScreenWidth() / 10 - 64, GetScreenHeight() / 10, 40, raylib::Color::White());
        playButton.Render();
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
