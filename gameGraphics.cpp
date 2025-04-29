#include "gameGraphics.h"
#include "cardDeck.h"

#include <iostream>
using namespace std;

GameGraphics::GameGraphics() {
    window.create(sf::VideoMode(1200, 900), "Poker");
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(144);

    font.loadFromFile("./data/minecraft_font.ttf");
    currentWindowSize.x = window.getSize().x;
    currentWindowSize.y = window.getSize().y;

    return;
}

GameGraphics::~GameGraphics() {
    clearWindow();
    return;
}

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

    interactables.clear();

    for (size_t i = 0; i < drawnElements.size(); i++) {
        for (size_t j = 0; j < drawnElements[i].size(); j++) {
        }
        drawnElements[i].clear();
    }
    drawnElements.clear();

    return;
}

void GameGraphics::closeWindow() {
    window.close();
    return;
}

void GameGraphics::resizeWindow(sf::Event resize) {
    sf::Vector2u newWindowSize(resize.size.width, resize.size.height);
    sf::Vector2f scaleFactor((float)newWindowSize.x/currentWindowSize.x, (float)newWindowSize.y/currentWindowSize.y);

    for (size_t i = 0; i < texts.size(); i++) {
        sf::Vector2f oldPosition = texts[i]->getPosition();
        texts[i]->setPosition(oldPosition.x * scaleFactor.x, oldPosition.y * scaleFactor.y);
    } for (size_t i = 0; i < shapes.size(); i++) {
        sf::Vector2f oldPosition = shapes[i]->getPosition();
        shapes[i]->setPosition(oldPosition.x * scaleFactor.x, oldPosition.y * scaleFactor.y);
    } for (size_t i = 0; i < sprites.size(); i++) {
        sf::Vector2f oldPosition = sprites[i]->getPosition();
        sprites[i]->setPosition(oldPosition.x * scaleFactor.x, oldPosition.y * scaleFactor.y);
    }

    // https://www.sfml-dev.org/tutorials/2.2/graphics-view.php#showing-more-when-the-window-is-resized
    sf::FloatRect visibleArea(0, 0, newWindowSize.x, newWindowSize.y);
    window.setView(sf::View(visibleArea));

    currentWindowSize = newWindowSize;

    return;
}

void GameGraphics::onClick() {
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f floatMousePos((float)mousePosition.x, (float)mousePosition.y);

    for (map<sf::Shape*,char>::iterator i = interactables.begin(); i != interactables.end(); i++) {

        sf::FloatRect interactableBounds = i->first->getGlobalBounds();
        if (interactableBounds.contains(floatMousePos)) {

            switch(i->second) {
                case 'p':
                    playScreen();
                    return;
                case 'e':
                    closeWindow();
                    return;
            }
        }
    }
}

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
    rect->setPosition(window.getSize().x / 2, window.getSize().y / 2 +20);

    drawnElements[0].push_back(rect);
    drawnElements[0].push_back(text);

    return;
}

string getCardNameModified(int cardValue) 
{
    if (cardValue < 0 || cardValue > 51) 
    {
        return "Invalid Card";
    }

    const vector<string> ranks = 
    {
        "ace", "2", "3", "4", "5", "6", "7", "8", 
        "9", "10", "jack", "queen", "king"
    };

    const vector<string> suits = 
    {
        "hearts", "diamonds", "clubs", "spades"
    };

    return "./data/PixelPlayingCardsPack/" + ranks[cardValue % 13] + "_" + suits[cardValue / 13] + "_white.png";
}

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

    playButtonText->setOrigin(playButtonText->getLocalBounds().width / 2, playButtonText->getLocalBounds().height/ 2);
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

    exitButtonText->setOrigin(exitButtonText->getLocalBounds().width / 2, exitButtonText->getLocalBounds().height/ 2);
    exitButtonText->setPosition(exitButton->getPosition().x, exitButton->getPosition().y - 10);

    CardDeck *displayDeck = new CardDeck;
    displayDeck->shuffle();
    vector<int> displayCards;

    for (int i = 0; i < 6; i++) {
        sf::Texture *texture = new sf::Texture;

        displayCards.push_back(displayDeck->draw());
        texture->loadFromFile(getCardNameModified(displayCards.back()));

        sf::Sprite *card = new sf::Sprite(*texture);

        textures.push_back(texture);
        sprites.push_back(card);
    }

    delete displayDeck;
    for (size_t i = 0; i < sprites.size(); i++) {
        sprites[i]->setOrigin(sprites[i]->getLocalBounds().width / 2, sprites[i]->getLocalBounds().height / 2);
        sprites[i]->setScale(2,2);

        if (i < sprites.size() / 2) {
            sprites[i]->setPosition(exitButton->getPosition().x - 474, exitButton->getPosition().y);
        }   else {
            sprites[i]->setPosition(exitButton->getPosition().x + 350, exitButton->getPosition().y);
        }

        sprites[i]->setOrigin(0, sprites[i]->getLocalBounds().height);
        sprites[i]->rotate((350) + (15 * (i % 3)));

        drawnElements[2].push_back(sprites[i]);
    }

    return;
}

void GameGraphics::playScreen() {

    clearWindow();
    drawnElements.resize(3);    // table cards, player hand, interface, cpu hands

    CardDeck *testDeck = new CardDeck();

    testDeck->shuffle();

    vector<int> tableCards;
    sf::Sprite *card;
    sf::Texture *texture;
    string cardName;

    for (int i = 0; i < 5; i++) {
        tableCards.push_back(testDeck->draw());

        cardName = getCardNameModified(tableCards.back());

        texture = new sf::Texture;

        textures.push_back(texture);
        texture->loadFromFile(cardName);
        card = new sf::Sprite(*texture);
        card->setScale({2.5,2.5});

        card->setOrigin(card->getLocalBounds().width / 2, card->getLocalBounds().height / 2);
        if (drawnElements[0].size() == 0) {
            card->setPosition(currentWindowSize.x / 2, currentWindowSize.y / 2);
        } else {
            card->setPosition(sprites.back()->getPosition().x + 25, sprites.back()->getPosition().y);
        }

        sprites.push_back(card);
        drawnElements[0].push_back(card);
    }

    vector<int> playerCards;

    for (int i = 0; i < 2; i++) {
        playerCards.push_back(testDeck->draw());

        cardName = getCardNameModified(playerCards.back());

        texture = new sf::Texture;

        textures.push_back(texture);
        texture->loadFromFile(cardName);
        card = new sf::Sprite(*texture);
        card->setScale({2.5,2.5});

        card->setOrigin(card->getLocalBounds().width / 2, card->getLocalBounds().height / 2);
        if (drawnElements[1].size() == 0) {
            card->setPosition(currentWindowSize.x - 300, currentWindowSize.y - 150);
        } else {
            card->setPosition(sprites.back()->getPosition().x + (sprites.back()->getLocalBounds().width)*2.5 + 25, sprites.back()->getPosition().y);      // mult localBounds by 2 because of scale factor
        }

        sprites.push_back(card);
        drawnElements[1].push_back(card);
    }


    return;
}
