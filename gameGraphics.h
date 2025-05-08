#ifndef GAMEGRAPHICS_H
#define GAMEGRAPHICS_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "cardDeck.h"
#include "CardEvaluation/handEvaluator.h"

class GameGraphics;

// GameState controls the gameplay loop
class GameState {
   public:
    GameState();

    bool playingGame;           // true if the player has clicked "Play Game" in the start menu. If true, we start the game loop

    int gameStage;              // at what point in the hand is the game? (these stages of the game are called "turns", or at least that is how we will refer to them in this documentation)
    int pot;                    // current total bet this hand
    int smallBlind;             // smaller of 2 required bets at the start of play ("antes")
    int bigBlind;               // larger ante
    int dealer;                 // the "dealer" player. This role shifts around the table, determining who pays the blinds and who bets first in a hand
    int numPlayers;
    int checkBet;               // the current largest bet by an individual player. This bet must be matched by all other players for them to stay in the game
    int turn;                   // the player whose turn it is to bet/fold currently

    CardDeck *deck;             // the deck of cards from which the players' hands and table cards come from
    GameGraphics *game;         // a pointer to the GameGraphics object that controls the GUI. This pointer allows the GameState class to directly call GUI functions if necessary

    std::vector<int> bets;                  // bets[0] == user's total bet this hand, stores bets for all players
    std::vector<int> chips;                 // stores players' chips
    std::vector<bool> folds;                // folds[player] == true if that player has folded this hand (or if the player is out of chips at the start of the hand)
    std::vector<bool> hasBet;               // hasBet[player] == true if the player has bet at least once this turn
    std::vector<std::vector<int>> hands;    // stores each player's hand
    std::vector<int> table;                 // stores the table cards

    std::vector<bool> bustPlayers;          // bustPlayers[player] == true if the player is out of chips at the start of a hand (they are out of the game)

    void bet(int);                          // controls cpu player betting (the user bets in a different way, with the GUI)
    void resetBets();                       // reset the bets vector for the next stage of the game
    void nextPlayer();                      // increment the turn int

    void gameBegin();                       // setup a fresh hand
    void checkState();                      // determine what action should occur next in the game
    void newStage();                        // if we move on to the next stage of the game, set up the new stage
    int determineWinner();                  // determine who won the hand

    int playerBet;                          // the value that the player bets
    bool playerHasBet;                      // playerHasBet == true if the player has pressed the check/call/raise button
    int minBet;                             // the minimum value that the player can bet and stay in the game
};


// GameGraphics handles the drawing of the GUI and player interaction with the game
class GameGraphics {

   public:
    GameGraphics(GameState *);
    ~GameGraphics();

    // object representing the GUI window
    sf::RenderWindow window;

    // functions that create drawable elements for different scenes/events in the game
    void titleScreen();                         // title scene
    void playScreen();                          // main game scene
    void endScreen();                           // add text describing the winner's hand at the end of a hand
    void gameOver();                            // show a message about the end of the game when all but 1 player are out of chips

    // functions to handle GUI functionality
    void loopDraw();                            // draw each frame of the game
    void resizeWindow(sf::Event);               // rescale drawable elements on window rescale
    void closeWindow();                         // delete drawn objects and close the game window
    void onClick();                             // handle user interaction via the mouse   
    void clearWindow();                         // delete drawn objects

   private:

    void incBet();                              // give functionality to player's increase bet button
    void decBet();                              // give functionality to player's decrease bet button
    void userBet();                             // give functionality to player's check/call/raise button
    void userFold();                            // give functionality to player's fold button

    sf::Vector2u currentWindowSize;             // used in the resizeWindow() function
    sf::Font font;                              // store the font used for all sf::Text objects

    GameState *state;                           // stores a pointer to the GameState object. This allows us to retrieve game info for display by the GameGraphics object

    // store texts, shapes, sprites, textures, and vertexArrays as well as drawnElements. This is double storage, but sf::Drawable is a virtual class so we cannot call all of an object's class functions from drawnElements
    std::vector<sf::Text *> texts;
    std::vector<sf::Shape *> shapes;
    std::vector<sf::Sprite *> sprites;
    std::vector<sf::Texture *> textures;
    std::vector<sf::VertexArray *> vertexArrays;
    std::vector<std::vector<sf::Drawable *>> drawnElements;  // 2D to allow element grouping (i.e., player's hand might be elements[2][0 through 5]). The order of elements in drawnElements is the order they are drawn in the game

    std::map<sf::Shape *, char> interactables;      // store what drawn elements are clickable and what action they trigger on click
};

#endif