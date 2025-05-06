#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <string>
#include "ai.h"

using namespace std;

// Forward declarations
float evaluateHandStrength(string handType);
float calculatePotOdds(int player, int betToCall, int pot);

float calculateGini(const vector<int>& chips) {
    if (chips.empty()) return 0.0;
    
    vector<int> sorted = chips;
    sort(sorted.begin(), sorted.end());
    
    float n = static_cast<float>(sorted.size());
    float sum = 0.0f;
    float total = accumulate(sorted.begin(), sorted.end(), 0.0f);
    
    for (size_t i = 0; i < n; i++) {
        sum += (n - i) * sorted[i];
    }
    
    // gini formula
    return (2.0f * sum) / (n * total) - (n + 1.0f) / n;
}

float evaluateHandStrength(string handType) {
    // higher value = better hand
    // probabilities for 5-7 player getting a certain hand
    map<string, float> probabilityHandStrength;
    probabilityHandStrength["One Pair"] = 0.4383f;
    probabilityHandStrength["High Card"] = 0.75f; // only one that is defined by me (to make high card worse than one pair)
    probabilityHandStrength["Two Pair"] = 0.235f;
    probabilityHandStrength["Three of a Kind"] = 0.0483f;
    probabilityHandStrength["Straight"] = 0.0462f;
    probabilityHandStrength["Flush"] = 0.0303f;
    probabilityHandStrength["Four of a Kind"] = 0.0206f;
    probabilityHandStrength["Straight Flush"] = 0.02f; // to make SF better than four of a kind
    probabilityHandStrength["Royal Flush"] = 0.0032f;
    
    // normalizing such that better card types are higher value
    return 1.0f - probabilityHandStrength[handType];
}

float calculatePotOdds(int player, int betToCall, int pot) {
    if (betToCall == 0) {
        return 1.0f;
    }
    
    return static_cast<float>(pot) / (pot + betToCall);
}

bool aiShouldFold(int player, string handType, int betToCall, int pot, const vector<int>& chips)
{
    float handStrength = evaluateHandStrength(handType);
    float potOdds = calculatePotOdds(player, betToCall, pot);
    
    float gini = calculateGini(chips);
    float handThreshold = 0.5f - (gini * 0.01);
    float potThreshold = 0.3f - (gini * 0.01);
    
    // Decision tree:
    if (handStrength > handThreshold) 
    {
        return false; // Raise
    } else if (potOdds > potThreshold) 
    {
        return false; // Call
    } else {
        return true;  // Fold
    }
}

int aiChosenBet(int player, string handType, int betToCall, int pot, const vector<int>& chips) 
{
    float handStrength = evaluateHandStrength(handType);
    float potOdds = calculatePotOdds(player, betToCall, pot);
    int currentBet = betToCall;
    
    float gini = calculateGini(chips);
    if (handStrength > 0.5f - gini * 0.01) 
    {
        // Raise by 20% of pot
        int raiseAmount = static_cast<int>(pot * 0.20);
        return currentBet + raiseAmount;
    } else if (potOdds > 0.4 - gini* .01) 
    {
        // Just call
        return currentBet;
    } else {
        // doesn't bet here
        return 0;
    }
}
