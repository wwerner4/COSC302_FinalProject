#ifndef CARDDECK_H
#define CARDDECK_H

#include <vector>

class CardDeck {
    public:
        CardDeck(int numDecks = 1);

        void shuffle();
        void discard();
        void addNewDeck();
        int draw();
        int deckSize;

    private:
        std::vector<int> deck;
};

#endif