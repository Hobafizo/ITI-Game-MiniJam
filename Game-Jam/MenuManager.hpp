#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <memory>
#include "BaseMenu.hpp"
#include "MainMenu.hpp"
#include "LevelMenu.hpp"
#include "PauseMenu.hpp"   // <--- NEW
#include "LoseMenu.hpp"    // <--- NEW

class MenuManager {
private:
    std::unique_ptr<BaseMenu> currentMenu;
    sf::Music _mainMusic;
    sf::Music _levelMusic;
    sf::SoundBuffer ck;
    sf::Sound ck_s;
    sf::Music _LoserMusic;

public:
    GameState currentState = MAIN_MENU;

    MenuManager() {
        currentMenu = std::make_unique<Menu>();
        std::cout << "MenuManager initialized with Main Menu." << std::endl;
        if (_mainMusic.openFromFile("Assets/Audio/John Powell This Is Berk (Piano Arrangement by Felipe Queiroga) - Felipe Queiroga.wav")) {
            _mainMusic.setLoop(true);
            _mainMusic.play();
        }
        else {
            std::cout << "Error loading Menu Music" << std::endl;
        }

        if (ck.loadFromFile("Assets/Audio/Water Plop - Sound Effect (HD) - Gaming Sound FX.wav")) {
            ck_s.setBuffer(ck);
        }
        if (_LoserMusic.getStatus() != sf::SoundSource::Playing)
        {
            // Load and Play
            _LoserMusic.openFromFile("Assets/Audio/YOU DIED (HD) - iwanPlays.wav");
        }
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
            _mainMusic.play();
            _mainMusic.setLoop(true);
            std::cout << "--- MenuManager switched state to Main Menu ---" << std::endl;
            break;

        case START_GAME:
            //currentMenu.reset();
            currentState = LOADING_LEVEL;
            _mainMusic.stop();
            

            std::cout << "--- MenuManager switched state to ACTIVE GAME ---" << std::endl;
            break;
           
        case RESUME_GAME:
            currentState = ACTIVE_GAME;
            BoxML::Instance()->SetRenderState(WorldRenderState::Running);
            //_mainMusic.stop();
            std::cout << "--- MenuManager switched state to ACTIVE GAME ---" << std::endl;
            break;

        case NONE:
            break;
        }
    }
    void handleHover() {
        sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition();
        if(currentMenu) {
			currentMenu->checkHover(mousePos);
		}
    }

    void setState(GameState newState) {
        currentState = newState;
    }

    // ----- NEW: Pause menu functions -----

    void showPauseMenu() {
        if(currentState==PAUSED) return;
        currentMenu = std::make_unique<PauseMenu>();
        currentState = PAUSED;
    }
    void showLoseMenu() {
        if(currentState==LOSE_MENU) return;
        currentMenu = std::make_unique<LoseMenu>();
        currentState= LOSE_MENU;
    }

    void resumeGame() {
        currentMenu.reset();
        currentState = ACTIVE_GAME;
    }
};
