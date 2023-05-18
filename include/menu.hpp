#pragma once

#include "typing.hpp"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace Imdb {
    struct MenuItem {
        std::string title;
        std::function<void(OpMovie, OpRecords)> callbackFn;
        OpArgs fnArgs;
    };

    class Menu {
    private:
        std::string _menuTitle;
        static bool _menuState;
        std::unordered_map<uint32_t, MenuItem> _items;

    private:
        class MenuBuilder {
        public:
            explicit MenuBuilder(const std::string &title) : _menu(std::make_unique<Menu>(title)) {}

            MenuBuilder &addItem(const std::string &title, std::function<void(OpMovie, OpRecords)> fn, OpArgs fnArgs);
            std::unique_ptr<Menu> build();

        private:
            std::unique_ptr<Menu> _menu;
        };

    public:
        explicit Menu(const std::string &title) : _menuTitle(title) {}

        static inline MenuBuilder create(const std::string &title) { return MenuBuilder(title); }
        static inline void setAliveState(bool isAlive) { _menuState = isAlive; }
        inline bool isAlive() { return _menuState; }
        void display();
    };

} // namespace Imdb