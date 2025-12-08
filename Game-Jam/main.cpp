#include "BoxML.h"
#include "bfCircle.h"
#include "bfRectangle.h"
#include "InputHandler.hpp"
#define WINDOW_WIDTH      1024
#define WINDOW_HEIGHT     768
#define WINDOW_FRAME_RATE 60

#define PIXELS_PER_UNIT   10

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
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            //inputHandler->handleInput();
        }

        boxWorld.Step();
        boxWorld.Render(window);
        inputHandler->handleInput();
    }

    return 0;
}
