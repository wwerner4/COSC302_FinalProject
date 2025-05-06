#ifndef HAND_EVALUATOR_H
#define HAND_EVALUATOR_H

#include <vector>
#include <string>

namespace std 
{
    string evaluateHand(const vector<int>& hand);
    string getCardName(int cardValue);
    int handRank(const std::string& handType);
    std::vector<int> getSortedRanks(const std::vector<int>& hand);
    int handWinner(const std::vector<std::vector<int>>& playerHands);
}

#endif 
