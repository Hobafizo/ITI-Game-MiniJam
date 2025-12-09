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
    sf::Vector2u windowSize = sf::Vector2u(1024, 768);
public:
	
	Menu() {
		if (!loadAssets()) {
			// Handle error, e.g., throw an exception or log a message
			std::cerr << "ERROR: Failed to load menu assets." << std::endl;
        }
        else {
            setupSprites();
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

        // --- 1. Fix Background Scaling ---
        // Calculate scale factors to fill the 1024x768 window
        float scaleX = (float)windowSize.x / backgroundTexture.getSize().x;
        float scaleY = (float)windowSize.y / backgroundTexture.getSize().y;
        backgroundSprite.setScale(scaleX, scaleY);
        backgroundSprite.setPosition(0.0f, 0.0f); // Place at top-left

        // --- 2. Fix Button Centering and Scaling ---
        // Define a standard width for buttons (e.g., 300 pixels)
        float desiredButtonWidth = 300.0f;

        // Calculate the horizontal center position
        float centerX = (windowSize.x - desiredButtonWidth) / 2.0f;

        // Calculate the scale factor for the Start button
        float startScale = desiredButtonWidth / startTexture.getSize().x;
        startSprite.setScale(startScale, startScale);

        // Position Start Button (Centered horizontally)
        // Vertical position remains 200.0f for now
        startSprite.setPosition(centerX, 200.0f);

        // Calculate the scale factor for the Exit button
        float exitScale = desiredButtonWidth / exitTexture.getSize().x;
        exitSprite.setScale(exitScale, exitScale);

        // Position Exit Button (Centered horizontally)
        // Vertical position remains 300.0f for now
        exitSprite.setPosition(centerX, 400.0f);
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