#include "cardDeck.h"
#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;

// init the deck
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

// randomize the positions of cards in the deck
void CardDeck::shuffle() {
    vector<int> shuffledDeck;
    srand(time(0));             // init rand

    while (deck.size() > 0) {
        int randElement = rand() % deck.size();           // select random card
        shuffledDeck.push_back(deck[randElement]);        // place random card in shuffled deck

        // get iterator to the selected random card in the pre-shuffle deck
        vector<int>::iterator pos = deck.begin();
        for (int i = 0; i < randElement; i++) {
            pos++;
        }
        deck.erase(pos);                // erase this value from the pre-shuffle deck
    }

    // replace pre-shuffle deck with shuffled deck
    deck = shuffledDeck;
    return;
}

// remove the top card from the deck
void CardDeck::discard() {
    deck.pop_back();
    deckSize--;

    return;
}

// add a 52-card deck to the larger deck
void CardDeck::addNewDeck() {
    deck.resize(deckSize + 52, -1);

    // add new cards
    for (int i = 0; i < 52; i++) {
        deck[deckSize + i] = i;
    }

    deckSize += 52;
    return;
}

// remove a card from the deck and return its value
int CardDeck::draw() {
    deckSize--;
    int card = deck.back();
    deck.pop_back();
    return card;
}