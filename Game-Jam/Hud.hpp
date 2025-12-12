#pragma once

#include "BaseMenu.hpp"
#include "BoxML.h"
// "Assets/UI/Menus/LevelMenu/"
//"Assets/UI/Menus/MainMenu/"
class Hud {
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture startTexture;
    sf::Sprite startSprite;
    sf::Texture exitTexture;
    sf::Sprite exitSprite;

    bool loadAssets() {
        if (!backgroundTexture.loadFromFile("Assets/UI/HudUI2/hud.png")) { cout << "did load hud?";return false; };
        /*if (!startTexture.loadFromFile("Assets/UI/HudUI2/withoutButtons.jpg")) return false;
        if (!exitTexture.loadFromFile("Assets/UI/HudUI2/withoutButtons.jpg")) return false;*/
        return true;
    }

    void setupSprites() {
		sf::Vector2u windowSize = {1920,1080};

        backgroundSprite.setTexture(backgroundTexture);
        startSprite.setTexture(startTexture);
        exitSprite.setTexture(exitTexture);

        float scaleX = (float)windowSize.x / backgroundTexture.getSize().x;
        float scaleY = (float)windowSize.y / backgroundTexture.getSize().y;
        backgroundSprite.setScale(scaleX-50.f, scaleY-50.f);
        backgroundSprite.setPosition(0.0f, 0.0f);

       /* float desiredButtonWidth = 1000.0f;
        float centerX = (windowSize.x - desiredButtonWidth) / 2.0f;

        float startScale = desiredButtonWidth / startTexture.getSize().x;
        startSprite.setScale(startScale, startScale);
        startSprite.setPosition(centerX, 200.0f);

        float exitScale = desiredButtonWidth / exitTexture.getSize().x;
        exitSprite.setScale(exitScale, exitScale);
        exitSprite.setPosition(centerX, 400.0f);*/
    }

public:
    Hud() {
        if (!loadAssets()) {
            std::cerr << "ERROR: Failed to load Hud assets." << std::endl;
        }
        else {
            setupSprites();
        }
    }

    void draw(sf::RenderWindow& window){
        window.draw(backgroundSprite);
        window.draw(exitSprite);
        window.draw(startSprite);
    }

};