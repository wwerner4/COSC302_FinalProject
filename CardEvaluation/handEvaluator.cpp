#include "handEvaluator.h"
#include <algorithm>
#include <unordered_map>

namespace std 
{

string evaluateHand(const vector<int>& hand) 
{
    // confirm the hand size
    if (hand.size() != 5) 
        return "Invalid hand (needs 5 cards)";
    
    // get and sort ranks (0-12 where 0=2, 12=Ace)
    vector<int> ranks;
    unordered_map<int, int> rankFrequency;
    
    for (int card : hand) 
    {
        int rank = card % 13;
        ranks.push_back(rank);
        rankFrequency[rank]++;
    }
    sort(ranks.begin(), ranks.end());
    
    // Count pairs and three of a kind
    int pairCount = 0;
    bool hasThreeOfAKind = false;
    
    // just checking for the frequency that a certain card appears
    for (const auto& [rank, count] : rankFrequency) 
    {
        if (count == 2) 
        {
            pairCount++;
        }
        if (count == 3) 
        {
            hasThreeOfAKind = true;
        }
    }
    
    // Check for straight (including A-2-3-4-5)
    bool isStraight = true;
    for (size_t i = 1; i < ranks.size(); i++) 
    {
        // so basically since they are numbered next to each other,
        // we can just check if the int value of the next one in the sorted list is + 1 from the previous one
        // ex: if we have 2,3,4,5,6 -> this would corresond numberically to ranks[0], ranks[0] + 1, ranks[0] + 2.. etc.
        if (ranks[i] != ranks[i-1] + 1) 
        {
            isStraight = false;
            break;
        }
    }
    // check if we are at the low straight
    bool isLowStraight = (ranks[0] == 0 && ranks[1] == 1 && 
                          ranks[2] == 2 && ranks[3] == 3 && 
                          ranks[4] == 12);
    
    // Return hand type
    if (hasThreeOfAKind) 
    {
        return "Three of a Kind";
    }
    if (pairCount == 2) 
    {
        return "Two Pair";
    }
    if (pairCount == 1) return "One Pair";
    if (isStraight || isLowStraight) return "Straight";
    return "High Card";
}

}