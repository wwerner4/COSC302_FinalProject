#include "cardNames.h"
#include <vector>
// gets the card name (like ace or what have you)
string getCardName(int cardValue) {
    if (cardValue < 0 || cardValue > 51) {
        return "Invalid Card";
    }

    const vector<string> ranks =
        {
            "ace", "2", "3", "4", "5", "6", "7", "8",
            "9", "10", "jack", "queen", "king"};

    const vector<string> suits =
        {
            "hearts", "diamonds", "clubs", "spades"};

    return "./data/PixelPlayingCardsPack/" + ranks[cardValue % 13] + "_" + suits[cardValue / 13] + "_white.png";
}