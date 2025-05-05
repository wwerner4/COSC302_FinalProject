#include "handEvaluator.h"
#include <algorithm>
#include <unordered_map>

namespace std 
{
    string getCardName(int cardValue) 
    {
        if (cardValue < 0 || cardValue > 51) 
        {
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

    string evaluateHand(const vector<int>& hand) 
    {
        // confirm the hand size
        if (hand.size() != 5) 
            return "Invalid hand (needs 5 cards)";
        
        // get and sort ranks (0-12 where 0=ace, 1=2, 2=3, ..., 12=king)
        vector<int> ranks;
        unordered_map<int, int> rankFrequency;
        unordered_map<int, int> suitFrequency;
        
        for (int card : hand) 
        {
            if (card < 0 || card > 51)
                return "Invalid card value";
                
            int rank = card % 13; // 0=ace, 1=2, ..., 12=king
            int suit = card / 13; // 0=hearts, 1=diamonds, 2=clubs, 3=spades
            
            ranks.push_back(rank);
            rankFrequency[rank]++;
            suitFrequency[suit]++;
        }
        sort(ranks.begin(), ranks.end());
        
        // Check for flush (all cards of the same suit)
        bool isFlush = false;
        for (const auto& [suit, count] : suitFrequency) 
        {
            if (count == 5) 
            {
                isFlush = true;
                break;
            }
        }
        
        // Count pairs, three of a kind, four of a kind
        int pairCount = 0;
        bool hasThreeOfAKind = false;
        bool hasFourOfAKind = false;
        
        for (const auto& [rank, count] : rankFrequency) 
        {
            if (count == 2) 
            {
                pairCount++;
            }
            else if (count == 3) 
            {
                hasThreeOfAKind = true;
            }
            else if (count == 4) 
            {
                hasFourOfAKind = true;
            }
        }
        
        // Check for straight (sequential ranks)
        bool isStraight = true;
        for (size_t i = 1; i < ranks.size(); i++) 
        {
            if (ranks[i] != ranks[i-1] + 1) 
            {
                isStraight = false;
                break;
            }
        }
        
        // Special case for Ace-low straight (A-2-3-4-5)
        // Ace is 0, 2 is 1, 3 is 2, 4 is 3, 5 is 4
        bool isAceLowStraight = (ranks[0] == 0 && ranks[1] == 1 && 
                                 ranks[2] == 2 && ranks[3] == 3 && 
                                 ranks[4] == 4);
                                 
        // Special case for Ace-high straight (10-J-Q-K-A)
        // 10 is 9, J is 10, Q is 11, K is 12, A is 0
        bool isAceHighStraight = (ranks[0] == 0 && ranks[1] == 9 && 
                                  ranks[2] == 10 && ranks[3] == 11 && 
                                  ranks[4] == 12);
        
        isStraight = isStraight || isAceLowStraight || isAceHighStraight;
        
        // Evaluate hand ranking
        if (isFlush && isStraight) {
            // Check for royal flush (10-J-Q-K-A of same suit)
            if (ranks[0] == 0 && ranks[1] == 9 && ranks[2] == 10 && 
                ranks[3] == 11 && ranks[4] == 12) {
                return "Royal Flush";
            }
            return "Straight Flush";
        }
        
        if (hasFourOfAKind) {
            return "Four of a Kind";
        }
        
        if (hasThreeOfAKind && pairCount == 1) {
            return "Full House";
        }
        
        if (isFlush) {
            return "Flush";
        }
        
        if (isStraight) {
            return "Straight";
        }
        
        if (hasThreeOfAKind) {
            return "Three of a Kind";
        }
        
        if (pairCount == 2) {
            return "Two Pair";
        }
        
        if (pairCount == 1) {
            return "One Pair";
        }
        
        return "High Card";
    }
    
int handRank(const string& handType) 
{
    // Return a number rank for each hand type (higher is better)
    if (handType == "Royal Flush")     return 10;
    if (handType == "Straight Flush")  return 9;
    if (handType == "Four of a Kind")  return 8;
    if (handType == "Full House")      return 7;
    if (handType == "Flush")           return 6;
    if (handType == "Straight")        return 5;
    if (handType == "Three of a Kind") return 4;
    if (handType == "Two Pair")        return 3;
    if (handType == "One Pair")        return 2;
    if (handType == "High Card")       return 1;
    
    // wont happen
    return 0;
}vector<int> getSortedRanks(const vector<int>& hand) 
{
    // Extract ranks from card values
    vector<int> ranks;
    unordered_map<int, int> rankFrequency;
    
    for (int card : hand) 
    {
        int rank = card % 13; // 0=ace, 1=2, ..., 12=king
        ranks.push_back(rank);
        rankFrequency[rank]++;
    }
    
    // For high card comparison, we need ranks sorted by frequency (highest to lowest)
    // and then by rank value (highest to lowest)
    
    // Create a vector of pairs (frequency, rank)
    vector<pair<int, int>> ranksByFrequency;
    for (const auto& [rank, frequency] : rankFrequency) 
    {
        ranksByFrequency.push_back({frequency, rank});
    }
    
    // Sort the pairs by frequency (descending) and then by rank (descending)
    sort(ranksByFrequency.begin(), ranksByFrequency.end(), 
        [](const pair<int, int>& a, const pair<int, int>& b) 
        {
            if (a.first != b.first) {
                return a.first > b.first; 
            }
            
            int rankA = a.second;
            int rankB = b.second;
            
            if (rankA == 0) 
            {
                rankA = 13; 
            }
            
            if (rankB == 0) 
            {
                rankB = 13;  
            }
            
            return rankA > rankB; 
        });
    
    // get sorted ranks
    vector<int> sortedRanks;
    for (const auto& [frequency, rank] : ranksByFrequency) 
    {
        sortedRanks.push_back(rank);
    }
    
    return sortedRanks;
}
    int handWinner(const std::vector<std::vector<int>>& playerHands) 
    {
        int bestIndex = 0;
        int bestRank = handRank(evaluateHand(playerHands[0]));
        vector<int> bestSorted = getSortedRanks(playerHands[0]);
    
        for (int i = 1; i < playerHands.size(); ++i) 
        {
            int currentRank = handRank(evaluateHand(playerHands[i]));
    
            if (currentRank > bestRank) 
            {
                bestRank = currentRank;
                bestIndex = i;
                bestSorted = getSortedRanks(playerHands[i]);
            }
            else if (currentRank == bestRank) 
            {
                vector<int> currentSorted = getSortedRanks(playerHands[i]);
    
                // Tie-breaker: compare sorted ranks
                for (size_t j = 0; j < currentSorted.size(); ++j) 
                {
                    if (j >= bestSorted.size()) break;
    
                    if (currentSorted[j] > bestSorted[j]) 
                    {
                        bestIndex = i;
                        bestSorted = currentSorted;
                        break;
                    }
                    else if (currentSorted[j] < bestSorted[j]) 
                    {
                        break; // existing best is better
                    }
                    // If equal, keep checking
                }
            }
        }
    
        return bestIndex;
    }
    

// this is all wrong...

// string evaluateHand(const vector<int>& hand) 
// {
//     // confirm the hand size
//     
    
//     // get and sort ranks (0-12 where 0=2, 12=Ace)
//     vector<int> ranks;
//     unordered_map<int, int> rankFrequency;
    
//   
//     sort(ranks.begin(), ranks.end());
    
//     // Count pairs and three of a kind
//     int pairCount = 0;
//     bool hasThreeOfAKind = false;
    
//     // just checking for the frequency that a certain card appears
//     for (const auto& [rank, count] : rankFrequency) 
//     {
//         
//         if (count == 3) 
//         {
//             hasThreeOfAKind = true;
//         }
//     }
    
//     // Return hand type
//     if (hasThreeOfAKind) 
//     {
//         return "Three of a Kind";
//     }
//     if (pairCount == 2) 
//     {
//         return "Two Pair";
//     }
//     if (pairCount == 1) return "One Pair";
//     if (isStraight || isLowStraight) return "Straight";
//     return "High Card";
// }

}
