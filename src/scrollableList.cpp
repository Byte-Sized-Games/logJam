//
// Created by phoenix on 19/04/24.
//

#include "scrollableList.hpp"

ui::ScrollableList::ScrollableList(const std::vector<std::pair<std::string, std::function<void()>>> &Bindings, const unsigned int &width) {}

ui::ScrollableList::ScrollableList(const std::vector<std::pair<std::string, std::function<void()>>> &Bindings, const unsigned int &width,
								   const Button &ItemTemplate) {}
// TODO: Implement sortable list layout