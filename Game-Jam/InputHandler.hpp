#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
class InputHandler {
	sf::RenderWindow &window;
	private:
		
		
		void isKeyPressed(sf::Keyboard::Key key) {
			cout<< sf::Keyboard::isKeyPressed(key);
		}

		void isMouseButtonPressed(sf::Mouse::Button button) {
			cout << "right";
		}

		void getMousePosition() {
			  sf::Mouse::getPosition(window);
			 cout<< sf::Mouse::getPosition(window).x<< sf::Mouse::getPosition(window).y;
			 cout << "hello";
		}
	public:
		InputHandler(sf::RenderWindow& win) : window(win) { }
		

		void handleInput() {
			isKeyPressed(sf::Keyboard::A); // Example key
			isMouseButtonPressed(sf::Mouse::Left); // Example button
			getMousePosition();
		}
		
};