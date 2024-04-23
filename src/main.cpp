// -- Headers -- //
#include <iostream>
#include <string>
#include <cstring>
#include <functional>
#include <vector>
#include "raylib-cpp.hpp"

#include "console.cpp"
#include "entities.hpp"
#include "menu.hpp"

#define VERSION "dev"

using namespace std;
using namespace ui;
// using namespace game;

int main(int argv, char *argc[]) {
	// Check CLI args
	if ((argv >= 2) && (!strcmp(argc[1], "debug")))
		console::debug((console::level = true) ? "Debugging!" : ""); // NOLINT

	//  Persistent callStacks. Probably not a good idea to use these, but they're here.
	std::vector<std::function<bool()>> LogicStack, DrawStack;
	console::level = true;
	const unsigned int screenWidth = 800, screenHeight = 800;
	const string title = "Logger (" + ((string)VERSION) + ")"; // in game title
	// GameState CurrentState = GameState();
	//  CurrentState.DrawStack.insert({[](){console::debug("balls");}, true})

	game::gameState currentState = game::loading; // set initial scene for game
	int delta = 60;								  // deltaTime/framerate target

	raylib::Window window(screenWidth, screenHeight, title); // Initialise window and window title

	int frameCounter = 0; // Frame utility. Used to check time

	// Set Target fps to deltaTime
	SetTargetFPS(delta);

	Button playButton = Button(20, 250, "Play", raylib::Color::DarkGray(), raylib::Color::LightGray());
	Screen mainMenu = Screen(std::vector<UiElement *>{&playButton}, raylib::Color::SkyBlue());

	TextObject loadingText = TextObject(400, 400, "Made in Raylib with Love", raylib::Color::Maroon());
	Screen loadingScreen = Screen(std::vector<UiElement *>{&loadingText}, raylib::Color::LightGray());

	//  example ephemeral stack item
	LogicStack.emplace_back([]() {
		console::debug("ephemeral stack item");
		return true;
	});
	//  example permanent stack item
	LogicStack.emplace_back([]() {
		console::debug("permanent stack item");
		return false;
	});

	while (!WindowShouldClose()) {
		// ---------------------------------
		// Game Logic
		// ---------------------------------

		for (unsigned int i = 0; i < LogicStack.size(); i++)
			if (LogicStack[i]())
				LogicStack.erase(LogicStack.begin() + i);
		/*for (unsigned int i = 0; i < CurrentState.LogicStack.size(); i++)
			if (CurrentState.LogicStack[i]())
				CurrentState.LogicStack.erase(CurrentState.LogicStack.begin() + i);
		*/
		switch (currentState) {
		case game::loading:
			frameCounter++;
			// Catch 2 seconds passing
			if (frameCounter > 2 * delta) {
				currentState = game::menu;
			}
			break;
		case game::menu:
			frameCounter = 0;
			if (playButton.IsPressed()) {
				currentState = game::loading;
			}
			break;
		case game::levelSelect:
			break; // TODO
		case game::level:
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
			case game::loading:
				loadingScreen.Render(&window);

				break;
			case game::menu:
				mainMenu.Render(&window);

				raylib::DrawText(title.c_str(), GetScreenWidth() / 10 - 64, GetScreenHeight() / 10, 40, raylib::Color::White());
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
