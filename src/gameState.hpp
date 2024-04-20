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
	std::vector<ui::UiElement *>* InitScene;
	//    State-specific call stack on input processing and frame events, respectively
	//    deleted from stack if returns true
	std::vector<std::function<bool()>> LogicStack, DrawStack = {[](){
        return true;
    }};
	GameState() = default;
    GameState(std::vector<ui::UiElement *>* InitScene) {
        this->InitScene = InitScene;
    }
};

#endif // LOGJAM_GAMESTATE_HPP
