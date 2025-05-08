#ifndef CARDDECK_H
#define CARDDECK_H

#include <vector>

// holds a deck of cards for use in the game
class CardDeck {
    public:
        CardDeck(int numDecks = 1);

        void shuffle();         // shuffle the deck
        void discard();         // remove a card from the deck
        void addNewDeck();      // add a new 52-card deck to the overall deck
        int draw();             // remove a card from the deck and return the card
        int deckSize;

    private:
        std::vector<int> deck;
};

#endif