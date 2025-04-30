#include <iostream>

#include "cardDeck.h"
#include "gameGraphics.h"
using namespace std;

GameState::GameState() {
    playingGame = false;
    pot = 0;
    smallBlind = 100;
    bigBlind = 200;
    dealer = 3;

    chips.resize(numPlayers, 10000);
    bets.resize(numPlayers, -1);
    hands.resize(numPlayers);
    folds.resize(numPlayers, false);
}

void GameState::bet(int player) {
    if (player == 0) {
        return;
    }

    // if (aiShouldFold(player)) {
    if (0) {
        folds[player] = true;
        return;
    }

    // bets[player] = aiChosenBet();
    bets[player] = 200;
    pot += bets[player];
    chips[player] -= bets[player];
    turn = (turn + 1) % numPlayers;
    return;
}

void GameState::resetBets() {
    bets.clear();
    bets.resize(numPlayers, -1);
}

void GameState::gameBegin() {
    cout << "new hand" << endl;
    playingGame = true;

    gameStage = 0;
    dealer = (dealer + 1) % numPlayers;

    resetBets();
    bets[(dealer + 1) % numPlayers] = smallBlind;
    bets[(dealer + 2) % numPlayers] = bigBlind;
    turn = (dealer + 3) % numPlayers;

    for (int i = 0 ; i < numPlayers; i ++) {
        cout << bets[i] << ", ";
    }
    cout << endl;

    pot = smallBlind + bigBlind;

    deck = new CardDeck(1);
    deck->shuffle();

    hands.clear();
    hands.resize(numPlayers);
    for (int i = 0; i < numPlayers; i++) {
        for (int j = 0; j < 2; j++) {
            hands[i].push_back(deck->draw());
        }
    }

    table.clear();

    folds.clear();
    folds.resize(numPlayers, false);
}

void GameState::checkState() {
    int numFolds = 0;

    vector<int> remainingPlayerBets;
    for (size_t i = 0; i < bets.size(); i++) {
        if (folds[i]) {
            numFolds++;
        } else {
            remainingPlayerBets.push_back(bets[i]);
        }
    }

    if (numFolds == numPlayers - 1) {
        cout << "here" << endl;
        gameStage = 4;
        newStage();
        return;
    }

    bool moveOn = true;

    for (size_t i = 1; i < remainingPlayerBets.size(); i++) {
        if (remainingPlayerBets[i - 1] != remainingPlayerBets[i] || remainingPlayerBets[i] == -1 || remainingPlayerBets[i - 1] == -1) {
            moveOn = false;
        }
    }

    if (moveOn) {
        cout << "here" << endl;
        gameStage = (gameStage + 1) % 5;
        newStage();
        return;
    }

    if (turn == 0) {
        if (bets[0] != -1 || folds[0]) {
            turn++;
        }
    } else {
        bet(turn);
    }

    return;
}

void GameState::newStage() {
    if (gameStage == 1) {
        for (int i = 0; i < 3; i++) {
            table.push_back(deck->draw());
        }

        /*
        for (int i = 0; i < numPlayers; i++) {
            cout << bets[0] << endl;
        }
            */

        resetBets();
    } else if (gameStage == 2) {
        table.push_back(deck->draw());

        resetBets();
    } else if (gameStage == 3) {
        table.push_back(deck->draw());
        delete deck;

        resetBets();
    } else {
        int winner = determineWinner();
        chips[winner] += pot;
        gameBegin();
    }

    cout << gameStage << endl;
    return;
}

int GameState::determineWinner() {
    return 0;
}