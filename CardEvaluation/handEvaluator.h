#ifndef HAND_EVALUATOR_H
#define HAND_EVALUATOR_H

#include <vector>
#include <string>

namespace std 
{
    // returns the hand type as a string
    string evaluateHand(const vector<int>& hand);
    // returns the card name (like Ace of Spades_
    string getCardName(int cardValue);
    // return the hand rank
    int handRank(const std::string& handType);
    // returns the card numbers in sorted order
    std::vector<int> getSortedRanks(const std::vector<int>& hand);
    // considers the entire table's hands and returns the index of the player who won the game
    int handWinner(const std::vector<std::vector<int>>& playerHands);
}

#endif 
