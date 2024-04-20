//
// Created by phoenix on 19/04/24.
//

#ifndef LOGGER_MENU_H
#define LOGGER_MENU_H

#include "screen.cpp"
#include "vector"
#include "string"

class Menu : Screen {
    std::vector<std::string> Items;
    std::vector<std::pair<std::vector<std::string>,std::vector<UiElement>>> Pages;

};


#endif //LOGGER_MENU_H
