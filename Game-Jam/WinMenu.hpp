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

    sf::Texture nextTexture;
    sf::Sprite nextSprite;

    sf::Texture quitTexture;
    sf::Sprite quitSprite;
    sf::Vector2f defaultScale;
    sf::Vector2f defaultScaleOther;
    
    bool loadAssets() {
        if (!backgroundTexture.loadFromFile("Assets/background/win.png")) return false;
        if (!menuTexture.loadFromFile("Assets/UI/Menus/MenusUI1/Back.png")) return false;
        if (!nextTexture.loadFromFile("Assets/UI/Menus/MenusUI1/Resume.png")) return false;
        if (!quitTexture.loadFromFile("Assets/UI/Menus/MenusUI1/exit2.png")) return false;
        return true;
    }

    void setupSprites() {
		sf::Vector2u windowSize = BoxML::Instance()->Resolution();

        backgroundSprite.setTexture(backgroundTexture);

        float bgScaleX = (float)windowSize.x / backgroundTexture.getSize().x;
        float bgScaleY = (float)windowSize.y / backgroundTexture.getSize().y;
        backgroundSprite.setScale(bgScaleX, bgScaleY);

        float buttonWidth = 200.0f;
        float centerX = (windowSize.x - buttonWidth) * 0.5f;

        float menuScale = buttonWidth / menuTexture.getSize().x;
        menuSprite.setTexture(menuTexture);
        menuSprite.setScale(menuScale, menuScale);
        menuSprite.setPosition(centerX, 260.0f);

        float nextScale = buttonWidth / nextTexture.getSize().x;
        nextSprite.setTexture(nextTexture);
        nextSprite.setScale(nextScale, nextScale);
        nextSprite.setPosition(centerX, 460.f);
        defaultScaleOther=nextSprite.getScale();
        float quitScale = buttonWidth / quitTexture.getSize().x;
        quitSprite.setTexture(quitTexture);
        quitSprite.setScale(quitScale, quitScale);
        quitSprite.setPosition(centerX, 620.0f);
        defaultScale=quitSprite.getScale();
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
        window.draw(nextSprite);
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
        if (nextSprite.getGlobalBounds().contains(mousePos)) {
            cout<<"WinMenu -> Next Level clicked\n";
            return START_GAME;
        }
        return NONE;
    }
    void checkHover(sf::Vector2f mousePos)
    {
        // Reset all to default
        menuSprite.setScale(defaultScaleOther);
        nextSprite.setScale(defaultScaleOther);
        quitSprite.setScale(defaultScale);

        // Hover scaling
        if (menuSprite.getGlobalBounds().contains(mousePos))
        {
            menuSprite.setScale(defaultScaleOther.x * 1.1f,
                defaultScaleOther.y * 1.1f);
        }
        else if (nextSprite.getGlobalBounds().contains(mousePos))
        {
            nextSprite.setScale(defaultScaleOther.x * 1.1f,
                defaultScaleOther.y * 1.1f);
        }
        else if (quitSprite.getGlobalBounds().contains(mousePos))
        {
            quitSprite.setScale(defaultScale.x * 1.1f,
                defaultScale.y * 1.1f);
        }
    }
};