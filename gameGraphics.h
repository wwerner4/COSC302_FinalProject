#ifndef GAMEGRAPHICS_H
#define GAMEGRAPHICS_H

#include <SFML/Graphics.hpp>
#include <vector>

#include "cardDeck.h"
#include "CardEvaluation/handEvaluator.h"

class GameGraphics;

class GameState {
   public:
    GameState();

    bool playingGame;

    int gameStage;
    int pot;
    int smallBlind;
    int bigBlind;
    int dealer;
    int numPlayers = 4;
    int checkBet;
    int turn;

    CardDeck *deck;
    GameGraphics *game;

    std::vector<int> bets;  // [0] == player
    std::vector<int> chips;
    std::vector<bool> folds;
    std::vector<bool> hasBet;
    std::vector<std::vector<int>> hands;
    std::vector<int> table;

    std::vector<bool> bustPlayers;

    void bet(int);
    void resetBets();
    void nextPlayer();

    void gameBegin();
    void checkState();
    void newStage();
    int determineWinner();

    int playerBet;
    bool playerHasBet;
    int minBet;
};

class GameGraphics {
   public:
    GameGraphics(GameState *);
    ~GameGraphics();

    sf::RenderWindow window;

    void titleScreen();
    void playScreen();
    void testScreen();
    void endScreen();
    void gameOver();

    void matchGameState();

    void loopDraw();
    void resizeWindow(sf::Event);
    void closeWindow();
    void onClick();
    void eventHandler(sf::Event);

   private:
    void clearWindow();
    void incBet();
    void decBet();
    void userBet();
    void userFold();

    sf::Vector2u currentWindowSize;
    sf::Font font;

    GameState *state;

    // I'm storing texts, shapes, sprites, and textures as well as drawnElements. This is double storage, but sf::Drawable is a virtual class so I can't call all of an object's functions from drawnElements
    std::vector<sf::Text *> texts;
    std::vector<sf::Shape *> shapes;
    std::vector<sf::Sprite *> sprites;
    std::vector<sf::Texture *> textures;
    std::vector<sf::VertexArray *> vertexArrays;
    std::map<sf::Shape *, char> interactables;
    std::vector<std::vector<sf::Drawable *>> drawnElements;  // 2D to allow element grouping (i.e., player's hand might be elements[2][0 through 5]). The order of elements in drawnElements is the order they are drawn in, which is important
};

#endif