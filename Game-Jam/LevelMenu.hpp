#include "BaseMenu.hpp"
#include "BoxML.h"

class LevelMenu : public BaseMenu {
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture level1Texture;
    sf::Sprite level1Sprite;
    sf::Texture exitTexture; // Reused for Back button
    sf::Sprite exitSprite;
    sf::Vector2f defaultScale;
    bool loadAssets() {
        
        if (!level1Texture.loadFromFile("Assets/UI/Menus/MenusUI1/Start2.png")) return false;
        if (!backgroundTexture.loadFromFile("Assets/UI/Menus/MenusUI1/withoutButtons.jpg")) return false;
        if (!exitTexture.loadFromFile("Assets/UI/Menus/MenusUI1/Back.png")) return false;
        return true;
    }

    void setupSprites() {
        sf::Vector2u windowSize = BoxML::Instance()->Resolution();

        backgroundSprite.setTexture(backgroundTexture);
        level1Sprite.setTexture(level1Texture);
        exitSprite.setTexture(exitTexture);

        float scaleX = (float)windowSize.x / backgroundTexture.getSize().x;
        float scaleY = (float)windowSize.y / backgroundTexture.getSize().y;
        backgroundSprite.setScale(scaleX, scaleY);
        backgroundSprite.setPosition(0.0f, 0.0f);

        float desiredButtonWidth = 300.f;
        float centerX = (windowSize.x - desiredButtonWidth) / 2.0f;

        float startScale = desiredButtonWidth / level1Texture.getSize().x;
        level1Sprite.setScale(startScale, startScale);
        level1Sprite.setPosition(centerX, 300.0f);

        float exitScale = desiredButtonWidth / exitTexture.getSize().x;
        exitSprite.setScale(exitScale, exitScale);
        exitSprite.setPosition(centerX, 600.0f);
        defaultScale = level1Sprite.getScale();
    }

public:
    LevelMenu() {
        if (!loadAssets()) {
            std::cerr << "ERROR: Failed to load LevelMenu assets." << std::endl;
        }
        else {
            setupSprites();
        }
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(backgroundSprite);
        window.draw(exitSprite);
        window.draw(level1Sprite);
    }

    MenuAction checkClick(sf::RenderWindow& window) override {
        sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);

        if (level1Sprite.getGlobalBounds().contains(mousePos)) {
            std::cout << "Level Menu -> Level 1 button clicked. Start Game." << std::endl;
            return START_GAME;
            
        }

        if (exitSprite.getGlobalBounds().contains(mousePos)) {
            std::cout << "Level Menu -> Exit button clicked. Go to Main Menu." << std::endl;
            return GOTO_MAIN_MENU;
        }
        return NONE;
    }
    void checkHover(sf::Vector2f mousePos) {
        if (level1Sprite.getScale() != defaultScale) level1Sprite.setScale(defaultScale);
        if (exitSprite.getScale() != defaultScale) exitSprite.setScale(defaultScale);
        if (level1Sprite.getGlobalBounds().contains(mousePos)) {
            level1Sprite.setScale(level1Sprite.getScale().x * 1.1f, level1Sprite.getScale().y * 1.1f);
        }
        if (exitSprite.getGlobalBounds().contains(mousePos)) {
            exitSprite.setScale(exitSprite.getScale().x * 1.1f, exitSprite.getScale().y * 1.1f);
        }

    }
};