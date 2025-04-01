#include "cardNames.h"
#include <iostream>

int main() 
{

    // testing

      // Ace of Hearts
    cout << "Card 0: " << getCardName(0) << endl;   
    // King of Hearts
    cout << "Card 12: " << getCardName(12) << endl;
    // Ace of Diamonds  
    cout << "Card 13: " << getCardName(13) << endl;  
    // King of Spades
    cout << "Card 51: " << getCardName(51) << endl;  
    // Invalid Card
    cout << "Card 52: " << getCardName(52) << endl;  
    return 0;
}