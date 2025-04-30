#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeinfo>

#include "cardDeck.h"
#include "gameGraphics.h"

using namespace std;

int main() {
    // GameGraphics class holds all visual elements and controls interactible elements that call other functions
    GameState *state = new GameState();
    GameGraphics *game = new GameGraphics(state);

    game->titleScreen();

    // first of 2 nested loops constituting the event loop
    while (game->window.isOpen()) {
        if (state->playingGame) {
            state->checkState();
            game->matchGameState();
        }

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
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // this function is where we implement interactible buttons
                    game->onClick();
                }
            }
        }

        // draw all drawable objects
        game->loopDraw();
    }

    return 0;
}