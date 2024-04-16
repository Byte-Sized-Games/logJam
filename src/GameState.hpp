//
// Created by phoenix on 15/04/24.
//

#ifndef LOGJAM_GAMESTATE_HPP
#define LOGJAM_GAMESTATE_HPP

#include "ui.hpp"
#include "vector"
#include "functional"
#include "utility"

class GameState {
public:
//    Elements drawn on state init
    const std::vector<UiElement*> InitScene;
//    State-specific callstack on input processing and frame events, respectively
//    formatted as <void callback(void),ephemerality>
    const std::vector<std::pair<std::function<void()>,bool>> LogicStack, DrawStack;
    GameState();
};


#endif //LOGJAM_GAMESTATE_HPP
