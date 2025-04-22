#include "cardDeck.h"
#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;

CardDeck::CardDeck(int numDecks) {
    deckSize = numDecks*52;
    deck.resize(deckSize, -1);

    // set initial values of cards in the deck. The 'deck' can be more than one 52-card deck shuffled together
    for (int i = 0; i < 52; i++) {
        for (int j = 0; j < numDecks; j++) {
            deck[j*52 + i] = i;
        }
    }
}

void CardDeck::shuffle() {
    vector<int> shuffledDeck;

    while (deck.size() > 0) {
        int randElement = rand()%deck.size();
        shuffledDeck.push_back(deck[randElement]);

        vector<int>::iterator pos = deck.begin();
        for (int i = 0; i < randElement; i++) {
            pos++;
        }
        deck.erase(pos);
    }

    deck = shuffledDeck;
    return;
}

void CardDeck::discard() {
    deck.pop_back();
    deckSize--;

    return;
}

void CardDeck::addNewDeck() {
    deck.resize(deckSize + 52, -1);

    for (int i = 0; i < 52; i++) {
        deck[deckSize + i] = i;
    }

    deckSize += 52;
    return;
}

int CardDeck::draw() {
    deckSize--;
    int card = deck.back();
    deck.pop_back();
    return card;
}

void printHand(vector<int> hand) {  // 0-12: spades; 13-25: hearts; 26-38: diamonds; 39-51: clubs
    for (size_t i = 0; i < hand.size(); i++) {
        int card = hand[i];
        string suit;
        if (card < 13)
            suit = "♠";
        else if (card < 26)
            suit = "♥";
        else if (card < 39)
            suit = "♦";
        else
            suit = "♣";

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