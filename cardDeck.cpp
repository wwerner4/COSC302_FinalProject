#include <cardDeck.h>
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