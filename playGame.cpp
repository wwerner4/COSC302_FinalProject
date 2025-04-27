#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeinfo>

#include "cardDeck.h"
#include "gameGraphics.h"

using namespace std;

int main() {

    GameGraphics *game = new GameGraphics();

    game->titleScreen();

    // first of 2 nested loops constituting the event loop
    while (game->window.isOpen()) {

        // second of 2 nested loops constituting the event loop
        sf::Event event;
        while (game->window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                game->closeWindow();
            }

            if (event.type == sf::Event::Resized) {
                game->resizeWindow(event);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                //sf::Vector2i localMousePosition = sf::Mouse::getPosition(game->window);

                if (event.mouseButton.button == sf::Mouse::Left) {
                    //cout << localMousePosition.x << ' ' << localMousePosition.y << endl;
                    game->onClick();
                }
            }
        }

        game->loopDraw();

    }

    return 0;
}