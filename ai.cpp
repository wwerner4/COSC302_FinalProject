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

// Here are a few of the references I used:
// How does gini apply to decision tree (what we are using for the basic AI): https://www.upgrad.com/blog/gini-index-for-decision-trees/
// A coding example of how gini is used: https://github.com/petersenpeter/CellExplorer/discussions/92
// What is gini and why is it useful?: https://en.wikipedia.org/wiki/Gini_coefficient

// basically, we are calculating the gini coefficient to measure the chip dispartity among players. (Since the chip knowledge of other players
// is known to all players in the game this doesnt create data leakage for our AI)
float calculateGini(const vector<int>& chips) {
    // if theres no chips on the board (Which shouldnt happen but just in case)
    if (chips.empty()) return 0.0;

    vector<int> sorted = chips;
    sort(sorted.begin(), sorted.end());

    //get the amount of players in play
    float n = static_cast<float>(sorted.size());
    // get the total chip size in the game
    float sum = 0.0f;
    float total = accumulate(sorted.begin(), sorted.end(), 0.0f);
    
    for (size_t i = 0; i < n; i++) {
        //getting the sum for the chips in play
        sum += (n - i) * sorted[i];
    }
    
    // this is the gini formula (measures the chip inequality in our case
    // the formula is found in the wikipedia article: https://en.wikipedia.org/wiki/Gini_coefficient 
    return (2.0f * sum) / (n * total) - (n + 1.0f) / n;
}

// returns the probability of getting a certain hand. Exclusing Straight Flush and High Card, their 
// respective probabilities essentially state how good of a hand they are. We are basically considering the high card and straight flush 
// as hyperparameters since their actual probabilties are not representative of their value.
// numbers found here: https://en.wikipedia.org/wiki/Poker_probability
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

// pot odds basically tell us the "economic value" of a call for a player.
// basically this helps with risk-adversion with the AI
float calculatePotOdds(int player, int betToCall, int pot) {
    if (betToCall == 0) {
        return 1.0f;
    }
    
    return static_cast<float>(pot) / (pot + betToCall);
}

// decides whether the AI should fold (forfeit all their bets to the pot and stop playing this round)
bool aiShouldFold(int player, string handType, int betToCall, int pot, const vector<int>& chips)
{
    // get the hand strength and pot odds for the current turn
    float handStrength = evaluateHandStrength(handType);
    float potOdds = calculatePotOdds(player, betToCall, pot);

    // measure the chip inequality on the table
    float gini = calculateGini(chips);
    // .7, .6 are hyperparamets in our AI
    float handThreshold = 0.7f - (gini * 0.01);
    float potThreshold = 0.6f - (gini * 0.01);
    
    // Decision tree part of the AI:
    if (handStrength > handThreshold) 
    {
        return false; // Raise
    } else if (potOdds > potThreshold || (handStrength + 0.3 > handThreshold && potOdds + 0.2 > potThreshold)) 
    {
        return false; // Call
    } else {
        return true;  // Fold
    }
}

// decide whether the AI should bet or not
int aiChosenBet(int player, string handType, int betToCall, int pot, const vector<int>& chips) 
{
    float handStrength = evaluateHandStrength(handType);
    int currentBet = betToCall;
    
    float gini = calculateGini(chips);
    // .5 is hyper
    if (handStrength > 0.5f - gini * 0.01) 
    {
        // Change up the raise depending on how strong our hand is
        int raiseAmount = (static_cast<int>(pot * (0.20 + handStrength / 4) + 100) / 100) * 100;

        // current bet is the amount to call. If we want to put in more than the amount to call for a turn, then we technically have to do a "raise" 
        if (raiseAmount > currentBet) {
            return raiseAmount;
        }
        return currentBet;
    } else
    {
        // Just call
        return currentBet;
    }
}
