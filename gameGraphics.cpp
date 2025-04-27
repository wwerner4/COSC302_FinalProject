#include "gameGraphics.h"

#include <iostream>
using namespace std;

struct GameGraphics::interactible{
    sf::Shape *element;
    char action;
};

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

    for (size_t i = 0; i < interactibles.size(); i++) {
        delete interactibles[i]->element;
        delete interactibles[i];
    }
    interactibles.clear();

    for (size_t i = 0; i < drawnElements.size(); i++) {
        for (size_t j = 0; j < drawnElements[i].size(); j++) {
            delete drawnElements[i][j];
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

    sf::Vector2f interactibleOrigin, interactiblePosition;
    for (size_t i = 0; i < interactibles.size(); i++) {
        sf::Shape *button = interactibles[i]->element;

        sf::FloatRect interactibleBounds = button->getGlobalBounds();
        if (interactibleBounds.contains(floatMousePos)) {

            switch(interactibles[i]->action) {
                case 'p':
                    playScreen();
                    break;
                case 'e':
                    closeWindow();
                    break;
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

void GameGraphics::titleScreen() {
    clearWindow();
    drawnElements.resize(2);

    sf::Text *title = new sf::Text;
    texts.push_back(title);
    drawnElements[0].push_back(title);

    title->setFont(font);
    title->setString("Poker (Working Title)");
    title->setCharacterSize(80);
    title->setFillColor(sf::Color::White);

    title->setOrigin(title->getLocalBounds().width / 2, title->getLocalBounds().height / 2);
    title->setPosition(currentWindowSize.x / 2, currentWindowSize.y / 5);

    sf::RectangleShape *playButton = new sf::RectangleShape(sf::Vector2f(500, 75));
    shapes.push_back(playButton);
    drawnElements[1].push_back(playButton);

    interactible *button = new interactible;
    button->element = playButton;
    button->action = 'p';

    interactibles.push_back(button);

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

    button = new interactible;
    button->element = exitButton;
    button->action = 'e';

    interactibles.push_back(button);

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

    return;
}

void GameGraphics::playScreen() {
    return;
}
