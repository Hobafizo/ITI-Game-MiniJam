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
#include "Level4.hpp"
#include "StartupVideo.h"

#include "Level4.hpp"
int main()
{
    vector <LevelData> levels = { Level1Data, Level2Data, Level3Data, Level4Data };
    int numLevels = levels.size();
    int curLevel= 0;
    const float timeStep = 1.0f / 60.0f; // 1/60 (frames/sec)
    const int32 velocityIterations = 6;
    const int32 positionIterations = 2;

	sf::Color defaultColor(0x000100);

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
#ifdef WINDOW_FULL_SCREEN
		"My Game", sf::Style::Fullscreen);
#else
		"My Game");
#endif

    window.setFramerateLimit(WINDOW_FRAME_RATE);
	window.clear(defaultColor);
	window.display();

    BoxML boxWorld(WINDOW_WIDTH, WINDOW_HEIGHT, PIXELS_PER_UNIT, timeStep, velocityIterations, positionIterations);
    BoxML::setInstance(&boxWorld);

	boxWorld.CreateWorld();
	boxWorld.LoadPositions();

    LevelManager levelMgr(boxWorld);

    bool shouldCloseWindow = false;

    InputHandler *inputHandler = new InputHandler(window);
    MenuManager menuManager;

#ifndef NO_INTRO
	StartupVideo introVideo(boxWorld.Resolution());
	introVideo.Load();
	introVideo.Start();

#else
	menuManager.playMainMusic();

#endif

    Hud hud;
    while (window.isOpen())
    {
        sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				levelMgr.unloadLevel();
				window.close();
			}

			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
			{
				if (menuManager.currentState == ACTIVE_GAME)
				{
					// Active Game: Handle Pause Input (ESC)
					//levelMgr.loadLevel(Level3Data);
					std::cout << "ESC pressed. Entering PAUSED state." << std::endl;

					menuManager.showPauseMenu();
					menuManager.setState(PAUSED);
					boxWorld.SetRenderState(WorldRenderState::Paused);

					// Handle other game inputs (WASD, etc.)
				}
				else if (menuManager.currentState == PAUSED) {
					std::cout << "ESC pressed. Resuming ACTIVE_GAME state." << std::endl;
					menuManager.setState(ACTIVE_GAME);
					boxWorld.SetRenderState(WorldRenderState::Running);
				}
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

        if (menuManager.currentState == EXIT) {
            levelMgr.unloadLevel();
            window.close();
        }

        if (boxWorld.RenderState() == WorldRenderState::Lose) {
            menuManager.showLoseMenu();
            boxWorld.StartLoserMusic();
            boxWorld.SetRenderState(WorldRenderState::Paused);
            levelMgr.unloadLevel();
            menuManager.handleHover();
        }
        if (boxWorld.RenderState() == WorldRenderState::Win) {

            menuManager.showWinMenu();
            if (curLevel < numLevels) { curLevel++; }
            levelMgr.unloadLevel();

            boxWorld.SetRenderState(WorldRenderState::Paused);
            boxWorld.StartWinMusic();
            menuManager.handleHover();
        }
        if (menuManager.currentState == MAIN_MENU)
        {
            levelMgr.unloadLevel();
        }

        else if (menuManager.currentState == LOADING_LEVEL)
        {
            levelMgr.loadLevel(levels[curLevel]);
            boxWorld.StopWinMusic();
            boxWorld.StopLoserMusic();
            menuManager.setState(ACTIVE_GAME);
            boxWorld.SetRenderState(WorldRenderState::Running);
        }

        if (menuManager.currentState == ACTIVE_GAME)
        {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

			boxWorld.UpdatePreviewObject(worldPos);
			boxWorld.Step();

			window.clear(defaultColor);
            boxWorld.Render(window);
			window.display();
        }

#ifndef NO_INTRO
		else if (menuManager.currentState == INTRO_VIDEO)
		{
			if (!introVideo.Ended())
			{
				window.clear(defaultColor);
				introVideo.Draw(window);
				window.display();
			}
			else
			{
				window.clear(defaultColor);
				window.display();

				menuManager.setState(MAIN_MENU);
				menuManager.playMainMusic();
			}
		}
#endif

        //inputHandler->handleInput
       
        else
        {
            window.clear(defaultColor);

            if (menuManager.currentState == MAIN_MENU
				|| menuManager.currentState == LEVEL_MENU
				|| menuManager.currentState == LOADING_LEVEL || menuManager.currentState == LOSE_MENU ||menuManager.currentState==WIN_MENU
				)
			{
                menuManager.draw(window); // Draw the current menu screen

                menuManager.handleHover();
                if (menuManager.currentState != LOSE_MENU)
                {
                    boxWorld.StopLoserMusic();
                }
                if (menuManager.currentState != WIN_MENU) {
                    boxWorld.StopWinMusic();
                }
            }

            //still need to code up pauseMenu.cpp, 
            else if (menuManager.currentState == PAUSED)
			{
				boxWorld.Render(window);
                menuManager.draw(window); // Draw the pause menu

                menuManager.handleHover();
            }

            window.display();
        }
    }

    return 0;
}
