#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
class InputHandler {
	sf::RenderWindow &window;
	private:
		
		
		void KeyPressed(sf::Keyboard::Key key) {
			cout<< sf::Keyboard::isKeyPressed(key);
		}

		void MouseButtonPressed(sf::Mouse::Button button) {
			getMousePosition();
			
		}

		void getMousePosition() {
			  sf::Mouse::getPosition(window);
			 cout<< sf::Mouse::getPosition(window).x<< sf::Mouse::getPosition(window).y;
			 cout << "hello";
		}
	public:
		InputHandler(sf::RenderWindow& win) : window(win) { }
		

		void handleInput() {
			KeyPressed(sf::Keyboard::Num1); // Example key
			MouseButtonPressed(sf::Mouse::Left); // Example button
			
		}
		
};