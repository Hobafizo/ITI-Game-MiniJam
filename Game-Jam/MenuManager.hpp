#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <memory>
#include "BaseMenu.hpp"
#include "MainMenu.hpp"
#include "LevelMenu.hpp"

class MenuManager {
private:
    std::unique_ptr<BaseMenu> currentMenu;
    sf::Music _mainMusic;
    sf::Music _levelMusic;
    sf::SoundBuffer ck;
    sf::Sound ck_s;

public:
    GameState currentState = MAIN_MENU;

    MenuManager() {
        _mainMusic.openFromFile("Assets/Audio/John Powell This Is Berk (Piano Arrangement by Felipe Queiroga) - Felipe Queiroga.wav");

        ck.loadFromFile("Assets/Audio/Water Plop - Sound Effect (HD) - Gaming Sound FX.wav");
        ck_s.setBuffer(ck);

        currentMenu = std::make_unique<Menu>();
        std::cout << "MenuManager initialized with Main Menu." << std::endl;

        _mainMusic.setLoop(true);
        _mainMusic.play();
    }

    void draw(sf::RenderWindow& window) {
        if (currentMenu) {
            currentMenu->draw(window);
        }
    }

    void handleClicks(sf::RenderWindow& window) {
        if (!currentMenu) return;

        MenuAction action = currentMenu->checkClick(window);

        if (action != NONE) {
            ck_s.play();
        }

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
            _mainMusic.stop();
            _levelMusic.openFromFile("Assets/Audio/Forbidden Friends.wav");
            _levelMusic.setLoop(true);
            _levelMusic.play();
            std::cout << "--- MenuManager switched state to ACTIVE GAME ---" << std::endl;
            break;

        case NONE:
            break;
        }
    }

    void setState(GameState newState) {
        currentState = newState;
    }
};