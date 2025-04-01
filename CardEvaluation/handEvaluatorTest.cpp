// testHandEvaluator.cpp
#include "handEvaluator.h"
#include <iostream>
#include <vector>

using namespace std;

int main() 
{
    cout << "Testing Poker Hand Evaluator" << endl;
    
    // Invalid hand (too small) since in poker we have to have a minimum of 5 cards
    vector<int> invalidHand = {1, 2, 3, 4};
    cout << "Invalid hand test: " << evaluateHand(invalidHand) << endl;
    
    //  High card - Ace high
    vector<int> highCard = {0, 15, 30, 45, 12};  // 2 Clubs, 4 Diamonds, 6 Hearts, 8 Spades, A Clubs
    cout << "High card test: " << evaluateHand(highCard) << endl;
    
    // One pair - pair of Kings
    vector<int> onePair = {11, 24, 2, 16, 39};  // K Clubs, K Diamonds, 4 Clubs, 5 Diamonds, 2 Hearts
    cout << "One pair test: " << evaluateHand(onePair) << endl;
    
    // Two pair - Aces and Queens
    vector<int> twoPair = {12, 25, 10, 23, 5};  // A Clubs, A Diamonds, Q Clubs, Q Diamonds, 7 Clubs
    cout << "Two pair test: " << evaluateHand(twoPair) << endl;
    
    //Three of a kind - three Jacks
    vector<int> threeOfAKind = {9, 22, 35, 3, 17};  // J Clubs, J Diamonds, J Hearts, 5 Clubs, 6 Diamonds
    cout << "Three of a kind test: " << evaluateHand(threeOfAKind) << endl;
    
    //Straight - 5 to 9
    vector<int> straight = {3, 17, 31, 45, 7};  // 5 Clubs, 6 Diamonds, 7 Hearts, 8 Spades, 9 Clubs
    cout << "Straight test: " << evaluateHand(straight) << endl;
    
    //Low-Ace straight - A-2-3-4-5
    vector<int> lowStraight = {12, 0, 14, 28, 42};  // A Clubs, 2 Clubs, 3 Diamonds, 4 Hearts, 5 Spades
    cout << "Low-Ace straight test: " << evaluateHand(lowStraight) << endl;
    
    return 0;
}