#include "cardDeck.h"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

void printHand(vector<int> hand) {                  // 0-12: spades; 13-25: hearts; 26-38: diamonds; 39-51: clubs
    for (size_t i = 0; i < hand.size(); i++) {
        int card = hand[i];
        string suit;
        if (card < 13) suit = "♠";
        else if (card < 26) suit = "♥";
        else if (card < 39) suit = "♦";
        else suit = "♣";

        string cardVal;
        if (card % 13 == 0) {
            cardVal = "A";
        } else if (card % 13 < 10) {
            cardVal = to_string(card % 13 + 1);
        } else if (card % 13 == 10) {
            cardVal = "J";
        } else if (card % 13 == 11) {
            cardVal = "Q";
        } else {
            cardVal = "K";
        }

        cout << cardVal << suit << ' ';
    }

    cout << endl;
}

int main() {
    /*
    int numDecks = 5;
    CardDeck *deck = new CardDeck(numDecks);
    deck->shuffle();

    //cout << deck->deckSize << endl;

    vector<int> testHand;
    for (int i = 0; i < 208; i++) {
        deck->discard();
    }

    for (int i = 0; i < 52; i++) {
        testHand.push_back(deck->draw());
    }

    //printHand(testHand);

    //cout << deck->deckSize << endl;

    deck->addNewDeck();

    testHand.clear();
    for (int i = 0; i < 52; i++) {
        testHand.push_back(deck->draw());
    }

    //printHand(testHand);
    */


    sf::RenderWindow window(sf::VideoMode(800, 600), "Poker");
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(144);

    sf::Font font;
    font.loadFromFile("./data/minecraft_font.ttf");

    sf::Text text;
    text.setFont(font);
    text.setString("Hello World");
    text.setCharacterSize(80);
    text.setFillColor(sf::Color::White);

    sf::FloatRect textSpace = text.getLocalBounds();
    text.setOrigin(textSpace.width/2,textSpace.height/2);

    sf::RectangleShape rect(sf::Vector2f(textSpace.width, textSpace.height));
    rect.setOrigin(textSpace.width/2,textSpace.height/2);
    rect.setFillColor(sf::Color::Black);

    // first of 2 nested loops constituting the event loop
    while (window.isOpen())
    {
        // second of 2 nested loops constituting the event loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // best practice is to redraw the frame from scratch at each frame. each frame is drawn sequentially, with newer draws on top of older draws. always clear first (reset to background color)
        window.clear(sf::Color(36,156,68,255));

        text.setPosition(window.getSize().x/2,window.getSize().y/2 - 20);
        rect.setPosition(window.getSize().x/2,window.getSize().y/2);

        cout << text.getPosition().x << " " << text.getPosition().y << endl;

        window.draw(rect);
        window.draw(text);

        window.display();
    }

    return 0;
}