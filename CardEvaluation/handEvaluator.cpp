#include "handEvaluator.h"

#include <algorithm>
#include <unordered_map>

namespace std {
string getCardName(int cardValue) {
    if (cardValue < 0 || cardValue > 51) {
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

string evaluateHand(const vector<int>& hand) {
    //  the hand size
    // if (hand.size() != 5)
    //     return "Invalid hand]";

    // get and sort ranks (0-12 where 0=ace, 1=2, 2=3, ..., 12=king)
    vector<int> ranks;
    unordered_map<int, int> rankFrequency;
    unordered_map<int, int> suitFrequency;

    for (int card : hand) {
        if (card < 0 || card > 51)
            return "Invalid card value";

        int rank = card % 13;  // 0=ace, 1=2, ..., 12=king
        int suit = card / 13;  // 0=hearts, 1=diamonds, 2=clubs, 3=spades

        if (find(ranks.begin(), ranks.end(), rank) == ranks.end()) {  // dont add duplicate rank to ranks
            ranks.push_back(rank);
        }

        rankFrequency[rank]++;
        suitFrequency[suit]++;
    }
    sort(ranks.begin(), ranks.end());

    // Check for flush (all cards of the same suit)
    bool isFlush = false;
    for (size_t i = 0; i < suitFrequency.size(); i++) {
        if (suitFrequency[i] >= 5) {
            isFlush = true;
            break;
        }
    }

    // Count pairs, three of a kind, four of a kind
    int pairCount = 0;
    bool hasThreeOfAKind = false;
    bool hasFourOfAKind = false;

    for (unordered_map<int, int>::iterator i = rankFrequency.begin(); i != rankFrequency.end(); i++) {
        if (i->second == 2) {
            pairCount++;
        } else if (i->second == 3) {
            hasThreeOfAKind = true;
        } else if (i->second == 4) {
            hasFourOfAKind = true;
        }
    }

    // Check for straight (sequential ranks)
    // we have to check all combinations (7,5) (cant just check adjacents because we could have a pair in the middle of the sorted ranks vector)
    bool isStraight = false;
    vector<int> straightCards;
    straightCards.push_back(ranks[0]);
    for (size_t i = 1; i < ranks.size(); i++) {
        if (ranks[i] != ranks[i - 1] + 1) {
            straightCards.clear();
        }
        straightCards.push_back(ranks[i]);
    }

    if (straightCards.size() >= 5) {
        isStraight = true;
    }

    bool isStraightFlush = false;
    // need to know if the same 5 cards make up a straight and a flush, otherwise we have only a straight
    if (isStraight && isFlush) {
        vector<int> hearts;
        vector<int> diamonds;
        vector<int> clubs;
        vector<int> spades;

        for (size_t i = 0; i < hand.size(); i++) {
            int card = hand[i];
            int suit = card / 13;
            int rank = card % 13;
            if (suit == 0) {
                hearts.push_back(rank);
            } else if (suit == 1) {
                diamonds.push_back(rank);
            } else if (suit == 2) {
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

        for (int i = 0; i < 4; i++) {
            if (sortedHand[i].size() >= 5) {

                vector<int> straightFlushCards;
                straightFlushCards.push_back(sortedHand[i][0]);
                for (size_t j = 1; j < sortedHand[i].size(); i++) {
                    if (sortedHand[i][j] != sortedHand[i][j - 1] + 1) {
                        straightFlushCards.clear();
                    }
                    straightFlushCards.push_back(sortedHand[i][j]);
                }

                if (straightFlushCards.size() >= 5) {
                    isStraightFlush = true;
                }
            }
        }
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

    isStraight = isStraight || isAceLowStraight || isAceHighStraight;

    // Evaluate hand ranking
    if (isStraightFlush) {
        // Check for royal flush (10-J-Q-K-A of same suit)
        if (isAceHighStraight) {
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

int handRank(const string& handType) {
    // Return a number rank for each hand type (higher is better)
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

    // wont happen
    return 0;
}

vector<int> getSortedRanks(const vector<int>& hand) {
    // ranks and count their appearences/freq
    vector<int> ranks;
    unordered_map<int, int> rankFrequency;

    for (int card : hand) {
        // where ace is 0, 1 is 2, 12 king, 13 will also be technically 13 since strongest
        int rank = card % 13;
        if (rank == 0) rank = 13;

        ranks.push_back(rank);
        rankFrequency[rank]++;
    }

    // Step 2: Create a simple list of all ranks with their frequencies
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

    //  in equal freq sort by rank desc
    //
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

    //  the ranks in order
    vector<int> sortedRanks;
    for (const auto& pair : rankFreqPairs) {
        sortedRanks.push_back(pair.first);
    }

    return sortedRanks;
}
// determine which hand is actually better than the other when same rank
int handWinner(const std::vector<std::vector<int>>& playerHands) {
    int bestIndex = 0;
    int bestRank = handRank(evaluateHand(playerHands[0]));
    vector<int> bestSorted = getSortedRanks(playerHands[0]);

    for (size_t i = 1; i < playerHands.size(); ++i) {
        int currentRank = handRank(evaluateHand(playerHands[i]));

        if (currentRank > bestRank) {
            bestRank = currentRank;
            bestIndex = i;
            bestSorted = getSortedRanks(playerHands[i]);
        } else if (currentRank == bestRank) {
            vector<int> currentSorted = getSortedRanks(playerHands[i]);

            // Tie-breaker: compare sorted ranks
            for (size_t j = 0; j < currentSorted.size(); ++j) {
                if (j >= bestSorted.size()) break;

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
