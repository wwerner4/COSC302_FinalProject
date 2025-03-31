#include "cardDeck.h"
#include <iostream>

using namespace std;

void printHand(vector<int> hand) {                  // 0-12: spades; 13-25: hearts; 26-38: diamonds; 39-51: clubs
    for (int i = 0; i < hand.size(); i++) {
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
    int numDecks = 1;
    CardDeck *deck = new CardDeck(numDecks);
    deck->shuffle();

    cout << deck->deckSize << endl;

    vector<int> testHand;
    for (int i = 0; i < 52; i++) {
        testHand.push_back(deck->draw());
    }

    printHand(testHand);

    cout << deck->deckSize << endl;
}