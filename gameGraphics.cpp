#include "gameGraphics.h"

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
    drawnElements.resize(1);

    sf::Text *title = new sf::Text;
    texts.push_back(title);
    drawnElements[0].push_back(title);

    title->setFont(font);
    title->setString("Poker (Working Title)");
    title->setCharacterSize(80);
    title->setFillColor(sf::Color::White);

    title->setOrigin(title->getLocalBounds().width / 2, title->getLocalBounds().height * 0.35);
    title->setPosition(currentWindowSize.x / 2, currentWindowSize.y / 5);


    return;
}
