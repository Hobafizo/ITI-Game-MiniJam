#include "BaseMenu.hpp"

class LevelMenu : public BaseMenu {
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture level1Texture;
    sf::Sprite level1Sprite;
    sf::Texture exitTexture; // Reused for Back button
    sf::Sprite exitSprite;
    sf::Vector2u windowSize = sf::Vector2u(1920, 1080);

    bool loadAssets() {
        
        if (!level1Texture.loadFromFile("Assets/UI/Menus/MenusUI1/Start.png")) return false;
        if (!backgroundTexture.loadFromFile("Assets/UI/Menus/MenusUI1/withoutButtons.jpg")) return false;
        if (!exitTexture.loadFromFile("Assets/UI/Menus/MenusUI1/Back.png")) return false;
        return true;
    }

    void setupSprites() {
        backgroundSprite.setTexture(backgroundTexture);
        level1Sprite.setTexture(level1Texture);
        exitSprite.setTexture(exitTexture);

        float scaleX = (float)windowSize.x / backgroundTexture.getSize().x;
        float scaleY = (float)windowSize.y / backgroundTexture.getSize().y;
        backgroundSprite.setScale(scaleX, scaleY);
        backgroundSprite.setPosition(0.0f, 0.0f);

        float desiredButtonWidth = 1000.f;
        float centerX = (windowSize.x - desiredButtonWidth) / 2.0f;

        float startScale = desiredButtonWidth / level1Texture.getSize().x;
        level1Sprite.setScale(startScale, startScale);
        level1Sprite.setPosition(centerX, 200.0f);

        float exitScale = desiredButtonWidth / exitTexture.getSize().x;
        exitSprite.setScale(exitScale, exitScale);
        exitSprite.setPosition(centerX, 400.0f);
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
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);

        if (level1Sprite.getGlobalBounds().contains(worldMousePos)) {
            std::cout << "Level Menu -> Level 1 button clicked. Start Game." << std::endl;
            return START_GAME;
        }

        if (exitSprite.getGlobalBounds().contains(worldMousePos)) {
            std::cout << "Level Menu -> Exit button clicked. Go to Main Menu." << std::endl;
            return GOTO_MAIN_MENU;
        }
        return NONE;
    }
};