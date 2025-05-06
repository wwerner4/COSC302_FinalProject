#include "gameGraphics.h"

#include <iostream>
#include <unistd.h>

#include "CardEvaluation/handEvaluator.h"
#include "cardDeck.h"
using namespace std;

// constructor, init values that don't change (window object, font object)
GameGraphics::GameGraphics(GameState *gameState) {
    window.create(sf::VideoMode(1920 * 0.75, 1080 * 0.75), "Pixel Poker");
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(144);

    font.loadFromFile("./data/minecraft_font.ttf");

    state = gameState;

    // used in resizeWindow()
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
    window.close();
    return;
}

// function for scaling texts, shapes, and sprites correctly when the window size changes.
void GameGraphics::resizeWindow(sf::Event resize) {
    sf::Vector2u newWindowSize(resize.size.width, resize.size.height);
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
    sf::FloatRect visibleArea(0, 0, newWindowSize.x, newWindowSize.y);
    window.setView(sf::View(visibleArea));

    currentWindowSize = newWindowSize;

    return;
}

// function for handling drawn interactible clickables
void GameGraphics::onClick() {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f floatMousePos((float)mousePosition.x, (float)mousePosition.y);

    for (map<sf::Shape *, char>::iterator i = interactables.begin(); i != interactables.end(); i++) {
        sf::FloatRect interactableBounds = i->first->getGlobalBounds();
        if (interactableBounds.contains(floatMousePos)) {
            switch (i->second) {
                case 'p':
                    state->gameBegin();
                    matchGameState();
                    return;
                case 'e':
                    closeWindow();
                    return;
                case 'i':
                    incBet();
                    return;
                case 'd':
                    decBet();
                    return;
                case 'b':
                    userBet();
                    return;
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

    for (size_t i = 0; i < drawnElements.size(); i++) {
        for (size_t j = 0; j < drawnElements[i].size(); j++) {
            window.draw(*drawnElements[i][j]);
        }
    }

    window.display();

    return;
}

void GameGraphics::incBet() {
    if (state->playerBet + 100 <= state->chips[0]) {
        state->playerBet += 100;
    }

    return;
}

void GameGraphics::decBet() {
    if (state->playerBet - 100 >= state->minBet) {
        state->playerBet -= 100;
    }

    return;
}

void GameGraphics::userBet() {
    if (state->turn == 0) {
        state->bets[0] += state->playerBet;
        state->pot += state->bets[0];
        state->chips[0] -= state->playerBet;
        state->playerHasBet = true;

        state->playerBet = 0;
    }

    return;
}

void GameGraphics::userFold() {
    if (state->turn == 0) {
        state->folds[0] = true;
        state->playerHasBet = true;
        state->playerBet = 0;
    }
    return;
}

void GameGraphics::matchGameState() {
    clearWindow();
    drawnElements.resize(4);

    sf::Sprite *card;
    sf::Texture *texture;
    string cardName;

    for (size_t i = 0; i < state->table.size(); i++) {
        cardName = getCardName(state->table[i]);

        texture = new sf::Texture;

        textures.push_back(texture);
        texture->loadFromFile(cardName);
        card = new sf::Sprite(*texture);
        card->setScale(2, 2);

        card->setOrigin(card->getLocalBounds().width / 2, card->getLocalBounds().height / 2);
        card->setPosition(currentWindowSize.x / 2 + ((card->getLocalBounds().width * card->getScale().x) + 25) * (int)(i - (state->table.size() / 2)), currentWindowSize.y / 2);

        sprites.push_back(card);
        drawnElements[0].push_back(card);
    }

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

    if (!state->folds[0]) {
        sf::RectangleShape *betIndicator = new sf::RectangleShape(sf::Vector2f(200, 100));
        shapes.push_back(betIndicator);
        drawnElements[2].push_back(betIndicator);

        betIndicator->setFillColor(sf::Color::Black);

        betIndicator->setOrigin(0, betIndicator->getLocalBounds().height / 2);
        betIndicator->setPosition(25, currentWindowSize.y - 100);

        sf::Text *betIndicatorText = new sf::Text;
        texts.push_back(betIndicatorText);
        drawnElements[2].push_back(betIndicatorText);

        betIndicatorText->setFont(font);
        betIndicatorText->setString(to_string(state->playerBet));
        betIndicatorText->setCharacterSize(50);
        betIndicatorText->setFillColor(sf::Color::White);

        betIndicatorText->setOrigin(betIndicatorText->getLocalBounds().width / 2, betIndicatorText->getLocalBounds().height / 2);
        betIndicatorText->setPosition(betIndicator->getPosition().x + betIndicator->getLocalBounds().width / 2, betIndicator->getPosition().y - 10);

        sf::RectangleShape *betDecrement = new sf::RectangleShape(sf::Vector2f(45, 45));
        shapes.push_back(betDecrement);
        drawnElements[2].push_back(betDecrement);

        interactables[betDecrement] = 'd';

        betDecrement->setFillColor(sf::Color(163, 163, 163, 255));

        betDecrement->setOrigin(0, betDecrement->getLocalBounds().height);
        betDecrement->setPosition(25 + betIndicator->getPosition().x + betIndicator->getSize().x, betIndicator->getPosition().y + betIndicator->getSize().y / 2);

        sf::VertexArray *decrementTriangle = new sf::VertexArray(sf::Triangles, 3);

        decrementTriangle[0][0].position = sf::Vector2f(betDecrement->getPosition().x + 5, betDecrement->getPosition().y - betDecrement->getSize().y + 5);
        decrementTriangle[0][0].color = sf::Color::White;
        decrementTriangle[0][1].position = sf::Vector2f(betDecrement->getPosition().x + betDecrement->getSize().x / 2, betDecrement->getPosition().y - 5);
        decrementTriangle[0][1].color = sf::Color::White;
        decrementTriangle[0][2].position = sf::Vector2f(betDecrement->getPosition().x + betDecrement->getSize().x - 5, betDecrement->getPosition().y - betDecrement->getSize().y + 5);
        decrementTriangle[0][2].color = sf::Color::White;

        drawnElements[2].push_back(decrementTriangle);

        sf::RectangleShape *betIncrement = new sf::RectangleShape(betDecrement->getSize());
        shapes.push_back(betIncrement);
        drawnElements[2].push_back(betIncrement);

        interactables[betIncrement] = 'i';

        betIncrement->setFillColor(sf::Color(163, 163, 163, 255));

        betIncrement->setOrigin(0, betIncrement->getLocalBounds().height);
        betIncrement->setPosition(betDecrement->getPosition().x, betDecrement->getPosition().y - betDecrement->getSize().y - 10);

        sf::VertexArray *incrementTriangle = new sf::VertexArray(sf::Triangles, 3);

        incrementTriangle[0][0].position = sf::Vector2f(betIncrement->getPosition().x + 5, betIncrement->getPosition().y - 5);
        incrementTriangle[0][0].color = sf::Color::White;
        incrementTriangle[0][1].position = sf::Vector2f(betIncrement->getPosition().x + betIncrement->getSize().x / 2, betIncrement->getPosition().y - betIncrement->getSize().y + 5);
        incrementTriangle[0][1].color = sf::Color::White;
        incrementTriangle[0][2].position = sf::Vector2f(betIncrement->getPosition().x + betIncrement->getSize().x - 5, betIncrement->getPosition().y - 5);
        incrementTriangle[0][2].color = sf::Color::White;

        drawnElements[2].push_back(incrementTriangle);

        sf::RectangleShape *betButton = new sf::RectangleShape(betIndicator->getSize());
        shapes.push_back(betButton);
        drawnElements[2].push_back(betButton);

        interactables[betButton] = 'b';

        betButton->setFillColor(sf::Color(163, 163, 163, 255));

        betButton->setOrigin(betButton->getLocalBounds().width / 2, betButton->getLocalBounds().height / 2);
        betButton->setPosition(betIncrement->getPosition().x + betIncrement->getSize().x + 50 + betButton->getSize().x / 2, betIndicator->getPosition().y);

        sf::Text *betButtonText = new sf::Text;
        texts.push_back(betButtonText);
        drawnElements[2].push_back(betButtonText);

        betButtonText->setFont(font);

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

        sf::RectangleShape *foldButton = new sf::RectangleShape(betIndicator->getSize());
        shapes.push_back(foldButton);
        drawnElements[2].push_back(foldButton);

        interactables[foldButton] = 'f';

        foldButton->setFillColor(sf::Color(163, 163, 163, 255));

        foldButton->setOrigin(foldButton->getLocalBounds().width / 2, foldButton->getLocalBounds().height / 2);
        foldButton->setPosition(betButton->getPosition().x + betButton->getSize().x + 25, betIndicator->getPosition().y);

        sf::Text *foldButtonText = new sf::Text;
        texts.push_back(foldButtonText);
        drawnElements[2].push_back(foldButtonText);

        foldButtonText->setFont(font);
        foldButtonText->setString("Fold");
        foldButtonText->setCharacterSize(50);
        foldButtonText->setFillColor(sf::Color::White);

        foldButtonText->setOrigin(foldButtonText->getLocalBounds().width / 2, foldButtonText->getLocalBounds().height / 2);
        foldButtonText->setPosition(foldButton->getPosition().x, foldButton->getPosition().y - 10);

    } else {
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

    sf::Text *totalChipsText = new sf::Text;
    texts.push_back(totalChipsText);
    drawnElements[2].push_back(totalChipsText);

    totalChipsText->setFont(font);
    totalChipsText->setString("YOUR CHIPS: " + to_string(state->chips[0]));
    totalChipsText->setCharacterSize(50);
    totalChipsText->setFillColor(sf::Color::White);

    totalChipsText->setOrigin(0, totalChipsText->getLocalBounds().height);
    totalChipsText->setPosition(25, (currentWindowSize.y - 100) - 50 - 40);

    sf::Text *potText = new sf::Text;
    texts.push_back(potText);
    drawnElements[2].push_back(potText);

    potText->setFont(font);
    potText->setString("Pot: " + to_string(state->pot));
    potText->setCharacterSize(50);
    potText->setFillColor(sf::Color::White);

    potText->setOrigin(0, potText->getLocalBounds().height / 2);
    potText->setPosition(25, currentWindowSize.y / 2);

    for (int i = 1; i < state->numPlayers; i++) {
        if (!state->folds[i]) {
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
            sf::Text *cpu = new sf::Text;
            texts.push_back(cpu);
            drawnElements[2].push_back(cpu);

            cpu->setFont(font);
            cpu->setString("CPU " + to_string(i));
            cpu->setCharacterSize(20);
            cpu->setFillColor(sf::Color::White);

            cpu->setOrigin(cpu->getLocalBounds().width / 2, cpu->getLocalBounds().height / 2);
            cpu->setPosition((currentWindowSize.x / (state->numPlayers - 1)) * (i - 1) + (currentWindowSize.x / ((state->numPlayers - 1) * 2)), 10 + 151);  // , 10 + cpu->getLocalBounds().height / 2 + sprites.back()->getPosition().y + (sprites.back()->getLocalBounds().height) * sprites.back()->getScale().y);

            sf::Text *cpuChips = new sf::Text;
            texts.push_back(cpuChips);
            drawnElements[2].push_back(cpuChips);

            cpuChips->setFont(font);
            cpuChips->setString("CHIPS: " + to_string(state->chips[i]));
            cpuChips->setCharacterSize(20);
            cpuChips->setFillColor(sf::Color::White);

            cpuChips->setOrigin(cpuChips->getLocalBounds().width / 2, cpuChips->getLocalBounds().height / 2);
            cpuChips->setPosition((currentWindowSize.x / (state->numPlayers - 1)) * (i - 1) + (currentWindowSize.x / ((state->numPlayers - 1) * 2)), cpu->getPosition().y + 10 + cpu->getLocalBounds().height);  // , 10 + cpu->getLocalBounds().height / 2 + sprites.back()->getPosition().y + (sprites.back()->getLocalBounds().height) * sprites.back()->getScale().y);

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
        } else {
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

    if (state->gameStage == 4) {
        endScreen();
    }

    return;
}

// https://en.sfml-dev.org/forums/index.php?topic=22496.0
void GameGraphics::endScreen() {
    int indexStart = textures.size() - ((state->numPlayers - 1)*2);
    for (int i = 1; i < state->numPlayers; i++) {
        if(!state->folds[i]) {
            for (int j = 0; j < 2; j++) {
                sf::Texture *texture = textures[indexStart + (i-1)*2 + j];

                string newTexture = getCardName(state->hands[i][j]);
                texture = new sf::Texture;
                texture->loadFromFile(newTexture);

                sprites[indexStart + (i-1)*2 + j]->setTexture(*texture);
            }
        }
    }

    sf::Text *win = new sf::Text;
            texts.push_back(win);
            drawnElements[2].push_back(win);

            win->setFont(font);
            if (state->determineWinner() == 0) {
                win->setString("You won this hand");
            } else {
                win->setString("CPU" + to_string(state->determineWinner()) + " won this hand");
            }
            win->setCharacterSize(100);
            win->setFillColor(sf::Color::White);

            win->setOrigin(win->getLocalBounds().width / 2, win->getLocalBounds().height / 2);
            win->setPosition(currentWindowSize.x / 2, currentWindowSize.y / 2 - 165);

    loopDraw();

    return;
}

// a test scene that the user should never see. this will eventually be deleted
void GameGraphics::testScreen() {
    clearWindow();
    drawnElements.resize(1);

    sf::Text *text = new sf::Text;
    texts.push_back(text);

    text->setFont(font);
    text->setString("Hello World");
    text->setCharacterSize(80);
    text->setFillColor(sf::Color::White);

    sf::FloatRect textSpace = text->getLocalBounds();
    text->setOrigin(textSpace.width / 2, textSpace.height / 2);

    sf::RectangleShape *rect = new sf::RectangleShape(sf::Vector2f(textSpace.width, textSpace.height));
    shapes.push_back(rect);

    rect->setOrigin(textSpace.width / 2, textSpace.height / 2);
    rect->setFillColor(sf::Color::Black);

    text->setPosition(window.getSize().x / 2, window.getSize().y / 2);
    rect->setPosition(window.getSize().x / 2, window.getSize().y / 2 + 20);

    drawnElements[0].push_back(rect);
    drawnElements[0].push_back(text);

    return;
}

// build scene for the title screen of the game
void GameGraphics::titleScreen() {
    clearWindow();
    drawnElements.resize(3);

    sf::Text *title = new sf::Text;
    texts.push_back(title);
    drawnElements[0].push_back(title);

    title->setFont(font);
    title->setString("Pixel Poker");
    title->setCharacterSize(80);
    title->setFillColor(sf::Color::White);

    title->setOrigin(title->getLocalBounds().width / 2, title->getLocalBounds().height / 2);
    title->setPosition(currentWindowSize.x / 2, currentWindowSize.y / 5);

    sf::RectangleShape *playButton = new sf::RectangleShape(sf::Vector2f(500, 75));
    shapes.push_back(playButton);
    drawnElements[1].push_back(playButton);

    interactables[playButton] = 'p';

    playButton->setFillColor(sf::Color(163, 163, 163, 255));

    playButton->setOrigin(playButton->getLocalBounds().width / 2, playButton->getLocalBounds().height / 2);
    playButton->setPosition(title->getPosition().x, title->getPosition().y + 200);

    sf::Text *playButtonText = new sf::Text;
    texts.push_back(playButtonText);
    drawnElements[1].push_back(playButtonText);

    playButtonText->setFont(font);
    playButtonText->setString("Play Game");
    playButtonText->setCharacterSize(50);
    playButtonText->setFillColor(sf::Color::White);

    playButtonText->setOrigin(playButtonText->getLocalBounds().width / 2, playButtonText->getLocalBounds().height / 2);
    playButtonText->setPosition(playButton->getPosition().x, playButton->getPosition().y - 10);

    sf::RectangleShape *exitButton = new sf::RectangleShape(sf::Vector2f(500, 75));
    shapes.push_back(exitButton);
    drawnElements[1].push_back(exitButton);

    interactables[exitButton] = 'e';

    exitButton->setFillColor(sf::Color(163, 163, 163, 255));

    exitButton->setOrigin(exitButton->getLocalBounds().width / 2, exitButton->getLocalBounds().height / 2);
    exitButton->setPosition(playButton->getPosition().x, playButton->getPosition().y + 100);

    sf::Text *exitButtonText = new sf::Text;
    texts.push_back(exitButtonText);
    drawnElements[1].push_back(exitButtonText);

    exitButtonText->setFont(font);
    exitButtonText->setString("Exit");
    exitButtonText->setCharacterSize(50);
    exitButtonText->setFillColor(sf::Color::White);

    exitButtonText->setOrigin(exitButtonText->getLocalBounds().width / 2, exitButtonText->getLocalBounds().height / 2);
    exitButtonText->setPosition(exitButton->getPosition().x, exitButton->getPosition().y - 10);

    CardDeck *displayDeck = new CardDeck;
    displayDeck->shuffle();
    vector<int> displayCards;

    for (int i = 0; i < 6; i++) {
        sf::Texture *texture = new sf::Texture;

        displayCards.push_back(displayDeck->draw());
        texture->loadFromFile(getCardName(displayCards.back()));

        sf::Sprite *card = new sf::Sprite(*texture);

        textures.push_back(texture);
        sprites.push_back(card);
    }

    delete displayDeck;
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

/*
// build the initial main game scene, currently just a layout
void GameGraphics::playScreen() {
    clearWindow();
    drawnElements.resize(4);  // table cards, player hand, interface, cpu hands

    CardDeck *testDeck = new CardDeck();

    testDeck->shuffle();

    vector<int> tableCards;
    sf::Sprite *card;
    sf::Texture *texture;
    string cardName;

    // setup table cards
    for (int i = 0; i < 5; i++) {
        cardName = getCardNameModified(tableCards.back());

        texture = new sf::Texture;

        textures.push_back(texture);
        texture->loadFromFile(cardName);
        card = new sf::Sprite(*texture);
        card->setScale(2, 2);

        card->setOrigin(card->getLocalBounds().width / 2, card->getLocalBounds().height / 2);
        card->setPosition(currentWindowSize.x / 2 + ((card->getLocalBounds().width * card->getScale().x) + 25) * (i - 2), currentWindowSize.y / 2);

        sprites.push_back(card);
        drawnElements[0].push_back(card);
    }

    vector<int> playerCards;

    // setup player cards
    for (int i = 0; i < 2; i++) {
        playerCards.push_back(testDeck->draw());

        cardName = getCardNameModified(playerCards.back());

        texture = new sf::Texture;

        textures.push_back(texture);
        texture->loadFromFile(cardName);
        card = new sf::Sprite(*texture);
        card->setScale({2.5, 2.5});

        card->setOrigin(card->getLocalBounds().width / 2, card->getLocalBounds().height / 2);
        if (drawnElements[1].size() == 0) {
            card->setPosition(currentWindowSize.x - 300, currentWindowSize.y - 150);
        } else {
            card->setPosition(sprites.back()->getPosition().x + (sprites.back()->getLocalBounds().width) * card->getScale().x + 25, sprites.back()->getPosition().y);  // mult localBounds by 2 because of scale factor
        }

        sprites.push_back(card);
        drawnElements[1].push_back(card);
    }

    sf::RectangleShape *betIndicator = new sf::RectangleShape(sf::Vector2f(200, 100));
    shapes.push_back(betIndicator);
    drawnElements[2].push_back(betIndicator);

    betIndicator->setFillColor(sf::Color::Black);

    betIndicator->setOrigin(0, betIndicator->getLocalBounds().height / 2);
    betIndicator->setPosition(25, currentWindowSize.y - 100);

    sf::Text *betIndicatorText = new sf::Text;
    texts.push_back(betIndicatorText);
    drawnElements[2].push_back(betIndicatorText);

    betIndicatorText->setFont(font);
    betIndicatorText->setString("PLACEHOLDER");
    betIndicatorText->setCharacterSize(50);
    betIndicatorText->setFillColor(sf::Color::White);

    betIndicatorText->setOrigin(betIndicatorText->getLocalBounds().width / 2, betIndicatorText->getLocalBounds().height / 2);
    betIndicatorText->setPosition(betIndicator->getPosition().x + betIndicator->getLocalBounds().width / 2, betIndicator->getPosition().y - 10);

    sf::Text *totalChipsText = new sf::Text;
    texts.push_back(totalChipsText);
    drawnElements[2].push_back(totalChipsText);

    totalChipsText->setFont(font);
    totalChipsText->setString("YOUR CHIPS: PLACEHOLDER");
    totalChipsText->setCharacterSize(50);
    totalChipsText->setFillColor(sf::Color::White);

    totalChipsText->setOrigin(0, totalChipsText->getLocalBounds().height);
    totalChipsText->setPosition(betIndicator->getPosition().x, betIndicator->getPosition().y - betIndicator->getSize().y / 2 - 40);

    sf::RectangleShape *betDecrement = new sf::RectangleShape(sf::Vector2f(45, 45));
    shapes.push_back(betDecrement);
    drawnElements[2].push_back(betDecrement);

    interactables[betDecrement] = 'd';

    betDecrement->setFillColor(sf::Color(163, 163, 163, 255));

    betDecrement->setOrigin(0, betDecrement->getLocalBounds().height);
    betDecrement->setPosition(25 + betIndicator->getPosition().x + betIndicator->getSize().x, betIndicator->getPosition().y + betIndicator->getSize().y / 2);

    sf::VertexArray *decrementTriangle = new sf::VertexArray(sf::Triangles, 3);

    decrementTriangle[0][0].position = sf::Vector2f(betDecrement->getPosition().x + 5, betDecrement->getPosition().y - betDecrement->getSize().y + 5);
    decrementTriangle[0][0].color = sf::Color::White;
    decrementTriangle[0][1].position = sf::Vector2f(betDecrement->getPosition().x + betDecrement->getSize().x / 2, betDecrement->getPosition().y - 5);
    decrementTriangle[0][1].color = sf::Color::White;
    decrementTriangle[0][2].position = sf::Vector2f(betDecrement->getPosition().x + betDecrement->getSize().x - 5, betDecrement->getPosition().y - betDecrement->getSize().y + 5);
    decrementTriangle[0][2].color = sf::Color::White;

    drawnElements[2].push_back(decrementTriangle);

    sf::RectangleShape *betIncrement = new sf::RectangleShape(betDecrement->getSize());
    shapes.push_back(betIncrement);
    drawnElements[2].push_back(betIncrement);

    interactables[betIncrement] = 'i';

    betIncrement->setFillColor(sf::Color(163, 163, 163, 255));

    betIncrement->setOrigin(0, betIncrement->getLocalBounds().height);
    betIncrement->setPosition(betDecrement->getPosition().x, betDecrement->getPosition().y - betDecrement->getSize().y - 10);

    sf::VertexArray *incrementTriangle = new sf::VertexArray(sf::Triangles, 3);

    incrementTriangle[0][0].position = sf::Vector2f(betIncrement->getPosition().x + 5, betIncrement->getPosition().y - 5);
    incrementTriangle[0][0].color = sf::Color::White;
    incrementTriangle[0][1].position = sf::Vector2f(betIncrement->getPosition().x + betIncrement->getSize().x / 2, betIncrement->getPosition().y - betIncrement->getSize().y + 5);
    incrementTriangle[0][1].color = sf::Color::White;
    incrementTriangle[0][2].position = sf::Vector2f(betIncrement->getPosition().x + betIncrement->getSize().x - 5, betIncrement->getPosition().y - 5);
    incrementTriangle[0][2].color = sf::Color::White;

    drawnElements[2].push_back(incrementTriangle);

    sf::RectangleShape *betButton = new sf::RectangleShape(betIndicator->getSize());
    shapes.push_back(betButton);
    drawnElements[2].push_back(betButton);

    interactables[betButton] = 'b';

    betButton->setFillColor(sf::Color(163, 163, 163, 255));

    betButton->setOrigin(betButton->getLocalBounds().width / 2, betButton->getLocalBounds().height / 2);
    betButton->setPosition(betIncrement->getPosition().x + betIncrement->getSize().x + 50 + betButton->getSize().x / 2, betIndicator->getPosition().y);

    sf::Text *betButtonText = new sf::Text;
    texts.push_back(betButtonText);
    drawnElements[2].push_back(betButtonText);

    betButtonText->setFont(font);
    betButtonText->setString("Call");
    betButtonText->setCharacterSize(50);
    betButtonText->setFillColor(sf::Color::White);

    betButtonText->setOrigin(betButtonText->getLocalBounds().width / 2, betButtonText->getLocalBounds().height / 2);
    betButtonText->setPosition(betButton->getPosition().x, betButton->getPosition().y - 10);

    sf::RectangleShape *foldButton = new sf::RectangleShape(betIndicator->getSize());
    shapes.push_back(foldButton);
    drawnElements[2].push_back(foldButton);

    interactables[foldButton] = 'f';

    foldButton->setFillColor(sf::Color(163, 163, 163, 255));

    foldButton->setOrigin(foldButton->getLocalBounds().width / 2, foldButton->getLocalBounds().height / 2);
    foldButton->setPosition(betButton->getPosition().x + betButton->getSize().x + 25, betIndicator->getPosition().y);

    sf::Text *foldButtonText = new sf::Text;
    texts.push_back(foldButtonText);
    drawnElements[2].push_back(foldButtonText);

    foldButtonText->setFont(font);
    foldButtonText->setString("Fold");
    foldButtonText->setCharacterSize(50);
    foldButtonText->setFillColor(sf::Color::White);

    foldButtonText->setOrigin(foldButtonText->getLocalBounds().width / 2, foldButtonText->getLocalBounds().height / 2);
    foldButtonText->setPosition(foldButton->getPosition().x, foldButton->getPosition().y - 10);

    int numOpponents = 3;

    for (int i = 0; i < numOpponents; i++) {
        for (int j = 0; j < 2; j++) {
            texture = new sf::Texture;

            textures.push_back(texture);
            texture->loadFromFile("./data/PixelPlayingCardsPack/back_red_basic_white.png");
            card = new sf::Sprite(*texture);
            card->setScale({1.5, 1.5});

            if (drawnElements[3].size() % 2 == 0) {
                card->setPosition((currentWindowSize.x / numOpponents) * i + (currentWindowSize.x / (numOpponents * 2)) - (card->getLocalBounds().width + 25) / 2, 25);
            } else {
                card->setPosition(sprites.back()->getPosition().x + 25, sprites.back()->getPosition().y);  // mult localBounds by 2 because of scale factor
            }

            sprites.push_back(card);
            drawnElements[3].push_back(card);
        }

        sf::Text *cpu = new sf::Text;
        texts.push_back(cpu);
        drawnElements[2].push_back(cpu);

        cpu->setFont(font);
        cpu->setString("PLACEHOLDER");
        cpu->setCharacterSize(20);
        cpu->setFillColor(sf::Color::White);

        cpu->setOrigin(cpu->getLocalBounds().width / 2, cpu->getLocalBounds().height / 2);
        cpu->setPosition((currentWindowSize.x / numOpponents) * i + (currentWindowSize.x / (numOpponents * 2)), 10 + cpu->getLocalBounds().height / 2 + sprites.back()->getPosition().y + (sprites.back()->getLocalBounds().height) * sprites.back()->getScale().y);
    }

    return;
}
    */
