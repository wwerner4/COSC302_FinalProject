bool GameState::aiShouldFold(int player) 
{
    float handStrength = evaluateHandStrength(player);  
    float potOdds = calculatePotOdds(player);           

    // Decision tree:
    if (handStrength > 0.7) 
    {
        return false; // Raise 
    } else if (potOdds > 0.4) 
    {
        return false; // Call
    } else {
        return true;  // Fold
    }
}
int GameState::aiChosenBet(int player) 
{
    float handStrength = evaluateHandStrength(player);
    float potOdds = calculatePotOdds(player);
    int currentBet = getCurrentBetToCall(player);  

    if (handStrength > 0.7f) 
    {
        // Raise by 20% of pot
        int raiseAmount = static_cast<int>(getPotSize() * 0.20);
        return currentBet + raiseAmount;
    } else if (potOdds > 0.4) 
    {
        // Just call
        return currentBet;
    } else {
        // doesnt bet here
        return 0;
    }
}
float GameState::calculatePotOdds(int player) 
{
    int callAmount = getCurrentBetToCall(player);  // somethin like 20 chips
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
