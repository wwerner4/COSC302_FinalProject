#include "cardNames.h"
#include <vector>
// gets the card name (like ace or what have you)
string getCardName(int cardValue) 
{
    if (cardValue < 0 || cardValue > 51) 
    {
        return "Invalid Card";
    }

    const vector<string> ranks = 
    {
        "Ace", "2", "3", "4", "5", "6", "7", "8", 
        "9", "10", "Jack", "Queen", "King"
    };

    const vector<string> suits = 
    {
        "Hearts", "Diamonds", "Clubs", "Spades"
    };

    return ranks[cardValue % 13] + " of " + suits[cardValue / 13];
}