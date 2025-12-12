#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory> // Required for std::unique_ptr
using namespace std;
enum MenuAction {
    NONE,
    GOTO_LEVEL_MENU,
    GOTO_MAIN_MENU,
    START_GAME,
	RESUME_GAME,
    QUIT
};
const string path = "Assets/UI/Menus/MenusUI1";
// Define the authoritative states for the entire application
enum GameState {
    MAIN_MENU,
    LEVEL_MENU,
	LOADING_LEVEL,
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