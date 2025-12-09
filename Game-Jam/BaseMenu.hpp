#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory> // Required for std::unique_ptr

enum MenuAction {
    NONE,
    GOTO_LEVEL_MENU,
    GOTO_MAIN_MENU,
    START_GAME,
    QUIT
};

// Define the authoritative states for the entire application
enum GameState {
    MAIN_MENU,
    LEVEL_MENU,
    ACTIVE_GAME,
    PAUSED,
    EXIT
};

// Base Menu Interface
class BaseMenu {
public:
    virtual ~BaseMenu() = default;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual MenuAction checkClick(sf::RenderWindow& window) = 0;
};