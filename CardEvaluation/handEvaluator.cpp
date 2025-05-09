#include "handEvaluator.h"

#include <algorithm>
#include <iostream>
#include <unordered_map>

namespace std {
// gets the value of the card and returns it. Since we are considering all card from 0-51 (52 cards in a deck) we distinguish this way
string getCardName(int cardValue) {
    if (cardValue < 0 || cardValue > 51) {
        return "Invalid Card";
    }

    // the ranks are the types of cards
    const vector<string> ranks =
        {
            "ace", "2", "3", "4", "5", "6", "7", "8",
            "9", "10", "jack", "queen", "king"};

    // the suit types in poker
    const vector<string> suits =
        {
            "hearts", "diamonds", "clubs", "spades"};

    // we use the modulus operator to determine the card's value from 0-12 (where we consider 0 as Ace and 12 as King). This is the Rank
    // the Suit is determined by the card number (like the card's place in the deck, divided by 13). Consider the 0th card, 
    // 0/13 = 0. We assign 0 to be hearts, 1 to be diamond, 2 to be club, and 3 is spade. this way we can separate the 4 different card types
    // There are 13 of each of the 4 card types (13 * 4 = 52 total cards in a deck). This is our easy way to reference which card and type it is.
    return "./data/PixelPlayingCardsPack/" + ranks[cardValue % 13] + "_" + suits[cardValue / 13] + "_white.png";
}


// This function essentially simply returns the hand type in poker. This can be used on any amount of cards
// for Example, If a player has Ace of Spades and Ace of Diamonds in their hand (and this is the only match found),
// the program would return "One Pair"
// Can return all hand types in poker.
string evaluateHand(const vector<int>& hand) {
    //  the hand size
    // this isnt important anymore
    // if (hand.size() != 5)
    //     return "Invalid hand]";

    // get and sort ranks (0-12 where 0=ace, 1 is actually 2, 2=3, .. 12 = king)
    vector<int> ranks;
    // map to store the amount of ranks we see 
    unordered_map<int, int> rankFrequency;
    // map to store the suit counter (consider 5 cards of any suit type should by default be at least the value of a flush)
    unordered_map<int, int> suitFrequency;

    // check each card in the player's hand
    for (int card : hand) {
        // out of bounds....
        if (card < 0 || card > 51)
            return "Invalid card value";
    
        int rank = card % 13;  // 0=ace, 1  is 2, etc,  12=king
        int suit = card / 13;  // 0=hearts, 1= diamonds, 2 = clubs, 3=spades (read getCardName documentation to see why)

         // dont add duplicate rank to ranks
        if (find(ranks.begin(), ranks.end(), rank) == ranks.end()) 
        { 
            ranks.push_back(rank);
        }
        // add each of the card's features to their respective map counter (we need to keep track in case of Flush, Straight flush etc)
        rankFrequency[rank]++;
        suitFrequency[suit]++;
    }
    // sorting just makes it easier to parse for us
    sort(ranks.begin(), ranks.end());

    // Check for flush (all cards of the same suit)
    bool isFlush = false;
    for (size_t i = 0; i < suitFrequency.size(); i++) 
    {
        // by definition if there is a suit type that the player's hand has at least 5 of, then it's a flush
        if (suitFrequency[i] >= 5) {
            isFlush = true;
            break;
        }
    }

    // Count pairs, three of a kind, four of a kind
    int pairCount = 0;
    // flag cehcks
    bool hasThreeOfAKind = false;
    bool hasFourOfAKind = false;

    
    for (unordered_map<int, int>::iterator i = rankFrequency.begin(); i != rankFrequency.end(); i++) 
    {
        
        if (i->second == 2) 
        {
            pairCount++;
            // if we have 3 of the same card (rank) we by default have a 3 of a kind
        } else if (i->second == 3) 
        {
            hasThreeOfAKind = true;
                // if we have 4 of the same card (rank) we by default have a 4 of a kind
        } else if (i->second == 4) 
        {
            hasFourOfAKind = true;
        }
    }

    // Check for straight (5 ranks in a row by value, like 2,3,4,5,6)
    // we have to check all combinations (7,5) (cant just check adjacents because we could have a pair in the middle of the sorted ranks vector)
    bool isStraight = false;
    vector<int> straightCards;
    
    straightCards.push_back(ranks[0]);
    for (size_t i = 1; i < ranks.size(); i++) 
    {
        // basically since sorted already we can just iterate from beginning to end and just check sequential order
        // the edge case here are two things. Recall that Ace can either be of value effectively like a rheoretical 1(the card below two) and also
        // the card above King (in our case we just define that as 13 since King is 12
        if (ranks[i] != ranks[i - 1] + 1) 
        {
            // not a current straight
            straightCards.clear();
        }
        straightCards.push_back(ranks[i]);
    }
    // straight found ! 
    if (straightCards.size() >= 5) {
        isStraight = true;
    }

    // Special case for Ace-low straight (A-2-3-4-5)
    // Ace is 0, 2 is 1, 3 is 2, 4 is 3, 5 is 4
    bool isAceLowStraight = (ranks[0] == 0 && ranks[1] == 1 &&
                             ranks[2] == 2 && ranks[3] == 3 &&
                             ranks[4] == 4);

    // Special case for Ace-high straight (10-J-Q-K-A)
    // 10 is 9, J is 10, Q is 11, K is 12, A is 0
    bool isAceHighStraight = (ranks[0] == 0 && ranks[ranks.size() - 4] == 9 &&
                              ranks[ranks.size() - 3] == 10 && ranks[ranks.size() - 2] == 11 &&
                              ranks[ranks.size() - 1] == 12);

    // we are dealing with the two edge cases for straight above. We found it easier to hardcode the special cases since
    // they effectively would never change card ranks
    isStraight = isStraight || isAceLowStraight || isAceHighStraight;

    bool isStraightFlush = false;
    // royal flush for reference is essentially just a straight + a hand with 5 cards of the same suit where the straight and suit are all the same cards
    bool isRoyalFlush = false;
    // need to know if the same 5 cards make up a straight and a flush, otherwise we have only a straight
    if (isStraight && isFlush) 
    {
        vector<int> hearts;
        vector<int> diamonds;
        vector<int> clubs;
        vector<int> spades;

        for (size_t i = 0; i < hand.size(); i++) {
            // get the card and its suit, rank
            int card = hand[i];
            int suit = card / 13;
            int rank = card % 13;
            // for each card, get its type (the vectors above) and is rank and put that into the respective vector
            if (suit == 0) 
            {
                hearts.push_back(rank);
            } else if (suit == 1) 
            {
                diamonds.push_back(rank);
            } else if (suit == 2) 
            {
                clubs.push_back(rank);
            } else {
                spades.push_back(rank);
            }
        }
        vector<vector<int>> sortedHand;
        sortedHand.push_back(hearts);
        sortedHand.push_back(diamonds);
        sortedHand.push_back(clubs);
        sortedHand.push_back(spades);

        for (size_t i = 0; i < sortedHand.size(); i++) {
            // Sorting makes it easier to tell if we have a straight 
            sort(sortedHand[i].begin(), sortedHand[i].end());

            // testin 
            for (size_t j = 0; j < sortedHand[i].size(); j++) {
                cout << sortedHand[i][j] << ", ";
            }
            cout << endl;
        }

        bool isAceLowStraightFlush = false;
        // recall that the straight low (Ace through the 5 card) is a special case, so we also have to do that same logic for the 
        // striaght flush 
        for (int i = 0; i < 4; i++) {
            if (sortedHand[i].size() >= 5) {
                vector<int> straightFlushCards;
                straightFlushCards.push_back(sortedHand[i][0]);
        
                for (size_t j = 1; j < sortedHand[i].size(); j++) {
                    cout << sortedHand[i][j] << endl;
                    // we dont have a straight currently
                    if (sortedHand[i][j] != sortedHand[i][j - 1] + 1) {
                        straightFlushCards.clear();
                    }
                    // adding to the return 
                    straightFlushCards.push_back(sortedHand[i][j]);
                }

                for (size_t j = 0; j < straightFlushCards.size(); j++) {
                    cout << straightFlushCards[j] << ", ";
                }
                cout << endl;
                // if we have 5 flush cards and they make a straight, we have a straight flush
                if (straightFlushCards.size() >= 5) {
                    isStraightFlush = true;
                }

                // same edge case as the straight (since this is a special type of straight anyway)
                // could have A, 2, 3, 4, 5 of any suit and be correct. thats what this is considering
                // 0 is 1, 1 is 2..., 4 is 5
                if (sortedHand[i][0] == 0 && sortedHand[i][1] == 1 &&
                    sortedHand[i][2] == 2 && sortedHand[i][3] == 3 &&
                    sortedHand[i][4] == 4) {
                    isAceLowStraightFlush = true;
                }

                // this is the upper straight flush (royal) which is the best hand in poker. it is the A, K, Q, J, 10
                // remember A is =, K is 12, Q is 11... 
                if (sortedHand[i][0] == 0 && sortedHand[i][sortedHand[i].size() - 4] == 9 &&
                    sortedHand[i][sortedHand[i].size() - 3] == 10 && sortedHand[i][sortedHand[i].size() - 2] == 11 &&
                    sortedHand[i][sortedHand[i].size() - 1] == 12) {
                    isRoyalFlush = true;
                }
            }
        }

        // a straight flush is any type of straight flush
        isStraightFlush = isStraightFlush || isAceLowStraightFlush;
    }

    // just returning hand ranking
    if (isRoyalFlush) {
        return "Royal Flush";
    }
    if (isStraightFlush) {
        return "Straight Flush";
    }

    if (hasFourOfAKind) {
        return "Four of a Kind";
    }
    // techically a full house is a three of a kind + a different pair, so thats where pairCount == 1 comesfrom
    if (hasThreeOfAKind && pairCount == 1) {
        return "Full House";
    }

    if (isFlush) {
        return "Flush";
    }

    if (isStraight) {
        // ace low straight is valued worse than ace high straight, so separated
        if (isAceLowStraight) {
            return "Ace Low Straight";
        } else if (isAceHighStraight) {
            return "Ace High Straight";
        }
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
// Return a number rank for each hand type (higher is better)
int handRank(const string& handType) {
    if (handType == "Royal Flush") return 11;
    if (handType == "Straight Flush") return 10;
    if (handType == "Four of a Kind") return 9;
    if (handType == "Full House") return 8;
    if (handType == "Flush") return 7;
    if (handType == "Ace High Straight") return 6;
    if (handType == "Ace Low Straight") return 4;
    if (handType == "Straight") return 5;
    if (handType == "Three of a Kind") return 3;
    if (handType == "Two Pair") return 2;
    if (handType == "One Pair") return 1;
    if (handType == "High Card") return 0;

    // wont happen. just for compilation
    return 0;
}

// sorts the ranks of a hand 
vector<int> getSortedRanks(const vector<int>& hand) {
    // ranks and count their appearences/freq
    vector<int> ranks;
    unordered_map<int, int> rankFrequency;

    for (int card : hand) {
        // where ace is 0, 1 is 2, 12 king, 13 will also be technically 13 since strongest
        int rank = card % 13;
        if (rank == 0) rank = 13;

        // add the card features
        ranks.push_back(rank);
        rankFrequency[rank]++;
    }

    // now we have to create a simple list of all ranks with their frequencies
    vector<pair<int, int>> rankFreqPairs;
    for (const auto& entry : rankFrequency) {
        rankFreqPairs.push_back(entry);
    }

    // sorting by freq
    for (size_t i = 0; i < rankFreqPairs.size(); i++) {
        for (size_t j = i + 1; j < rankFreqPairs.size(); j++) {
            // Compare frequencies
            if (rankFreqPairs[j].second > rankFreqPairs[i].second) {
                // Swap if frequency is higher
                swap(rankFreqPairs[i], rankFreqPairs[j]);
            }
        }
    }

    //  in equal freq sort by rank descendingly
    for (size_t i = 0; i < rankFreqPairs.size(); i++) {
        for (size_t j = i + 1; j < rankFreqPairs.size(); j++) {
            if (rankFreqPairs[j].second == rankFreqPairs[i].second) {
                // make  Ace (0) as highest (13) since it is the strongest card
                int rankA = rankFreqPairs[i].first;
                int rankB = rankFreqPairs[j].first;
                if (rankA == 0) rankA = 13;
                if (rankB == 0) rankB = 13;

                if (rankB > rankA) {
                    swap(rankFreqPairs[i], rankFreqPairs[j]);
                }
            }
        }
    }

    //  the ranks in order in the sorted list 
    vector<int> sortedRanks;
    for (const auto& pair : rankFreqPairs) {
        sortedRanks.push_back(pair.first);
    }

    return sortedRanks;
}
// determine which hand is actually better than the other when same rank
int handWinner(const std::vector<std::vector<int>>& playerHands) {
    int bestIndex = 0;
    // temp best rank 
    int bestRank = handRank(evaluateHand(playerHands[0]));
    // temp best sorted rank
    vector<int> bestSorted = getSortedRanks(playerHands[0]);

    for (size_t i = 1; i < playerHands.size(); ++i) {
        // E.G: get the current rank of the player hand (so basically we get the player hand and get its type (like a Two Pair)
        int currentRank = handRank(evaluateHand(playerHands[i]));

        // if the current hand type is the best hand type
        if (currentRank > bestRank) {
            bestRank = currentRank;
            // set the curr index as best
            bestIndex = i;
            // sorting so that if we have multiple best hands in the same hand evaluation we can get each card in order and check which hand
            // is actually better
            bestSorted = getSortedRanks(playerHands[i]);
        } else if (currentRank == bestRank) {
            // this is how we determine if a given Two Pair is better than another, for example
            vector<int> currentSorted = getSortedRanks(playerHands[i]);

            // Tie-breaker: compare sorted ranks
            for (size_t j = 0; j < currentSorted.size(); ++j) {
                if (j >= bestSorted.size()) break;
                // since both hands are already sorted, whichever hand has higher numbered cards wins (poker rules)
                if (currentSorted[j] > bestSorted[j]) {
                    bestIndex = i;
                    bestSorted = currentSorted;
                    break;
                } else if (currentSorted[j] < bestSorted[j]) {
                    break;  // existing best is better
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

}  // namespace std
