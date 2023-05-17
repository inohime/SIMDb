#include "menu.hpp"
#include <format>
#include <iostream>
#include <limits>

namespace Imdb {
    bool Menu::_menuState = false;

    Menu::MenuBuilder &Menu::MenuBuilder::addItem(const std::string &title, std::function<void(OpMovie, OpRecords)> fn,
                                                  OpArgs fnArgs) {
        static uint32_t idx = 1;
        _menu->_items.insert({idx, {title, fn, fnArgs}});
        idx++;

        return *this;
    }

    std::unique_ptr<Menu> Menu::MenuBuilder::build() {
        setAliveState(true);
        return std::move(_menu);
    }

    void Menu::display() {
        std::cout << _menuTitle << '\n';

        for (const auto &[itemIdx, item] : _items) {
            std::cout << std::format("  {}. {}\n", itemIdx, item._title);
        }

        std::cout << "Your choice: ";

        size_t in;
        if (std::cin >> in) {
            // verify the input is not greater than the size of our map and not 0
            if (in > _items.size() || in == 0) {
                std::cout << "That option doesn't exist! Pick another option!\n\n";
                return display();
            }

            auto &[mvArg, rcArg] = *_items[in]._fnArgs;
            _items[in]._callbackFn(mvArg, rcArg);
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Malinformed input\n\n";
            return;
        }
    }
} // namespace Imdb