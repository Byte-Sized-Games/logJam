//
// Created by phoenix on 20/04/24.
//
#include "gameState.hpp"

namespace game {
/// @brief Enum used to determine what game logic should be running at a given moment.
enum gameState { loading = 0, menu, levelSelect, level };
extern GameState CurrentState;
} // namespace game
