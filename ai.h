#ifndef AI_H
#define AI_H
#include <vector>
#include <string>
using namespace std;

bool aiShouldFold(int player, string handType, int betToCall, int pot, const vector<int>& chips);
int aiChosenBet(int player, string handType, int betToCall, int pot, const vector<int>& chips);
float calculatePotOdds(int player, int betToCall, int pot);
float evaluateHandStrength(string handType);
int getPotSize();
// int getCurrentBetToCall(int betAmount); //getter method for call amt (checkBet var)
#endif // AI_H
