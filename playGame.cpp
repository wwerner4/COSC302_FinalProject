#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeinfo>

#include "cardDeck.h"
#include "gameGraphics.h"

using namespace std;

int main() {
    // create GameState and GameGraphics objects
    GameState *state = new GameState();
    GameGraphics *game = new GameGraphics(state);
    state->game = game;         // store pointer to game in state

    // show title screen to user
    game->titleScreen();

    // we must continually draw new frames while the game is running
    while (game->window.isOpen()) {

        // if the game has started, determine the next game action and display an updated game scene
        if (state->playingGame) {
            state->checkState();
            game->playScreen();
        }

        // handle all user input
        sf::Event event;
        while (game->window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                game->closeWindow();
            }

            // if the window is resized, we rescale drawn elements
            if (event.type == sf::Event::Resized) {
                game->resizeWindow(event);
            }

            // if a mouse click is detected, check if we need to performa button's action
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    game->onClick();
                }
            }
        }

        // draw all drawable objects
        game->loopDraw();
    }

    return 0;
}