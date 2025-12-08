#include "BoxML.h"
#include "bfCircle.h"
#include "bfRectangle.h"
#include "InputHandler.hpp"
#include "MainMenu.cpp"
#include "LevelMenu.cpp"
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
    

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");
    window.setFramerateLimit(WINDOW_FRAME_RATE);
    InputHandler *inputHandler = new InputHandler(window);
    Menu mainMenu;
    LevelMenu levelMenu;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            //inputHandler->handleInput();
            if (active_menu == 0) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    mainMenu.checkClick();
                }
            }

        }

       /* boxWorld.Step();
        boxWorld.Render(window);*/
        //inputHandler->handleInput
        window.clear();
        levelMenu.draw(window);
        window.display();
    }

    return 0;
}
