#include <string>
#include <vector>
#include <map>
#include "ai.h"
using namespace std;

bool aiShouldFold(int player, string handType, int betToCall, int pot) 
{
    float handStrength = evaluateHandStrength(handType);  
    float potOdds = calculatePotOdds(player, betToCall, pot);           

    // Decision tree:
    if (handStrength > 0.7) 
    {
        return false; // Raise 
    } else if (potOdds > 0.4) 
    {
        return false; // Call
    } else {
        if (betToCall > 0) {
            return true;  // Fold
        }
        return false;
    }
}
float evaluateHandStrength(string handType)
{
    // higher value = worse
    // probabilities for 5-7 player getting a certain hand is here:
     // https://de.wikipedia.org/wiki/Texas_Hold%E2%80%99em?utm_source=chatgpt.com
    map<string, float> probabilityHandStrength;
    probabilityHandStrength["One Pair"] = .4383;
    probabilityHandStrength["High Card"] = .75; // only one that is defined by me (to make high card worse than one pair)
    probabilityHandStrength["Two Pair"] = .235;
    probabilityHandStrength["Three of a Kind"] = .0483;
    probabilityHandStrength["Straight"] = .0462;
    probabilityHandStrength["Flush"] = .0303;
    probabilityHandStrength["Four of a Kind"] = .0206;
    probabilityHandStrength["Straight Flush"] = .02; // to make SF better than four of a kind
    // probabilityHandStrength["Royal Straight Flush"] = .0303
    probabilityHandStrength["Royal Flush"] = .0032;

    //normalizin such that better card types are higher value
    return 1.0f - probabilityHandStrength[handType];

}
int aiChosenBet(int player, string handType, int betToCall, int pot) 
{
    float handStrength = evaluateHandStrength(handType);
    //float potOdds = calculatePotOdds(player, betToCall, pot);
    int currentBet = betToCall;  

    if (handStrength > 0.7f) 
    {
        // Raise by 20% of pot
        int raiseAmount = ((static_cast<int>(pot * 0.20) + 100) / 100) * 100;
        return currentBet + raiseAmount;
    } else
    {
        // Just call
        return currentBet;
    } //else {
        // doesnt bet here
       // return 0;
    //}
}
float calculatePotOdds(int player, int betToCall, int pot) 
{
    int callAmount = betToCall;  // somethin like 20 chips
    // int pot = getPotSize();                        // some like 100 chips

    if (callAmount == 0) 
    {
        return 1.0f;
    }
    
    return static_cast<float>(callAmount) / (pot + callAmount);
}

// gotta fix this 
int getPotSize()
{
    return -1;
}
