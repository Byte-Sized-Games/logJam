//
// Created by phoenix on 19/04/24.
//

#ifndef LOGGER_SCROLLABLELIST_H
#define LOGGER_SCROLLABLELIST_H

#include <functional>
#include "vector"
#include "string"
#include "ui.hpp"

namespace ui {
    class ScrollableList {
        std::vector<std::pair<std::string,std::function<void()>>> Bindings;
        Button ItemTemplate;
        ScrollableList(const std::vector<std::pair<std::string,std::function<void()>>>& Bindings, const unsigned int& width);
        ScrollableList(const std::vector<std::pair<std::string,std::function<void()>>>& Bindings, const unsigned int& width, const Button& ItemTemplate);
    };
}



#endif //LOGGER_SCROLLABLELIST_H
