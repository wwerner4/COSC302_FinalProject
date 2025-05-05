#include <unistd.h>

#include <iostream>

#include "cardDeck.h"
#include "cardEvaluation/handEvaluator.h"
#include "gameGraphics.h"
//#include "ai.h"
using namespace std;

GameState::GameState() {
    playingGame = false;
    smallBlind = 100;
    bigBlind = 200;
    dealer = 3;

    // AIGame *ai = new AIGame;

    chips.resize(numPlayers, 10000);
}

void GameState::bet(int player) {
    if (player == 0) {
        return;
    }

    // if (aiShouldFold(player)) {
    if (player == 1) {
        folds[player] = true;
        turn = (turn + 1) % numPlayers;

        sleep(1);
        return;
    }

    // bets[player] = aiChosenBet();
    int newBet;
    if (checkBet < 200) {
        newBet = 200 - bets[player];
    } else {
        newBet = checkBet - bets[player];
    }

    pot += newBet;
    chips[player] -= newBet;
    bets[player] += newBet;
    turn = (turn + 1) % numPlayers;

    if (bets[player] > checkBet) {
        checkBet = bets[player];
    }

    hasBet[player] = true;

    minBet = checkBet - bets[0];
    if (minBet < 0) {
        minBet = 0;
    }

    playerBet = minBet;
    playerHasBet = false;

    sleep(1);

    return;
}

void GameState::resetBets() {
    bets.clear();
    bets.resize(numPlayers, 0);

    hasBet.clear();
    hasBet.resize(numPlayers, false);

    return;
}

void GameState::gameBegin() {
    cout << "new hand" << endl;
    playingGame = true;

    playerHasBet = false;
    gameStage = 0;
    playerBet = 0;
    checkBet = 0;
    dealer = (dealer + 1) % numPlayers;

    resetBets();
    bets[(dealer + 1) % numPlayers] = smallBlind;
    bets[(dealer + 2) % numPlayers] = bigBlind;
    hasBet[(dealer + 1) % numPlayers] = true;
    hasBet[(dealer + 2) % numPlayers] = true;

    chips[(dealer + 1) % numPlayers] -= smallBlind;
    chips[(dealer + 2) % numPlayers] -= bigBlind;
    turn = (dealer + 3) % numPlayers;

    if ((dealer + 1) % numPlayers == 0) {
        minBet = bigBlind - smallBlind;
        playerBet = minBet;
    } else if ((dealer + 2) % numPlayers == 0) {
        minBet = 0;
        playerBet = minBet;
    } else {
        minBet = bigBlind;
        playerBet = minBet;
    }

    /*
    for (int i = 0 ; i < numPlayers; i ++) {
        cout << bets[i] << ", ";
    }
    cout << endl;
    */

    pot = smallBlind + bigBlind;

    table.clear();

    folds.clear();
    folds.resize(numPlayers, false);

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

void GameState::checkState() {
    int numFolds = 0;

    for (size_t i = 0; i < bets.size(); i++) {
        if (folds[i]) {
            numFolds++;
        }
    }

    if (numFolds == numPlayers - 1) {
        gameStage = 4;
        newStage();
        return;
    }

    vector<bool> moveOn;

    for (size_t i = 0; i < bets.size(); i++) {
        if ((bets[i] == checkBet && hasBet[i]) || folds[i]) {
            moveOn.push_back(true);
        }
    }

    if (moveOn.size() == (size_t)numPlayers) {
        gameStage = (gameStage + 1) % 5;
        newStage();
        return;
    }

    if (turn == 0) {
        if (playerHasBet || folds[0]) {

            if (!folds[0]) {
                hasBet[0] = true;
            }

            if (bets[0] > checkBet) {
                checkBet = bets[0];
            }

            turn = (turn + 1) % numPlayers;
        }
    } else {
        cout << "ibet: " << turn << endl;
        bet(turn);
    }

    return;
}

void GameState::newStage() {
    /*
    cout << "gameStage: " << gameStage << endl;
    cout << "checkBet: " << checkBet << endl;
    for (size_t i = 0; i < bets.size(); i++) {
        cout << bets[i] << ", ";
    }
    cout << endl;

    for (size_t i = 0; i < hasBet.size(); i++) {
        cout << hasBet[i] << ", ";
    }
    cout << endl;
    */

    sleep(1);

    if (gameStage == 1) {
        for (int i = 0; i < 3; i++) {
            table.push_back(deck->draw());
        }

        playerBet = 0;
        checkBet = 0;

        resetBets();
    } else if (gameStage == 2) {
        table.push_back(deck->draw());

        playerBet = 0;
        checkBet = 0;

        resetBets();
    } else if (gameStage == 3) {
        table.push_back(deck->draw());
        delete deck;

        playerBet = 0;
        checkBet = 0;

        resetBets();
    } else {
        int winner = determineWinner();
        chips[winner] += pot;

        sleep(5);
        gameBegin();
    }

    return;
}

int GameState::determineWinner() 
{
    vector<vector<int>> thePlayerHands;
    vector<int> thePlayersInTheGame;
    vector<string> hands; 


    for(int i = 0; i < numPlayers; i++)
    {
        // only adding the current players that havent folded
        if(!folds[i])
        {
            vector<int> fullHand = hands[i];
            fullHand.insert(fullHand.end(), table.begin(), table.end());
            activePlayerHands.push_back(fullHand);
            activePlayers.push_back(i);
        }
    }
    // we got all the hands in thePlayerHands now that are still in tehg ame
    for(size_t i = 0; i < thePlayerHands.size(); i++)
    {
        hands.push_back(evaluateHand(thePlayerHands[i]));
    }
    // -1 is me placeholder value
    int bestRank = -1;

    // find the best ranking hand
    for(size_t i = 0; i < hands.size(); i++)
    {
        int rank = handRank(hands[i]);
        if(rank > bestRank)
        {
            bestRank = rank;
        }
    }

    // now get the highest value of the best ranking hand (since there could be multiple of same best hand type)
    // vector<
    // for(size_t j = 0; j < hands.size(); j++)
    // {

    // }
    // return 0;
}
