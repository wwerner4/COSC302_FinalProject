#include "cardDeck.h"
#include <iostream>
#include <SFML/Window.hpp>

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


    sf::Window window(sf::VideoMode(800, 600), "My window");
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    return 0;
}