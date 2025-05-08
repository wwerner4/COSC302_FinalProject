#include "gameGraphics.h"

#include <unistd.h>

#include <iostream>

#include "CardEvaluation/handEvaluator.h"
#include "cardDeck.h"
using namespace std;

// constructor, init values that don't change (window object, font object)
GameGraphics::GameGraphics(GameState *gameState) {

    // create the GUI window
    window.create(sf::VideoMode(1920 * 0.75, 1080 * 0.75), "Pixel Poker");
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(144);

    font.loadFromFile("./data/minecraft_font.ttf");

    // store pointer to GameState object
    state = gameState;

    // init values used in resizeWindow()
    currentWindowSize.x = window.getSize().x;
    currentWindowSize.y = window.getSize().y;

    return;
}

// free memory before destructing object
GameGraphics::~GameGraphics() {
    clearWindow();
    return;
}

// delete all drawn elements in a scene, used before beginning drawing a different scene or before class object deletion
void GameGraphics::clearWindow() {

    // delete all drawn objects stored in these vectors, then clear the vectors
    for (size_t i = 0; i < texts.size(); i++) {
        delete texts[i];
    }
    texts.clear();

    for (size_t i = 0; i < shapes.size(); i++) {
        delete shapes[i];
    }
    shapes.clear();

    for (size_t i = 0; i < sprites.size(); i++) {
        delete sprites[i];
    }
    sprites.clear();

    for (size_t i = 0; i < textures.size(); i++) {
        delete textures[i];
    }
    textures.clear();

    for (size_t i = 0; i < vertexArrays.size(); i++) {
        delete vertexArrays[i];
    }
    vertexArrays.clear();

    // at this point, all drawable objects are already deleted. we just need to reset interactibles and drawnElements
    interactables.clear();

    for (size_t i = 0; i < drawnElements.size(); i++) {
        for (size_t j = 0; j < drawnElements[i].size(); j++) {
        }
        drawnElements[i].clear();
    }
    drawnElements.clear();

    return;
}

// close the game window
void GameGraphics::closeWindow() {
    clearWindow();
    window.close();
    return;
}

// function for scaling texts, shapes, and sprites correctly when the window size changes.
void GameGraphics::resizeWindow(sf::Event resize) {
    sf::Vector2u newWindowSize(resize.size.width, resize.size.height);

    // compute the ratio of previous window size to new window size
    sf::Vector2f scaleFactor((float)newWindowSize.x / currentWindowSize.x, (float)newWindowSize.x / currentWindowSize.x);

    sf::Vector2f oldPosition, oldScale;
    // these loops are why we need texts, shapes, and sprites. drawnElements->getPosition() isn't a valid function call because drawnElements is sf::Drawable*, not specific drawable classes
    for (size_t i = 0; i < texts.size(); i++) {
        oldPosition = texts[i]->getPosition();
        oldScale = texts[i]->getScale();
        texts[i]->setPosition(oldPosition.x * scaleFactor.x, oldPosition.y * scaleFactor.y);
        texts[i]->setScale(oldScale.x * scaleFactor.x, oldScale.y * scaleFactor.y);
    }
    for (size_t i = 0; i < shapes.size(); i++) {
        oldPosition = shapes[i]->getPosition();
        oldScale = shapes[i]->getScale();
        shapes[i]->setPosition(oldPosition.x * scaleFactor.x, oldPosition.y * scaleFactor.y);
        shapes[i]->setScale(oldScale.x * scaleFactor.x, oldScale.y * scaleFactor.y);
    }
    for (size_t i = 0; i < sprites.size(); i++) {
        oldPosition = sprites[i]->getPosition();
        oldScale = sprites[i]->getScale();
        sprites[i]->setPosition(oldPosition.x * scaleFactor.x, oldPosition.y * scaleFactor.y);
        sprites[i]->setScale(oldScale.x * scaleFactor.x, oldScale.y * scaleFactor.y);
    }

    // https://www.sfml-dev.org/tutorials/2.2/graphics-view.php#showing-more-when-the-window-is-resized

    // reset the window's size so that drawn object positions will scale correctly
    sf::FloatRect visibleArea(0, 0, newWindowSize.x, newWindowSize.y);
    window.setView(sf::View(visibleArea));

    currentWindowSize = newWindowSize;

    return;
}

// function for handling drawn interactible clickables
void GameGraphics::onClick() {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f floatMousePos((float)mousePosition.x, (float)mousePosition.y);  // store the position of the mouse when a click occured

    // iterate through all objects in the interactables map. if the mouse was within the bounds of the interactable object when the click occured, do the interactable's associated action
    for (map<sf::Shape *, char>::iterator i = interactables.begin(); i != interactables.end(); i++) {
        sf::FloatRect interactableBounds = i->first->getGlobalBounds();
        if (interactableBounds.contains(floatMousePos)) {
            switch (i->second) {
                // play game
                case 'p':
                    state->gameBegin();
                    playScreen();
                    return;
                // exit game
                case 'e':
                    closeWindow();
                    return;
                // increment player bet
                case 'i':
                    incBet();
                    return;
                // decrement player bet
                case 'd':
                    decBet();
                    return;
                // player check/call/raise
                case 'b':
                    userBet();
                    return;
                // player fold
                case 'f':
                    userFold();
                    return;
            }
        }
    }
}

// helper function to draw each frame
void GameGraphics::loopDraw() {
    // best practice is to redraw the frame from scratch at each frame. each frame is drawn sequentially, with newer draws on top of older draws. always clear first (reset to background color)
    window.clear(sf::Color(36, 156, 68, 255));

    // draw each element in drawnElements sequentially
    for (size_t i = 0; i < drawnElements.size(); i++) {
        for (size_t j = 0; j < drawnElements[i].size(); j++) {
            window.draw(*drawnElements[i][j]);
        }
    }

    // display rendered frame to GUI
    window.display();

    return;
}

// give functionality to the increment bet button
void GameGraphics::incBet() {
    // only increment if the player can afford the incremented bet
    if (state->playerBet + 100 <= state->chips[0]) {
        state->playerBet += 100;
    }

    return;
}

// give functionality to the decrement bet button
void GameGraphics::decBet() {
    // only decrement if the player can bet the decremented bet legally
    if (state->playerBet - 100 >= state->minBet) {
        state->playerBet -= 100;
    }

    return;
}

// give functionality to the check/call/raise button
void GameGraphics::userBet() {

    // if it is the user's turn
    if (state->turn == 0) {
        state->bets[0] += state->playerBet;     // store the player's bet
        state->pot += state->playerBet;         // add the bet to the pot
        state->chips[0] -= state->playerBet;    // remove the bet from the player's chips
        state->playerHasBet = true;             // tell the checkState() function in the GameState object that the player has made their bet

        state->playerBet = 0;                   // reset the player's chosen bet indicator
    }

    return;
}

// give functionality to the fold button
void GameGraphics::userFold() {

    // if it is the user's turn
    if (state->turn == 0) {
        state->folds[0] = true;         // the player has folded
        state->playerHasBet = true;     // tell checkState() that the player has performed an action
        state->playerBet = 0;           // reset the player's chosen bet indicator
    }
    return;
}

// draw the main game scene
void GameGraphics::playScreen() {
    clearWindow();          // reset drawn objects
    drawnElements.resize(4);

    sf::Sprite *card;
    sf::Texture *texture;
    string cardName;

    // draw the table cards
    for (size_t i = 0; i < state->table.size(); i++) {

        // get the filename of the texture for this card
        cardName = getCardName(state->table[i]);

        // create a card texture for the card sprite we will create soon
        texture = new sf::Texture;

        textures.push_back(texture);
        texture->loadFromFile(cardName);

        // create a sprite from the card texture
        card = new sf::Sprite(*texture);
        card->setScale(2, 2);

        // position the card
        card->setOrigin(card->getLocalBounds().width / 2, card->getLocalBounds().height / 2);
        card->setPosition(currentWindowSize.x / 2 + ((card->getLocalBounds().width * card->getScale().x) + 25) * (int)(i - (state->table.size() / 2)), currentWindowSize.y / 2);

        sprites.push_back(card);
        drawnElements[0].push_back(card);
    }

    // draw the player's hand. card sprites are created in the same way as the table card sprites
    for (size_t i = 0; i < state->hands[0].size(); i++) {
        cardName = getCardName(state->hands[0][i]);

        texture = new sf::Texture;

        textures.push_back(texture);
        texture->loadFromFile(cardName);
        card = new sf::Sprite(*texture);
        card->setScale({2.5, 2.5});

        card->setOrigin(card->getLocalBounds().width / 2, card->getLocalBounds().height / 2);
        if (i == 0) {
            card->setPosition(currentWindowSize.x - 300, currentWindowSize.y - 150);
        } else {
            card->setPosition(sprites.back()->getPosition().x + (sprites.back()->getLocalBounds().width) * card->getScale().x + 25, sprites.back()->getPosition().y);  // mult localBounds by 2 because of scale factor
        }

        sprites.push_back(card);
        drawnElements[1].push_back(card);
    }

    // if the user hasn't folded, draw the user's betting ui
    if (!state->folds[0]) {

        // betIndicator is the black box behind the player's chosen bet
        sf::RectangleShape *betIndicator = new sf::RectangleShape(sf::Vector2f(200, 100));
        shapes.push_back(betIndicator);
        drawnElements[2].push_back(betIndicator);

        betIndicator->setFillColor(sf::Color::Black);

        betIndicator->setOrigin(0, betIndicator->getLocalBounds().height / 2);
        betIndicator->setPosition(25, currentWindowSize.y - 100);

        // betIndicatorText is the number showing the bet the user is about to place
        sf::Text *betIndicatorText = new sf::Text;
        texts.push_back(betIndicatorText);
        drawnElements[2].push_back(betIndicatorText);

        betIndicatorText->setFont(font);

        // if the player tries to bet more than they have, set the bet to their max possible bet
        if (state->playerBet < state->chips[0]) {
            betIndicatorText->setString(to_string(state->playerBet));
        } else {
            betIndicatorText->setString(to_string(state->chips[0]));
        }
        betIndicatorText->setCharacterSize(50);
        betIndicatorText->setFillColor(sf::Color::White);

        betIndicatorText->setOrigin(betIndicatorText->getLocalBounds().width / 2, betIndicatorText->getLocalBounds().height / 2);
        betIndicatorText->setPosition(betIndicator->getPosition().x + betIndicator->getLocalBounds().width / 2, betIndicator->getPosition().y - 10);

        // betDecrement is the gray box behind the white triangle that points down
        sf::RectangleShape *betDecrement = new sf::RectangleShape(sf::Vector2f(45, 45));
        shapes.push_back(betDecrement);
        drawnElements[2].push_back(betDecrement);

        interactables[betDecrement] = 'd';

        betDecrement->setFillColor(sf::Color(163, 163, 163, 255));

        betDecrement->setOrigin(0, betDecrement->getLocalBounds().height);
        betDecrement->setPosition(25 + betIndicator->getPosition().x + betIndicator->getSize().x, betIndicator->getPosition().y + betIndicator->getSize().y / 2);

        // decrementTriangle is the triangle representing a decrease in the user's bet
        sf::VertexArray *decrementTriangle = new sf::VertexArray(sf::Triangles, 3);
        vertexArrays.push_back(decrementTriangle);

        decrementTriangle[0][0].position = sf::Vector2f(betDecrement->getPosition().x + 5, betDecrement->getPosition().y - betDecrement->getSize().y + 5);
        decrementTriangle[0][0].color = sf::Color::White;
        decrementTriangle[0][1].position = sf::Vector2f(betDecrement->getPosition().x + betDecrement->getSize().x / 2, betDecrement->getPosition().y - 5);
        decrementTriangle[0][1].color = sf::Color::White;
        decrementTriangle[0][2].position = sf::Vector2f(betDecrement->getPosition().x + betDecrement->getSize().x - 5, betDecrement->getPosition().y - betDecrement->getSize().y + 5);
        decrementTriangle[0][2].color = sf::Color::White;

        drawnElements[2].push_back(decrementTriangle);

        // betIncrement is the grey box behind the white triangle that points up
        sf::RectangleShape *betIncrement = new sf::RectangleShape(betDecrement->getSize());
        shapes.push_back(betIncrement);
        drawnElements[2].push_back(betIncrement);

        interactables[betIncrement] = 'i';

        betIncrement->setFillColor(sf::Color(163, 163, 163, 255));

        betIncrement->setOrigin(0, betIncrement->getLocalBounds().height);
        betIncrement->setPosition(betDecrement->getPosition().x, betDecrement->getPosition().y - betDecrement->getSize().y - 10);

        // incrementTriangle is the triangle representing an increase in the player's bet
        sf::VertexArray *incrementTriangle = new sf::VertexArray(sf::Triangles, 3);
        vertexArrays.push_back(incrementTriangle);

        incrementTriangle[0][0].position = sf::Vector2f(betIncrement->getPosition().x + 5, betIncrement->getPosition().y - 5);
        incrementTriangle[0][0].color = sf::Color::White;
        incrementTriangle[0][1].position = sf::Vector2f(betIncrement->getPosition().x + betIncrement->getSize().x / 2, betIncrement->getPosition().y - betIncrement->getSize().y + 5);
        incrementTriangle[0][1].color = sf::Color::White;
        incrementTriangle[0][2].position = sf::Vector2f(betIncrement->getPosition().x + betIncrement->getSize().x - 5, betIncrement->getPosition().y - 5);
        incrementTriangle[0][2].color = sf::Color::White;

        drawnElements[2].push_back(incrementTriangle);

        // betButton is the button that the user presses to call/check/raise
        sf::RectangleShape *betButton = new sf::RectangleShape(betIndicator->getSize());
        shapes.push_back(betButton);
        drawnElements[2].push_back(betButton);

        interactables[betButton] = 'b';

        betButton->setFillColor(sf::Color(163, 163, 163, 255));

        betButton->setOrigin(betButton->getLocalBounds().width / 2, betButton->getLocalBounds().height / 2);
        betButton->setPosition(betIncrement->getPosition().x + betIncrement->getSize().x + 50 + betButton->getSize().x / 2, betIndicator->getPosition().y);

        // betButtonText is the word displayed on betButton
        sf::Text *betButtonText = new sf::Text;
        texts.push_back(betButtonText);
        drawnElements[2].push_back(betButtonText);

        betButtonText->setFont(font);

        // the text of the button changes depending on the betting state of the game
        if (state->minBet == 0 && state->playerBet == 0) {
            betButtonText->setString("Check");
        } else if (state->minBet == state->playerBet) {
            betButtonText->setString("Call");
        } else {
            betButtonText->setString("Raise");
        }

        betButtonText->setCharacterSize(50);
        betButtonText->setFillColor(sf::Color::White);

        betButtonText->setOrigin(betButtonText->getLocalBounds().width / 2, betButtonText->getLocalBounds().height / 2);
        betButtonText->setPosition(betButton->getPosition().x, betButton->getPosition().y - 10);

        // foldButton is the button the user presses to fold
        sf::RectangleShape *foldButton = new sf::RectangleShape(betIndicator->getSize());
        shapes.push_back(foldButton);
        drawnElements[2].push_back(foldButton);

        interactables[foldButton] = 'f';

        foldButton->setFillColor(sf::Color(163, 163, 163, 255));

        foldButton->setOrigin(foldButton->getLocalBounds().width / 2, foldButton->getLocalBounds().height / 2);
        foldButton->setPosition(betButton->getPosition().x + betButton->getSize().x + 25, betIndicator->getPosition().y);

        // foldButtonText is the word "Fold" on foldButton
        sf::Text *foldButtonText = new sf::Text;
        texts.push_back(foldButtonText);
        drawnElements[2].push_back(foldButtonText);

        foldButtonText->setFont(font);
        foldButtonText->setString("Fold");
        foldButtonText->setCharacterSize(50);
        foldButtonText->setFillColor(sf::Color::White);

        foldButtonText->setOrigin(foldButtonText->getLocalBounds().width / 2, foldButtonText->getLocalBounds().height / 2);
        foldButtonText->setPosition(foldButton->getPosition().x, foldButton->getPosition().y - 10);

    } else {        // if the player has folded/ is out of chips

        // replace the betting interface with text thta says the user has folded
        sf::Text *foldIndicator = new sf::Text;
        texts.push_back(foldIndicator);
        drawnElements[2].push_back(foldIndicator);

        foldIndicator->setFont(font);
        foldIndicator->setString("You Have Folded");
        foldIndicator->setCharacterSize(80);
        foldIndicator->setFillColor(sf::Color::White);

        foldIndicator->setOrigin(0, foldIndicator->getLocalBounds().height);
        foldIndicator->setPosition(25, currentWindowSize.y - 100);
    }

    // text to display the user's total chip count
    sf::Text *totalChipsText = new sf::Text;
    texts.push_back(totalChipsText);
    drawnElements[2].push_back(totalChipsText);

    totalChipsText->setFont(font);
    totalChipsText->setString("YOUR CHIPS: " + to_string(state->chips[0]));
    totalChipsText->setCharacterSize(50);
    totalChipsText->setFillColor(sf::Color::White);

    totalChipsText->setOrigin(0, totalChipsText->getLocalBounds().height);
    totalChipsText->setPosition(25, (currentWindowSize.y - 100) - 50 - 40);

    // text to display the amount that the user has bet in this stage of the game
    sf::Text *playerTurnBetText = new sf::Text;
    texts.push_back(playerTurnBetText);
    drawnElements[2].push_back(playerTurnBetText);

    playerTurnBetText->setFont(font);
    playerTurnBetText->setString("YOUR BET THIS TURN: " + to_string(state->bets[0]));
    playerTurnBetText->setCharacterSize(50);
    playerTurnBetText->setFillColor(sf::Color::White);

    playerTurnBetText->setOrigin(0, playerTurnBetText->getLocalBounds().height);
    playerTurnBetText->setPosition(totalChipsText->getPosition().x, totalChipsText->getPosition().y - 50);

    // text to display the total pot for this hand
    sf::Text *potText = new sf::Text;
    texts.push_back(potText);
    drawnElements[2].push_back(potText);

    potText->setFont(font);
    potText->setString("Pot: " + to_string(state->pot));
    potText->setCharacterSize(50);
    potText->setFillColor(sf::Color::White);

    potText->setOrigin(0, potText->getLocalBounds().height / 2);
    potText->setPosition(25, currentWindowSize.y / 2);


    // this loop displays information about each cpu player
    for (int i = 1; i < state->numPlayers; i++) {
        // if the player hasn't folded
        if (!state->folds[i]) {

            // draw hidden cards representing the cpu player's hand
            for (size_t j = 0; j < state->hands[i].size(); j++) {
                texture = new sf::Texture;

                textures.push_back(texture);
                texture->loadFromFile("./data/PixelPlayingCardsPack/back_red_basic_white.png");
                card = new sf::Sprite(*texture);
                card->setScale({1.5, 1.5});

                if (j == 0) {
                    card->setPosition((currentWindowSize.x / (state->numPlayers - 1)) * (i - 1) + (currentWindowSize.x / ((state->numPlayers - 1) * 2)) - (card->getLocalBounds().width + 25) / 2, 25);
                } else {
                    card->setPosition(sprites.back()->getPosition().x + 25, sprites.back()->getPosition().y);  // mult localBounds by 2 because of scale factor
                }

                sprites.push_back(card);
                drawnElements[3].push_back(card);
            }

            // this text names the cpu player
            sf::Text *cpu = new sf::Text;
            texts.push_back(cpu);
            drawnElements[2].push_back(cpu);

            cpu->setFont(font);
            cpu->setString("CPU " + to_string(i));
            cpu->setCharacterSize(20);
            cpu->setFillColor(sf::Color::White);

            cpu->setOrigin(cpu->getLocalBounds().width / 2, cpu->getLocalBounds().height / 2);
            cpu->setPosition((currentWindowSize.x / (state->numPlayers - 1)) * (i - 1) + (currentWindowSize.x / ((state->numPlayers - 1) * 2)), 10 + 151);  // , 10 + cpu->getLocalBounds().height / 2 + sprites.back()->getPosition().y + (sprites.back()->getLocalBounds().height) * sprites.back()->getScale().y);

            // this text displays the cup player's chip count
            sf::Text *cpuChips = new sf::Text;
            texts.push_back(cpuChips);
            drawnElements[2].push_back(cpuChips);

            cpuChips->setFont(font);
            cpuChips->setString("CHIPS: " + to_string(state->chips[i]));
            cpuChips->setCharacterSize(20);
            cpuChips->setFillColor(sf::Color::White);

            cpuChips->setOrigin(cpuChips->getLocalBounds().width / 2, cpuChips->getLocalBounds().height / 2);
            cpuChips->setPosition((currentWindowSize.x / (state->numPlayers - 1)) * (i - 1) + (currentWindowSize.x / ((state->numPlayers - 1) * 2)), cpu->getPosition().y + 10 + cpu->getLocalBounds().height);  // , 10 + cpu->getLocalBounds().height / 2 + sprites.back()->getPosition().y + (sprites.back()->getLocalBounds().height) * sprites.back()->getScale().y);

            // this text displays the cpu player's bet for this stage in the hand
            sf::Text *cpuBet = new sf::Text;
            texts.push_back(cpuBet);
            drawnElements[2].push_back(cpuBet);

            cpuBet->setFont(font);
            int betThisHand = state->bets[i];
            if (betThisHand < 0) {
                betThisHand = 0;
            }

            cpuBet->setString("Bet: " + to_string(betThisHand));
            cpuBet->setCharacterSize(20);
            cpuBet->setFillColor(sf::Color::White);

            cpuBet->setOrigin(cpuBet->getLocalBounds().width / 2, cpuBet->getLocalBounds().height / 2);
            cpuBet->setPosition((currentWindowSize.x / (state->numPlayers - 1)) * (i - 1) + (currentWindowSize.x / ((state->numPlayers - 1) * 2)), cpuChips->getPosition().y + 10 + cpuChips->getLocalBounds().height);
        } else {            // if the cpu player has folded

            // this text states that the cpu player has folded
            sf::Text *cpuFoldIndicator = new sf::Text;
            texts.push_back(cpuFoldIndicator);
            drawnElements[2].push_back(cpuFoldIndicator);

            cpuFoldIndicator->setFont(font);
            cpuFoldIndicator->setString("CPU" + to_string(i) + " HAS FOLDED");
            cpuFoldIndicator->setCharacterSize(45);
            cpuFoldIndicator->setFillColor(sf::Color::White);

            cpuFoldIndicator->setOrigin(cpuFoldIndicator->getLocalBounds().width / 2, cpuFoldIndicator->getLocalBounds().height / 2);
            cpuFoldIndicator->setPosition(((currentWindowSize.x / (state->numPlayers - 1)) * (i - 1)) + (currentWindowSize.x / ((state->numPlayers - 1) * 2)), 75);
        }
    }

    // if the hand is over, display the hand winner and their winning hand to the screen
    if (state->gameStage == 4) {
        endScreen();
    }

    return;
}

// show all cpu players' cards and display info about the winner of the hand
// https://en.sfml-dev.org/forums/index.php?topic=22496.0
void GameGraphics::endScreen() {
    int indexStart = textures.size() - ((state->numPlayers - 1) * 2);
    for (int i = 1; i < state->numPlayers; i++) {
        if (!state->folds[i]) {
            for (int j = 0; j < 2; j++) {
                sf::Texture *texture = textures[indexStart + (i - 1) * 2 + j];

                string newTexture = getCardName(state->hands[i][j]);
                texture = new sf::Texture;
                texture->loadFromFile(newTexture);

                sprites[indexStart + (i - 1) * 2 + j]->setTexture(*texture);
            }
        }
    }

    // display the player who won the hand and the rank of their hand's cards
    sf::Text *win = new sf::Text;
    texts.push_back(win);
    drawnElements[2].push_back(win);

    int winner = state->determineWinner();

    string winText;
    if (winner == 0) {
        winText = "You won this hand";
    } else {
        winText = "CPU" + to_string(winner) + " won this hand";
    }
    vector<int> winnerHand = state->hands[winner];
    winnerHand.insert(winnerHand.end(), state->table.begin(), state->table.end());
    winText += " with a " + evaluateHand(winnerHand);

    win->setString(winText);
    win->setFont(font);
    win->setCharacterSize(50);
    win->setFillColor(sf::Color::White);

    win->setOrigin(win->getLocalBounds().width / 2, win->getLocalBounds().height / 2);
    win->setPosition(currentWindowSize.x / 2, currentWindowSize.y / 2 - 125);

    // draw these additions to the screen. When this function is called, we sleep for 10 seconds, so this frame is only drawn to the screen 1 time for this 10-second span.
    loopDraw();

    return;
}

// display a message about players being out of chips before closing the game window
void GameGraphics::gameOver() {
    sf::Text *gameOver = new sf::Text;
    texts.push_back(gameOver);
    drawnElements[2].push_back(gameOver);

    gameOver->setFont(font);
    gameOver->setString("Game Over: Too Many Players Out Of Chips");
    gameOver->setCharacterSize(50);
    gameOver->setFillColor(sf::Color::White);

    gameOver->setOrigin(gameOver->getLocalBounds().width / 2, gameOver->getLocalBounds().height / 2);
    gameOver->setPosition(currentWindowSize.x / 2, currentWindowSize.y / 2 - 165);

    loopDraw();

    return;
}

// build scene for the title screen of the game
void GameGraphics::titleScreen() {
    clearWindow();
    drawnElements.resize(3);

    // game title
    sf::Text *title = new sf::Text;
    texts.push_back(title);
    drawnElements[0].push_back(title);

    title->setFont(font);
    title->setString("Pixel Poker");
    title->setCharacterSize(80);
    title->setFillColor(sf::Color::White);

    title->setOrigin(title->getLocalBounds().width / 2, title->getLocalBounds().height / 2);
    title->setPosition(currentWindowSize.x / 2, currentWindowSize.y / 5);

    // button to start the game loop
    sf::RectangleShape *playButton = new sf::RectangleShape(sf::Vector2f(500, 75));
    shapes.push_back(playButton);
    drawnElements[1].push_back(playButton);

    interactables[playButton] = 'p';

    playButton->setFillColor(sf::Color(163, 163, 163, 255));

    playButton->setOrigin(playButton->getLocalBounds().width / 2, playButton->getLocalBounds().height / 2);
    playButton->setPosition(title->getPosition().x, title->getPosition().y + 200);

    // text for play button
    sf::Text *playButtonText = new sf::Text;
    texts.push_back(playButtonText);
    drawnElements[1].push_back(playButtonText);

    playButtonText->setFont(font);
    playButtonText->setString("Play Game");
    playButtonText->setCharacterSize(50);
    playButtonText->setFillColor(sf::Color::White);

    playButtonText->setOrigin(playButtonText->getLocalBounds().width / 2, playButtonText->getLocalBounds().height / 2);
    playButtonText->setPosition(playButton->getPosition().x, playButton->getPosition().y - 10);

    // button to close game
    sf::RectangleShape *exitButton = new sf::RectangleShape(sf::Vector2f(500, 75));
    shapes.push_back(exitButton);
    drawnElements[1].push_back(exitButton);

    interactables[exitButton] = 'e';

    exitButton->setFillColor(sf::Color(163, 163, 163, 255));

    exitButton->setOrigin(exitButton->getLocalBounds().width / 2, exitButton->getLocalBounds().height / 2);
    exitButton->setPosition(playButton->getPosition().x, playButton->getPosition().y + 100);

    // text for exit button
    sf::Text *exitButtonText = new sf::Text;
    texts.push_back(exitButtonText);
    drawnElements[1].push_back(exitButtonText);

    exitButtonText->setFont(font);
    exitButtonText->setString("Exit");
    exitButtonText->setCharacterSize(50);
    exitButtonText->setFillColor(sf::Color::White);

    exitButtonText->setOrigin(exitButtonText->getLocalBounds().width / 2, exitButtonText->getLocalBounds().height / 2);
    exitButtonText->setPosition(exitButton->getPosition().x, exitButton->getPosition().y - 10);

    // deck of cards used to draw random fanned cards to the screen for decoration
    CardDeck *displayDeck = new CardDeck;
    displayDeck->shuffle();
    vector<int> displayCards;

    // add 6 random cards to displayCards
    for (int i = 0; i < 6; i++) {
        sf::Texture *texture = new sf::Texture;

        displayCards.push_back(displayDeck->draw());
        texture->loadFromFile(getCardName(displayCards.back()));

        sf::Sprite *card = new sf::Sprite(*texture);

        textures.push_back(texture);
        sprites.push_back(card);
    }

    delete displayDeck;

    // display the displayDeck
    for (size_t i = 0; i < sprites.size(); i++) {
        sprites[i]->setOrigin(sprites[i]->getLocalBounds().width / 2, sprites[i]->getLocalBounds().height / 2);
        sprites[i]->setScale(2, 2);

        if (i < sprites.size() / 2) {
            sprites[i]->setPosition(exitButton->getPosition().x - 474, exitButton->getPosition().y);
        } else {
            sprites[i]->setPosition(exitButton->getPosition().x + 350, exitButton->getPosition().y);
        }

        sprites[i]->setOrigin(0, sprites[i]->getLocalBounds().height);
        sprites[i]->rotate((350) + (15 * (i % 3)));

        drawnElements[2].push_back(sprites[i]);
    }

    return;
}