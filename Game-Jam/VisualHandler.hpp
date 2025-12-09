#pragma once
#include "BoxML.h"

#include <SFML/Graphics.hpp>
#include <iostream>


//this should be integrated into the menus and object spawner later/animator
class VisualHandler {
private:
    // 2. Current Window Size (The actual size of the player's window)
    sf::Vector2u currentWindowSize;

    // 3. Scaling Factors
    float scaleX = 1.0f;
    float scaleY = 1.0f;

public:
    // Constructor
    VisualHandler(const sf::Vector2u& initialWindowSize) {
        updateWindowSize(initialWindowSize);
    }

    // Call this whenever the window is resized
    void updateWindowSize(const sf::Vector2u& newWindowSize) {
        currentWindowSize = newWindowSize;

        auto referenceSize = BoxML::Instance()->Resolution();

        // Calculate uniform scaling factors relative to the reference size
        scaleX = (float)currentWindowSize.x / referenceSize.x;
        scaleY = (float)currentWindowSize.y / referenceSize.y;
    }

    // --- Core Conversion Methods ---

    // Converts a single coordinate (e.g., designed position) to the scaled position
    float scaleCoordinate(float designCoord, char axis) const {
        if (axis == 'x' || axis == 'X') {
            return designCoord * scaleX;
        }
        else if (axis == 'y' || axis == 'Y') {
            return designCoord * scaleY;
        }
        return designCoord; // Return original if axis is invalid
    }

    // Converts a designed position vector to the scaled screen position
    sf::Vector2f getScaledPosition(const sf::Vector2f& designPosition) const {
        return sf::Vector2f(designPosition.x * scaleX, designPosition.y * scaleY);
    }

    // Calculates the correct scale factor for an object to achieve a DESIRED PIXEL WIDTH 
    // (e.g., a button should always be 200px wide in the current window)
    float getButtonScale(float desiredWidth, const sf::Texture& texture) const {
        // Calculate the scale needed to achieve the desired width in the current window
        return desiredWidth / texture.getSize().x;
    }

    // Calculates the correct scale factor for an object to achieve a DESIRED DESIGN WIDTH 
    // (e.g., a button should be 200 units wide in the design space)
    float getScaleByDesignWidth(float designWidth, const sf::Texture& texture) const {
        // First, scale the desired design width to the current pixel width
        float currentDesiredWidth = designWidth * scaleX;

        // Then, calculate the scale factor based on the texture's original pixel width
        return currentDesiredWidth / texture.getSize().x;
    }

    // --- Utility Methods ---

    // Returns the calculated scaling factors (e.g., to scale the background)
    sf::Vector2f getScaleFactors() const {
        return sf::Vector2f(scaleX, scaleY);
    }

    // Calculates the center position for an object given its desired design width
    float getCenteredX(float designWidth) const {
        float currentDesiredWidth = designWidth * scaleX;
        return (currentWindowSize.x - currentDesiredWidth) / 2.0f;
    }

    sf::Vector2u getReferenceSize() const {
        return BoxML::Instance()->Resolution();
    }
};