#ifndef CARDDECK_H
#define CARDDECK_H

#include <vector>

class CardDeck {
    int deckSize;
    std::vector<int> deck;

    CardDeck(int numDecks = 1);

    void shuffle();
    void addNewDeck();
    int draw();
};

#endif