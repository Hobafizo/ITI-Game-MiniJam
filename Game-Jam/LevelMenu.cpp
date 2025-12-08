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
public:
	LevelMenu() {
		if (!loadAssets()) {
			// Handle error, e.g., throw an exception or log a message
			std::cerr << "ERROR: Failed to load menu assets." << std::endl;
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
		//level2Sprite.setTexture(level2Texture);

		level1Sprite.setPosition(350.0f, 200.0f);
		//level2Sprite.setPosition(350.0f, 300.0f);

		exitSprite.setTexture(exitTexture);
		exitSprite.setPosition(350.0f, 400.0f);

		// Example: Scale the background if needed
		// backgroundSprite.setScale(0.5f, 0.5f);

		// Example: Position the sprite
		// backgroundSprite.setPosition(0.0f, 0.0f);
	}
};