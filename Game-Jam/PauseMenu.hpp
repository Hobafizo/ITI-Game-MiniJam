#pragma once
#include "BaseMenu.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

class PauseMenu : public BaseMenu {
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture resumeTexture;
    sf::Sprite resumeSprite;

    sf::Texture quitTexture;
    sf::Sprite quitSprite;

    sf::Vector2u windowSize = sf::Vector2u(1920, 1080);

    bool loadAssets() {
        if (!backgroundTexture.loadFromFile("Assets/UI/Menus/MenusUI1/pause.png")) return false;
        if (!resumeTexture.loadFromFile("Assets/UI/Menus/MenusUI1/Resume.png")) return false;
        if (!quitTexture.loadFromFile("Assets/UI/Menus/MenusUI1/exit.png")) return false;
        return true;
    }

    void setupSprites() {
        backgroundSprite.setTexture(backgroundTexture);

        float bgScaleX = (float)windowSize.x / backgroundTexture.getSize().x;
        float bgScaleY = (float)windowSize.y / backgroundTexture.getSize().y;
        backgroundSprite.setScale(bgScaleX, bgScaleY);

        float buttonWidth = 1000.0f;
        float centerX = (windowSize.x - buttonWidth) * 0.5f;

        float resumeScale = buttonWidth / resumeTexture.getSize().x;
        resumeSprite.setTexture(resumeTexture);
        resumeSprite.setScale(resumeScale, resumeScale);
        resumeSprite.setPosition(centerX, 260.0f);

        float quitScale = buttonWidth / quitTexture.getSize().x;
        quitSprite.setTexture(quitTexture);
        quitSprite.setScale(quitScale, quitScale);
        quitSprite.setPosition(centerX, 420.0f);
    }

public:
    PauseMenu() {
        if (!loadAssets()) {
            std::cout << "ERROR: Failed to load PauseMenu assets.\n";
        }
        else {
            setupSprites();
        }
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(backgroundSprite);
        window.draw(resumeSprite);
        window.draw(quitSprite);
    }

    MenuAction checkClick(sf::RenderWindow& window) override {
        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (resumeSprite.getGlobalBounds().contains(mouse)) {
            std::cout << "PauseMenu -> Resume clicked\n";
            return START_GAME;
        }
        if (quitSprite.getGlobalBounds().contains(mouse)) {
            std::cout << "PauseMenu -> Quit clicked\n";
            return GOTO_MAIN_MENU;
        }
        return NONE;
    }
};