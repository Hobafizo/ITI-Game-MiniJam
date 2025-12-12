#pragma once
#include "BaseMenu.hpp"
#include "BoxML.h"

#include <SFML/Graphics.hpp>
#include <iostream>

class LoseMenu : public BaseMenu {
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture retryTexture;
    sf::Sprite retrySprite;

    sf::Texture quitTexture;
    sf::Sprite quitSprite;

    bool loadAssets() {
        if (!backgroundTexture.loadFromFile("Assets/UI/Menus/LoseMenu/LoseBackground.jpg")) return false;
        if (!retryTexture.loadFromFile("Assets/UI/Menus/LoseMenu/Retry.jpg")) return false;
        if (!quitTexture.loadFromFile("Assets/UI/Menus/LoseMenu/Quit.jpg")) return false;
        return true;
    }

    void setupSprites() {
		sf::Vector2u windowSize = BoxML::Instance()->Resolution();

        backgroundSprite.setTexture(backgroundTexture);

        float bgScaleX = (float)windowSize.x / backgroundTexture.getSize().x;
        float bgScaleY = (float)windowSize.y / backgroundTexture.getSize().y;
        backgroundSprite.setScale(bgScaleX, bgScaleY);

        float buttonWidth = 300.0f;
        float centerX = (windowSize.x - buttonWidth) * 0.5f;

        float retryScale = buttonWidth / retryTexture.getSize().x;
        retrySprite.setTexture(retryTexture);
        retrySprite.setScale(retryScale, retryScale);
        retrySprite.setPosition(centerX, 260.0f);

        float quitScale = buttonWidth / quitTexture.getSize().x;
        quitSprite.setTexture(quitTexture);
        quitSprite.setScale(quitScale, quitScale);
        quitSprite.setPosition(centerX, 420.0f);
    }

public:
    LoseMenu() {
        if (!loadAssets()) {
            std::cout << "ERROR: Failed to load LoseMenu assets.\n";
        }
        else {
            setupSprites();
        }
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(backgroundSprite);
        window.draw(retrySprite);
        window.draw(quitSprite);
    }

    MenuAction checkClick(sf::RenderWindow& window) override {
		sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);

        if (retrySprite.getGlobalBounds().contains(mousePos)) {
            std::cout << "LoseMenu -> Retry clicked\n";
            return START_GAME;
        }
        if (quitSprite.getGlobalBounds().contains(mousePos)) {
            std::cout << "LoseMenu -> Quit clicked\n";
            return QUIT;
        }
        return NONE;
    }
};