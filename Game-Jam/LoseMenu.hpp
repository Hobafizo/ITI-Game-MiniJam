#pragma once
#include "BaseMenu.hpp"
#include "BoxML.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream> // Required for string building
#include <iomanip> // Required for std::setw and setfill

class LoseMenu : public BaseMenu {
private:
    // --- Animation Assets & State ---
    std::vector<sf::Texture> backgroundFrames; // Stores all 55 frames
    sf::Sprite backgroundSprite;
    sf::Clock animationClock;
    float frameDuration;
    int currentFrameIndex;

    // --- Button Assets ---
    sf::Texture retryTexture;
    sf::Sprite retrySprite;

    sf::Texture quitTexture;
    sf::Sprite quitSprite;
    sf::Vector2f defaultScale;
    bool loadAssets() {
        // 1. Load Button Textures
        if (!retryTexture.loadFromFile("Assets/UI/Menus/MenusUI1/Start2.png")) return false;
        if (!quitTexture.loadFromFile("Assets/UI/Menus/MenusUI1/exit2.png")) return false;

        // 2. Load Background Animation Frames
        backgroundFrames.clear();
        backgroundFrames.reserve(55);

        for (int i = 0; i < 55; ++i) {
            sf::Texture t;
            std::stringstream ss;
            // Constructs path: Assets/frog_frames/frame_000.png ... frame_054.png
            ss << "Assets/frog_frames/frame_"
                << std::setw(3) << std::setfill('0') << i << ".png";

            if (!t.loadFromFile(ss.str())) {
                std::cout << "ERROR: LoseMenu failed to load frame: " << ss.str() << "\n";
                // CRITICAL: We MUST return false if any frame is missing for animation to work,
                // and to avoid using an empty texture.
                return false;
            }

            t.setSmooth(true);
            backgroundFrames.push_back(t);
        }

        if (backgroundFrames.empty()) {
            std::cout << "ERROR: No background frames loaded.\n";
            return false;
        }

        return true;
    }

    void setupSprites() {
        sf::Vector2u windowSize = BoxML::Instance()->Resolution();

        // --- Background Setup ---
        if (!backgroundFrames.empty()) {
            unsigned int frameWidth = backgroundFrames[0].getSize().x;
            unsigned int frameHeight = backgroundFrames[0].getSize().y;

            // SAFETY CHECK 1 & 2: Prevent division by zero if the first frame is empty.
            if (frameWidth > 0 && frameHeight > 0) {
                backgroundSprite.setTexture(backgroundFrames[0]);

                // Calculate scaling to cover the screen
                float scaleX = (float)windowSize.x / frameWidth;
                float scaleY = (float)windowSize.y / frameHeight;
                float finalScale = std::max(scaleX, scaleY);

                backgroundSprite.setScale(finalScale, finalScale);

                // Center the background
                backgroundSprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);
                backgroundSprite.setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
            }
            else {
                std::cout << "WARNING: Background frame size is zero! Cannot display animation.\n";
            }
        }

        // --- Animation Settings ---
        currentFrameIndex = 0;
        frameDuration = 0.04f;
        animationClock.restart();


        // --- Button Setup ---
        float buttonWidth = 300.0f;
        float centerX = (windowSize.x - buttonWidth) * 0.5f;

        // --- Retry Button ---
        unsigned int retryTexWidth = retryTexture.getSize().x;
        float retryScale = 0.0f;

        // SAFETY CHECK 3: Prevent division by zero for retry button width.
        if (retryTexWidth > 0) {
            retryScale = buttonWidth / retryTexWidth;
            retrySprite.setTexture(retryTexture);
            retrySprite.setScale(retryScale, retryScale);
            retrySprite.setPosition(centerX, windowSize.y * 0.4f);
        }
        else {
            std::cout << "WARNING: Retry button texture width is zero! Button will not display correctly.\n";
        }

        // --- Quit Button ---
        unsigned int quitTexWidth = quitTexture.getSize().x;
        float quitScale = 0.0f;

        // SAFETY CHECK 4: Prevent division by zero for quit button width.
        if (quitTexWidth > 0) {
            quitScale = buttonWidth / quitTexWidth;
            quitSprite.setTexture(quitTexture);
            quitSprite.setScale(quitScale, quitScale);
            quitSprite.setPosition(centerX, windowSize.y * 0.6f);
        }
        else {
            std::cout << "WARNING: Quit button texture width is zero! Button will not display correctly.\n";
        }
        defaultScale= retrySprite.getScale();
    }

public:
    LoseMenu() {
        if (!loadAssets()) {
            std::cout << "ERROR: Failed to initialize LoseMenu (Check console for asset load failures).\n";
        }
        else {
            setupSprites();
        }
    }

    void draw(sf::RenderWindow& window) override {
        // --- 1. Update Animation State ---
        // Only update if we have frames to prevent access violation
        if (!backgroundFrames.empty() && animationClock.getElapsedTime().asSeconds() > frameDuration) {
            currentFrameIndex = (currentFrameIndex + 1) % backgroundFrames.size();

            backgroundSprite.setTexture(backgroundFrames[currentFrameIndex], true);
            animationClock.restart();
        }

        // --- 2. Render ---


        window.draw(backgroundSprite);
        window.draw(retrySprite);
        window.draw(quitSprite);


    }

    MenuAction checkClick(sf::RenderWindow& window) override {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos, window.getDefaultView());

        if (retrySprite.getGlobalBounds().contains(mousePos)) {
            std::cout << "LoseMenu -> Retry clicked\n";
            return START_GAME;
        }
        if (quitSprite.getGlobalBounds().contains(mousePos)) {
            std::cout << "LoseMenu -> Quit clicked\n";
            return GOTO_MAIN_MENU;
        }
        return NONE;
    }
    void checkHover(sf::Vector2f mousePos) {
        if(retrySprite.getScale()!=defaultScale){
			retrySprite.setScale(defaultScale);
		}
        if (quitSprite.getScale() != defaultScale) {
            quitSprite.setScale(defaultScale);
        }
        if (retrySprite.getGlobalBounds().contains(mousePos)) {
            retrySprite.setScale(retrySprite.getScale().x * 1.1f, retrySprite.getScale().y * 1.1f);
        }
        if (quitSprite.getGlobalBounds().contains(mousePos)) {
            quitSprite.setScale(quitSprite.getScale().x * 1.1f, quitSprite.getScale().y * 1.1f);
        }

    }
};
