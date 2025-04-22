#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeinfo>

#include "cardDeck.h"
#include "gameGraphics.h"

using namespace std;

int main() {

    GameGraphics *game = new GameGraphics();

    game->testScreen();


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
        }

        game->loopDraw();

    }

    return 0;
}