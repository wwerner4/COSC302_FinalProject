#include <unistd.h>

#include <iostream>

#include "CardEvaluation/handEvaluator.h"
#include "ai.h"
#include "cardDeck.h"
#include "gameGraphics.h"
using namespace std;

// checkSameHand is a test, should be deleted
void checkSameHand();

// init constant values and initial states of some non-constant values
GameState::GameState() {
    checkSameHand();  // to delete

    playingGame = false;
    smallBlind = 100;
    bigBlind = 200;
    dealer = 3;
    numPlayers = 4;

    chips.resize(numPlayers, 10000);
    bustPlayers.resize(numPlayers, false);
}

// control betting for non-user players
void GameState::bet(int player) {
    if (player == 0) {
        return;
    }

    vector<int> handIncludingCommunityCards = hands[player];
    handIncludingCommunityCards.insert(handIncludingCommunityCards.end(), table.begin(), table.end());
    string handType = evaluateHand(handIncludingCommunityCards);

    // if the cpu player chooses to fold
    if (aiShouldFold(player, handType, checkBet, pot, chips)) {
        folds[player] = true;
        nextPlayer();

        // we sleep after cpu actions so that the user has time to see what actions are taken by the cpu
        sleep(1);
        return;
    }

    int aiBet = aiChosenBet(player, handType, checkBet, pot, chips);
    int newBet = aiBet - bets[player];

    // if the ai wants to bet more than the cpu has, go all in
    if (newBet >= chips[player]) {
        newBet = chips[player];
    }

    pot += newBet;              // add the bet to the pot
    chips[player] -= newBet;    // subtract the bet from the player's chips
    bets[player] += newBet;     // add the bet to the player's bets
    hasBet[player] = true;

    // update the bet all players must match
    if (bets[player] > checkBet) {
        checkBet = bets[player];
    }

    // update the minimum allowable bet for the user
    minBet = checkBet - bets[0];
    if (minBet < 0) {
        minBet = 0;
    }

    // update betting ui, betting state for user
    playerBet = minBet;
    playerHasBet = false;

    sleep(1);
    nextPlayer();    // move on to next player

    return;
}

// reset betting at the start of each stage of the game
void GameState::resetBets() {
    bets.clear();
    bets.resize(numPlayers, 0);

    hasBet.clear();
    hasBet.resize(numPlayers, false);

    return;
}

// move on to the next player in the game
void GameState::nextPlayer() {
    turn = (turn + 1) % numPlayers;
    return;
}

// initialize a new hand
void GameState::gameBegin() {

    // reset folds
    folds.clear();
    folds.resize(numPlayers, false);

    // handle the players who are out of chips and can no longer play
    int bustCount = 0;
    for (size_t i = 0; i < chips.size(); i++) {
        if (chips[i] == 0) {
            bustPlayers[i] = true;
            bustCount++;
            folds[i] = true;
        }
    }

    // if too many players are out of chips, end the game
    if (bustCount >= numPlayers - 1) {
        game->playScreen();
        game->gameOver();

        sleep(10);
        game->closeWindow();
    }

    gameStage = 0;          // set gameStage to game start (pre-flop)
    playingGame = true;     // tell the game loop to check and update the game state

    // init bets
    playerHasBet = false;
    playerBet = 0;
    resetBets();

    bool playerBetSet = false;              // used to control the initialization of playerBet and minBet

    // init dealer
    dealer = (dealer + 1) % numPlayers;
    turn = dealer;

    // find small blind player
    do {
        nextPlayer();
    } while (bustPlayers[turn]);

    // perform small blind
    bets[turn] = smallBlind;
    hasBet[turn] = true;
    chips[turn] -= smallBlind;

    // if the user is the small blind, set up their betting indicator
    if (turn == 0) {
        minBet = bigBlind - smallBlind;
        playerBet = minBet;
        playerBetSet = true;
    }

    // find large blind player
    do {
        nextPlayer();
    } while (bustPlayers[turn]);

    // perform large blind
    bets[turn] = bigBlind;
    hasBet[turn] = true;
    chips[turn] -= bigBlind;

    // if the user is the large blind, set up their betting indicator
    if (turn == 0) {
        playerHasBet = true;
        minBet = 0;
        playerBet = minBet;
        playerBetSet = true;
    }

    checkBet = 200;

    nextPlayer();

    // is user does not post blind, set up their betting indicator
    if (!playerBetSet) {
        minBet = bigBlind;
        playerBet = minBet;
    }

    pot = smallBlind + bigBlind;

    table.clear();

    // deal the players' hands
    deck = new CardDeck(1);
    deck->shuffle();

    hands.clear();
    hands.resize(numPlayers);
    for (int i = 0; i < numPlayers; i++) {
        for (int j = 0; j < 2; j++) {
            hands[i].push_back(deck->draw());
        }
    }
}

// this function is called each game loop while the game is being played. it determines what the next game action will be, and if the game stage needs to be incremented
void GameState::checkState() {

    // count the number of folded players
    int numFolds = 0;
    for (size_t i = 0; i < bets.size(); i++) {
        if (folds[i]) {
            numFolds++;
        }
    }

    // if only 1 player remains in the game, end the game
    if (numFolds == numPlayers - 1) {
        gameStage = 4;
        newStage();
        return;
    }

    int moveOn = 0;

    // for each player, if they have bet and their bet matches the checkBet, or the player has folded or run out of chips, increment moveOn
    for (size_t i = 0; i < bets.size(); i++) {
        if ((bets[i] == checkBet && hasBet[i]) || folds[i] || chips[i] == 0) {
            moveOn++;
        }
    }

    // in this condition, we move on to the next stage of the game
    if (moveOn == numPlayers) {
        gameStage = (gameStage + 1) % 5;
        newStage();
        return;
    }

    // if it is the user's turn to bet, we wait until playerHasBet is set to true by gameGraphics.cpp) (when the user presses the bet button)
    if (turn == 0) {
        if (playerHasBet || folds[0]) {

            // if the player hasn't folded, record that they have bet
            if (!folds[0]) {
                hasBet[0] = true;
            }

            // update checkBet
            if (bets[0] > checkBet) {
                checkBet = bets[0];
            }

            nextPlayer();
        }
    } else {            // if it is not the player's turn, determine the cpu's action
        bet(turn);
    }

    return;
}

// this function handles changes in the stage of the game
void GameState::newStage() {
    sleep(1);
    
    // gameStage == 0 is the pre-flop

    // the flop
    if (gameStage == 1) {
        // add 3 table cards
        for (int i = 0; i < 3; i++) {
            table.push_back(deck->draw());
        }

        playerBet = 0;
        checkBet = 0;

        resetBets();
    } else if (gameStage == 2) {        // the turn
        table.push_back(deck->draw());

        playerBet = 0;
        checkBet = 0;

        resetBets();
    } else if (gameStage == 3) {           // the river
        table.push_back(deck->draw());
        delete deck;

        playerBet = 0;
        checkBet = 0;

        resetBets();
    } else {                            // end of the hand
        int winner = determineWinner();
        chips[winner] += pot;       // credit chips to the winner

        game->endScreen();          // draw info about the winner

        sleep(10);
        gameBegin();
    }

    return;
}

// determine who won a hand
int GameState::determineWinner() {
    // all 7 cards
    vector<vector<int>> thePlayerHands;
    // indexing players
    vector<int> thePlayersInTheGame;
    // evaluated hands
    vector<string> handTypes;

    // Collect active players and their full hands
    for (int i = 0; i < numPlayers; i++) {
        if (!folds[i]) {
            // Combine hole cards with community cards
            vector<int> fullHand = hands[i];
            fullHand.insert(fullHand.end(), table.begin(), table.end());
            thePlayerHands.push_back(fullHand);
            thePlayersInTheGame.push_back(i);
        }
    }

    // If only one player remains, they win
    if (thePlayerHands.size() == 1) {
        return thePlayersInTheGame[0];
    }

    // check what is current at each hand
    for (size_t i = 0; i < thePlayerHands.size(); i++) {
        handTypes.push_back(evaluateHand(thePlayerHands[i]));
        cout << handTypes.back() << endl;
    }

    // Find best rank in all the hands
    int bestRank = -1;
    for (size_t i = 0; i < handTypes.size(); i++) {
        int currentRank = handRank(handTypes[i]);
        if (currentRank > bestRank) {
            bestRank = currentRank;
        }
    }

    // get index of best hands
    vector<int> bestHandIndices;
    for (size_t j = 0; j < handTypes.size(); j++) {
        if (handRank(handTypes[j]) == bestRank) {
            bestHandIndices.push_back(j);
        }
    }

    // If best hand only one, return it cause they won
    if (bestHandIndices.size() == 1) {
        return thePlayersInTheGame[bestHandIndices[0]];
    }

    // now ties...
    vector<vector<int>> tiedHands;
    for (int index : bestHandIndices) {
        tiedHands.push_back(thePlayerHands[index]);
    }

    int winningHandIndex = handWinner(tiedHands);                   // break ties
    return thePlayersInTheGame[bestHandIndices[winningHandIndex]];
}

void checkSameHand() {
    vector<vector<int>> hands;
    hands.push_back({13, 7 + 13});
    hands.push_back({0 + 13, 12});

    vector<int> table = {8, 9, 10, 11, 1 + 13};

    // all 7 cards
    vector<vector<int>> thePlayerHands;
    // indexing players
    vector<int> thePlayersInTheGame;
    // evaluated hands
    vector<string> handTypes;

    // Collect active players and their full hands
    for (int i = 0; i < 2; i++) {
        //    if(!folds[i])
        //    {
        // Combine hole cards with community cards
        vector<int> fullHand = hands[i];
        fullHand.insert(fullHand.end(), table.begin(), table.end());
        thePlayerHands.push_back(fullHand);
        thePlayersInTheGame.push_back(i);
        //    }
    }

    // If only one player remains, they win
    if (thePlayerHands.size() == 1) {
        cout << "winner: " << thePlayersInTheGame[0] << endl;
        return;
    }

    // check what is current at each hand
    for (size_t i = 0; i < thePlayerHands.size(); i++) {
        handTypes.push_back(evaluateHand(thePlayerHands[i]));
        cout << handTypes[i] << endl;
    }

    // Find best rank in all the hands
    int bestRank = -1;
    for (size_t i = 0; i < handTypes.size(); i++) {
        int currentRank = handRank(handTypes[i]);
        if (currentRank > bestRank) {
            bestRank = currentRank;
        }
    }

    // get index of best hands
    vector<int> bestHandIndices;
    for (size_t j = 0; j < handTypes.size(); j++) {
        if (handRank(handTypes[j]) == bestRank) {
            bestHandIndices.push_back(j);
        }
    }

    // If best hand only one, return it cause they won
    if (bestHandIndices.size() == 1) {
        cout << "winner: " << thePlayersInTheGame[bestHandIndices[0]] << endl;
        return;
    }

    // now ties...
    vector<vector<int>> tiedHands;
    for (int index : bestHandIndices) {
        tiedHands.push_back(thePlayerHands[index]);
    }

    /*
    for (size_t i = 0; i < thePlayerHands[index].size(); i++) {
        if ()
    }
        */

    int winningHandIndex = handWinner(tiedHands);
    cout << "winner: " << thePlayersInTheGame[bestHandIndices[winningHandIndex]] << endl;
}