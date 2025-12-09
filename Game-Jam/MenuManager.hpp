#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "BaseMenu.hpp"
#include "MainMenu.hpp"
#include "LevelMenu.hpp"
class MenuManager {
private:
    std::unique_ptr<BaseMenu> currentMenu;

public:
    // This must be public for main loop control
    GameState currentState = MAIN_MENU;

    
        MenuManager() {
        // Since Menu is defined above, we can instantiate it here
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
            currentMenu = std::make_unique<Menu>(); // New Main Menu instance
            currentState = MAIN_MENU;
            std::cout << "--- MenuManager switched state to Main Menu ---" << std::endl;
            break;

        case START_GAME:
            currentMenu.reset(); // Destroy the menu, resources are freed
            currentState = ACTIVE_GAME;
            std::cout << "--- MenuManager switched state to ACTIVE GAME ---" << std::endl;
            break;

        case NONE:
            break;
        }
    }

    // Helper to change state directly (used by main loop for pause)
    void setState(GameState newState) {
        currentState = newState;
    }
};