#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "BaseMenu.hpp"
#include "MainMenu.hpp"
#include "LevelMenu.hpp"
#include "PauseMenu.hpp"   // <--- NEW

class MenuManager {
private:
    std::unique_ptr<BaseMenu> currentMenu;

public:
    GameState currentState = MAIN_MENU;

    MenuManager() {
        currentMenu = std::make_unique<Menu>();
        std::cout << "MenuManager initialized with Main Menu." << std::endl;
    }

    void draw(sf::RenderWindow& window) {
        if (currentMenu) {
            currentMenu->draw(window);
        }
    }

    void handleClicks(sf::RenderWindow& window) {
        if (!currentMenu) return;

        MenuAction action = currentMenu->checkClick(window);

        switch (action) {
        case QUIT:
            currentState = EXIT;
            break;

        case GOTO_LEVEL_MENU:
            currentMenu = std::make_unique<LevelMenu>();
            currentState = LEVEL_MENU;
            std::cout << "--- MenuManager switched state to Level Menu ---" << std::endl;
            break;

        case GOTO_MAIN_MENU:
            currentMenu = std::make_unique<Menu>();
            currentState = MAIN_MENU;
            std::cout << "--- MenuManager switched state to Main Menu ---" << std::endl;
            break;

        case START_GAME:
            currentMenu.reset();
            currentState = ACTIVE_GAME;
            std::cout << "--- MenuManager switched state to ACTIVE GAME ---" << std::endl;
            break;

        case NONE:
            break;
        }
    }

    void setState(GameState newState) {
        currentState = newState;
    }

    // ----- NEW: Pause menu functions -----

    void showPauseMenu() {
        currentMenu = std::make_unique<PauseMenu>();
        currentState = PAUSED;
    }

    void resumeGame() {
        currentMenu.reset();
        currentState = ACTIVE_GAME;
    }
};