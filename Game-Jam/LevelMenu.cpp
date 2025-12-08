#include <SFML/Graphics.hpp>
#include <iostream>
class LevelMenu {
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;

	sf::Texture level1Texture;
	sf::Sprite level1Sprite;

	sf::Texture level2Texture;
	sf::Sprite level2Sprite;

	sf::Texture exitTexture;
	sf::Sprite exitSprite;

	sf::Vector2u windowSize = sf::Vector2u(1024, 768);
public:
	LevelMenu() {
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
		if (!backgroundTexture.loadFromFile("Assets/UI/Menus/LevelMenu/LevelMenuBackground.jpeg")) {
			// If loading fails, return false
			return false;
		}
		if (!level1Texture.loadFromFile("Assets/UI/Menus/LevelMenu/Level1.jpeg")) {
			// If loading fails, return false
			return false;
		}
		if (!exitTexture.loadFromFile("Assets/UI/Menus/LevelMenu/Exit.jpg")) {
			// If loading fails, return false
			return false;
		}
		//if (!level2Texture.loadFromFile("Assets/UI/Menus/LevelMenu/Level2.png")) {
		//	// If loading fails, return false
		//	return false;
		//}

		// Add more asset loading here, e.g., button images
		// if (!buttonTexture.loadFromFile("assets/button.png")) {
		//     return false;
		// }

		return true; // All assets loaded successfully
	}
	void setupSprites() {
		// Link the texture to the sprite
		backgroundSprite.setTexture(backgroundTexture);
		level1Sprite.setTexture(level1Texture);
		exitSprite.setTexture(exitTexture);
		std::cout << "Level 1 Scale: " << level1Sprite.getScale().x << std::endl;
		// --- 1. Fix Background Scaling ---
		// Calculate scale factors to fill the 1024x768 window
		float scaleX = (float)windowSize.x / backgroundTexture.getSize().x;
		float scaleY = (float)windowSize.y / backgroundTexture.getSize().y;
		backgroundSprite.setScale(scaleX, scaleY);
		backgroundSprite.setPosition(0.0f, 0.0f); // Place at top-left

		// --- 2. Fix Button Centering and Scaling ---
		// Define a standard width for buttons (e.g., 300 pixels)
		float desiredButtonWidth = 200.f;

		// Calculate the horizontal center position
		float centerX = (windowSize.x - desiredButtonWidth) / 2.0f;

		// Calculate the scale factor for the Start button
		float startScale = desiredButtonWidth / level1Texture.getSize().x;
		level1Sprite.setScale(startScale, startScale);
		std::cout << "Level 1 Scale: " << level1Sprite.getScale().x << std::endl;
		// Position Start Button (Centered horizontally)
		// Vertical position remains 200.0f for now
		level1Sprite.setPosition(centerX, 200.0f);

		// Calculate the scale factor for the Exit button
		float exitScale = desiredButtonWidth / exitTexture.getSize().x;
		exitSprite.setScale(exitScale, exitScale);

		// Position Exit Button (Centered horizontally)
		// Vertical position remains 300.0f for now
		exitSprite.setPosition(centerX, 400.0f);
	}
	void checkClick() {
		// Implement click detection logic here
		if (level1Sprite.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y))) {
			std::cout << "Start button clicked!" << std::endl;
			//switch to level menu
		};
		if (exitSprite.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y))) {
			std::cout << "Exit button clicked!" << std::endl;
		};
	}
	void draw(sf::RenderWindow& window) {
		window.draw(backgroundSprite);
		window.draw(exitSprite);
		window.draw(level1Sprite);
		// Draw other elements like buttons
		// window.draw(playButtonSprite);
	}

};