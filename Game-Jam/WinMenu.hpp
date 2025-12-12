#pragma once
#include "BaseMenu.hpp"
#include "BoxML.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class WinMenu : public BaseMenu {
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture menuTexture;
    sf::Sprite menuSprite;

    sf::Texture quitTexture;
    sf::Sprite quitSprite;

    bool loadAssets() {
        if (!backgroundTexture.loadFromFile("Assets/UI/Menus/WinMenu/WinBackground.jpg")) return false;
        if (!menuTexture.loadFromFile("Assets/UI/Menus/WinMenu/Menu.jpg")) return false;
        if (!quitTexture.loadFromFile("Assets/UI/Menus/WinMenu/Quit.jpg")) return false;
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

        float menuScale = buttonWidth / menuTexture.getSize().x;
        menuSprite.setTexture(menuTexture);
        menuSprite.setScale(menuScale, menuScale);
        menuSprite.setPosition(centerX, 260.0f);

        float quitScale = buttonWidth / quitTexture.getSize().x;
        quitSprite.setTexture(quitTexture);
        quitSprite.setScale(quitScale, quitScale);
        quitSprite.setPosition(centerX, 420.0f);
    }

public:
    WinMenu() {
        if (!loadAssets()) {
            std::cout << "ERROR: Failed to load WinMenu assets.\n";
        }
        else {
            setupSprites();
        }
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(backgroundSprite);
        window.draw(menuSprite);
        window.draw(quitSprite);
    }

    MenuAction checkClick(sf::RenderWindow& window) override {
		sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);

        if (menuSprite.getGlobalBounds().contains(mousePos)) {
            std::cout << "WinMenu -> Back To Menu clicked\n";
            return GOTO_MAIN_MENU;
        }
        if (quitSprite.getGlobalBounds().contains(mousePos)) {
            std::cout << "WinMenu -> Quit clicked\n";
            return QUIT;
        }
        return NONE;
    }
};