#include "BaseMenu.hpp"
#include "BoxML.h"
#include<SFML/Audio.hpp>

// "Assets/UI/Menus/LevelMenu/"
//"Assets/UI/Menus/MainMenu/"
class Menu : public BaseMenu {
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture startTexture;
    sf::Sprite startSprite;
    sf::Texture exitTexture;
    sf::Sprite exitSprite;


    bool loadAssets() {
        if (!backgroundTexture.loadFromFile("Assets/UI/Menus/MenusUI1/withoutButtons.jpg")) return false;
        if (!startTexture.loadFromFile("Assets/UI/Menus/MenusUI1/Start.png")) return false;
        if (!exitTexture.loadFromFile("Assets/UI/Menus/MenusUI1/exit.png")) return false;
        return true;
    }

    void setupSprites() {
		sf::Vector2u windowSize = BoxML::Instance()->Resolution();

        backgroundSprite.setTexture(backgroundTexture);
        startSprite.setTexture(startTexture);
        exitSprite.setTexture(exitTexture);

        float scaleX = (float)windowSize.x / backgroundTexture.getSize().x;
        float scaleY = (float)windowSize.y / backgroundTexture.getSize().y;
        backgroundSprite.setScale(scaleX, scaleY);
        backgroundSprite.setPosition(0.0f, 0.0f);

        float desiredButtonWidth = 1000.0f;
        float centerX = (windowSize.x - desiredButtonWidth) / 2.0f;

        float startScale = desiredButtonWidth / startTexture.getSize().x;
        startSprite.setScale(startScale, startScale);
        startSprite.setPosition(centerX, 200.0f);

        float exitScale = desiredButtonWidth / exitTexture.getSize().x;
        exitSprite.setScale(exitScale, exitScale);
        exitSprite.setPosition(centerX, 400.0f);
    }

public:
    Menu() {
        if (!loadAssets()) {
            std::cerr << "ERROR: Failed to load MainMenu assets." << std::endl;
        }
        else {
            setupSprites();
        }
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(backgroundSprite);
        window.draw(exitSprite);
        window.draw(startSprite);
    }

    MenuAction checkClick(sf::RenderWindow& window) override {
        sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);

        if (startSprite.getGlobalBounds().contains(mousePos)) {
            std::cout << "Main Menu -> Start button clicked." << std::endl;
            return GOTO_LEVEL_MENU;
        }
        if (exitSprite.getGlobalBounds().contains(mousePos)) {
            std::cout << "Main Menu -> Exit button clicked." << std::endl;
            return QUIT;
        }
        return NONE;
    }
};