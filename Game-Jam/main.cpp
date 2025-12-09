#include "BoxML.h"
#include "bfCircle.h"
#include "bfRectangle.h"
#include "InputHandler.hpp"
#include "MenuManager.cpp"
#define WINDOW_WIDTH      1024
#define WINDOW_HEIGHT     768
#define WINDOW_FRAME_RATE 60

#define PIXELS_PER_UNIT   10

int active_menu= 0; // 0 - main menu, 1 - level menu 3-pause menu 4-game over menu 5-win menu 6-none
int main()
{
    const float timeStep = 1.0f / 60.0f; // 1/60 (frames/sec)
    const int32 velocityIterations = 6;
    const int32 positionIterations = 2;

    BoxML boxWorld(WINDOW_WIDTH, WINDOW_HEIGHT, PIXELS_PER_UNIT, timeStep, velocityIterations, positionIterations);

	boxWorld.CreateWorld();
	boxWorld.LoadPositions();
    bool shouldCloseWindow = false;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");
    window.setFramerateLimit(WINDOW_FRAME_RATE);
    InputHandler *inputHandler = new InputHandler(window);
    MenuManager menuManager;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || menuManager.currentState == EXIT)
                window.close();
            //inputHandler->handleInput();
            //if (menuManager.currentState == MAIN_MENU || menuManager.currentState == LEVEL_MENU) {
            //    // Menu state: Only handle mouse clicks for navigation
            //    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            //        menuManager.handleClicks(window);
            //    }
            //}
            //else if (menuManager.currentState == ACTIVE_GAME) {
            //    // Active Game: Handle Pause Input (ESC)
            //    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape) {
            //        std::cout << "ESC pressed. Entering PAUSED state." << std::endl;
            //        // Note: You would typically load a PauseMenu object here
            //        menuManager.setState(PAUSED);
            //    }
            //    // Handle other game inputs (WASD, etc.)
            //}
            //else if (menuManager.currentState == PAUSED) {
            //    // Paused State: Handle Resume Input (ESC)
            //    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape) {
            //        std::cout << "ESC pressed. Resuming ACTIVE_GAME state." << std::endl;
            //        menuManager.setState(ACTIVE_GAME);
            //    }
            //    // Handle pause menu clicks/input if a PauseMenu is displayed
            //}
            if (event.type == sf::Event::KeyPressed) {
                boxWorld.HandleKeyPress(event.key.code);
            }

            // 2. Pass Left Click to place the wall
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    boxWorld.PlacePreviewObject();
                }
            }
            else if (event.mouseButton.button == sf::Mouse::Right) {
                boxWorld.HandleRightClick((sf::Vector2f)sf::Mouse::getPosition(window));
            }
        }
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
       

            boxWorld.UpdatePreviewObject(worldPos);
        boxWorld.Step();
        boxWorld.Render(window);

        //inputHandler->handleInput
       /*
       if (menuManager.currentState == MAIN_MENU || menuManager.currentState == LEVEL_MENU) {
            menuManager.draw(window); // Draw the current menu screen
        }

        else if (menuManager.currentState == ACTIVE_GAME || menuManager.currentState == PAUSED){
        //draw world and player
        }

        //still need to code up pauseMenu.cpp, 
        if (menuManager.currentState == PAUSED) {
            // Draw Pause Menu overlay on top of the game
            sf::RectangleShape overlay(sf::Vector2f(1024.f, 768.f));
            overlay.setFillColor(sf::Color(0, 0, 0, 150)); // Dark overlay
            window.draw(overlay);

            //sf::Text pauseText("PAUSED", sf::Font::getDefaultFont(), 72);
            //pauseText.setFillColor(sf::Color::White);
            //pauseText.setPosition(350.f, 300.f);
            //pausemenu.draw(window);

            // If you had a PauseMenu class, you would draw it here.
        }
        window.display();
        */
        
        
    }

    return 0;
}
