#include "def.h"
#include "Hud.hpp"
#include "BoxML.h"
#include "bfCircle.h"
#include "bfRectangle.h"
#include "InputHandler.hpp"
#include "MenuManager.hpp"
#include "LevelManager.hpp"
#include "PauseMenu.hpp"
#include "Level1.hpp"
#include "Level2.hpp"
#include "Level3.hpp"

int active_menu= 0; // 0 - main menu, 1 - level menu 3-pause menu 4-game over menu 5-win menu 6-none
int main()
{
    const float timeStep = 1.0f / 60.0f; // 1/60 (frames/sec)
    const int32 velocityIterations = 6;
    const int32 positionIterations = 2;

    //sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "My Game", sf::Style::Fullscreen);
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "My Game");
    window.setFramerateLimit(WINDOW_FRAME_RATE);

    BoxML boxWorld(WINDOW_WIDTH, WINDOW_HEIGHT, PIXELS_PER_UNIT, timeStep, velocityIterations, positionIterations);
    BoxML::setInstance(&boxWorld);

	boxWorld.CreateWorld();
	boxWorld.LoadPositions();

    LevelManager levelMgr(boxWorld);
    

    bool shouldCloseWindow = false;

    InputHandler *inputHandler = new InputHandler(window);
    MenuManager menuManager;

    Hud hud;
    PauseMenu pausemenu;
    levelMgr.loadLevel(Level3Data); // Load level 
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || menuManager.currentState == EXIT) {
                levelMgr.unloadLevel();
                window.close();
            }

            if (menuManager.currentState == ACTIVE_GAME) {
                // Active Game: Handle Pause Input (ESC)
                //levelMgr.loadLevel(Level3Data);
                if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape) {
                    std::cout << "ESC pressed. Entering PAUSED state." << std::endl;

                    menuManager.setState(PAUSED);
                }
                // Handle other game inputs (WASD, etc.)
            }
            else if (menuManager.currentState == PAUSED) {

                if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape) {
                    std::cout << "ESC pressed. Resuming ACTIVE_GAME state." << std::endl;
                    menuManager.setState(ACTIVE_GAME);
                }
                
                // Handle pause menu clicks/input if a PauseMenu is displayed
            }

            if (event.type == sf::Event::KeyPressed) {
                if (menuManager.currentState == ACTIVE_GAME) {
                    
                    boxWorld.HandleKeyPress(event.key.code);
                    //levelMgr.loadLevel(Level1Data);
                }
            }

            // 2. Pass Left Click to place the wall
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (menuManager.currentState != ACTIVE_GAME)
                        menuManager.handleClicks(window);
                    else
                        boxWorld.PlacePreviewObject();
                }
            }
            else if (event.mouseButton.button == sf::Mouse::Right) {
                boxWorld.HandleRightClick((sf::Vector2f)sf::Mouse::getPosition(window));
            }
        }
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
       

        if (menuManager.currentState == ACTIVE_GAME)
        {
            boxWorld.UpdatePreviewObject(worldPos);
            boxWorld.Step();
            boxWorld.Render(window);
        }

        //inputHandler->handleInput
       
        else
        {
            window.clear();

            if (menuManager.currentState == MAIN_MENU || menuManager.currentState == LEVEL_MENU) {
                levelMgr.unloadLevel(); // Unload any active level
                menuManager.draw(window); // Draw the current menu screen
            }

            //still need to code up pauseMenu.cpp, 
            else if (menuManager.currentState == PAUSED) {
                menuManager.showPauseMenu();
                menuManager.draw(window); // Draw the pause menu

            }

            window.display();
        }
    }

    return 0;
}
