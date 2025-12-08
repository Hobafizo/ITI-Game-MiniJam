#include <SFML/Graphics.hpp>
#include <iostream>
// "Assets/UI/Menus/LevelMenu/"
//"Assets/UI/Menus/MainMenu/"

class Menu {
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite; 

    sf::Texture startTexture;
    sf::Sprite startSprite;    
    
    sf::Texture exitTexture;
    sf::Sprite exitSprite;
public:
	
	Menu() {
		if (!loadAssets()) {
			// Handle error, e.g., throw an exception or log a message
			std::cerr << "ERROR: Failed to load menu assets." << std::endl;
		}
	}
    bool loadAssets() {
        // Load the background image from the assets folder
        if (!backgroundTexture.loadFromFile("Assets/UI/Menus/MainMenu/MenuBackground.jpeg")) {
            // If loading fails, return false
            return false;
        }    
        if (!startTexture.loadFromFile("Assets/UI/Menus/MainMenu/Start.jpeg")) {
            // If loading fails, return false
            return false;
        }    
        if (!exitTexture.loadFromFile("Assets/UI/Menus/MainMenu/Exit.jpg")) {
            // If loading fails, return false
            return false;
        }

        // Add more asset loading here, e.g., button images
        // if (!buttonTexture.loadFromFile("assets/button.png")) {
        //     return false;
        // }

        return true; // All assets loaded successfully
    }

    // A function to configure the sprites (position, scale, etc.)
    void setupSprites() {
        // Link the texture to the sprite
        backgroundSprite.setTexture(backgroundTexture);
        startSprite.setTexture(startTexture);
        exitSprite.setTexture(exitTexture);

        startSprite.setPosition(350.0f, 200.0f);
        exitSprite.setPosition(350.0f, 300.0f);

        // Example: Scale the background if needed
        // backgroundSprite.setScale(0.5f, 0.5f);

        // Example: Position the sprite
        // backgroundSprite.setPosition(0.0f, 0.0f);
    }

    // A function to draw the menu to the window
    void draw(sf::RenderWindow& window) {
        window.draw(backgroundSprite);
        window.draw(exitSprite);
        window.draw(startSprite);
        // Draw other elements like buttons
        // window.draw(playButtonSprite);
    }
    void checkClick() {
        // Implement click detection logic here
        if (startSprite.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y))) {
            std::cout << "Start button clicked!" << std::endl;
            //switch to level menu
        };
        if (exitSprite.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y))) {
			std::cout << "Exit button clicked!" << std::endl;
		};
    }
};