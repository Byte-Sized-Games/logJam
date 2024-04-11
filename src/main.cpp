#include "inc/raylib-cpp.hpp"

int main(void) {
  int screenWidth = 800;
  int screenHeight = 450;

  raylib::Window window(screenWidth, screenHeight, "raylib cpp - basic window");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();

    window.ClearBackground(RAYWHITE);

    DrawText("Hello, World!", 190, 200, 20, LIGHTGRAY);
    EndDrawing();
  }

  return 0;
}
