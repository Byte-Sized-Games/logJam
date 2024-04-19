//
// Created by phoenix on 15/04/24.
//
#include "ui.hpp"

#ifndef LOGJAM_GAMESTATE_HPP
#define LOGJAM_GAMESTATE_HPP

#include "vector"
#include "functional"
#include "utility"

class GameState {
public:
//    Elements drawn on state init
    const std::vector<UiElement*> InitScene;
//    State-specific callstack on input processing and frame events, respectively
//    formatted as <void callback(void),ephemerality>
    std::vector<std::function<bool()>> LogicStack, DrawStack;
    GameState() = default;
};


#endif //LOGJAM_GAMESTATE_HPP
