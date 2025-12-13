#pragma once
#include "BaseMenu.hpp"
#include "BoxML.h"

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
    sf::Vector2f defaultScale;
    bool loadAssets() {
        if (!backgroundTexture.loadFromFile("Assets/UI/Menus/MenusUI1/pause.png")) return false;
        if (!resumeTexture.loadFromFile("Assets/UI/Menus/MenusUI1/Resume.png")) return false;
        if (!quitTexture.loadFromFile("Assets/UI/Menus/MenusUI1/Back.png")) return false;
        return true;
    }

    void setupSprites() {
		sf::Vector2u windowSize = BoxML::Instance()->Resolution();

        backgroundSprite.setTexture(backgroundTexture);

        float bgScaleX = (float)windowSize.x / backgroundTexture.getSize().x;
        float bgScaleY = (float)windowSize.y / backgroundTexture.getSize().y;
        backgroundSprite.setScale(bgScaleX, bgScaleY);

        float buttonWidth = 150.0f;
        float centerX = (windowSize.x - buttonWidth) * 0.5f;

        float resumeScale = buttonWidth / resumeTexture.getSize().x;
        resumeSprite.setTexture(resumeTexture);
        resumeSprite.setScale(resumeScale, resumeScale);
        resumeSprite.setPosition(centerX, 360.0f);

        float quitScale = buttonWidth / quitTexture.getSize().x;
        quitSprite.setTexture(quitTexture);
        quitSprite.setScale(quitScale, quitScale);
        quitSprite.setPosition(centerX, 620.0f);
        defaultScale = resumeSprite.getScale();
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
		sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);

        if (resumeSprite.getGlobalBounds().contains(mousePos)) {
            std::cout << "PauseMenu -> Resume clicked\n";
            return RESUME_GAME;
        }
        if (quitSprite.getGlobalBounds().contains(mousePos)) {
            std::cout << "PauseMenu -> Quit clicked\n";
            return GOTO_MAIN_MENU;
        }
        return NONE;
    }
    void checkHover(sf::Vector2f mousePos) {
        if(resumeSprite.getScale()!= defaultScale)
			resumeSprite.setScale(defaultScale);
        if(quitSprite.getScale()!= defaultScale)
            quitSprite.setScale(defaultScale);

        if (resumeSprite.getGlobalBounds().contains(mousePos)) {
            resumeSprite.setScale(resumeSprite.getScale().x * 1.1f, resumeSprite.getScale().y * 1.1f);
        }
        if (quitSprite.getGlobalBounds().contains(mousePos)) {
            quitSprite.setScale(quitSprite.getScale().x * 1.1f, quitSprite.getScale().y * 1.1f);
        }

    }
};