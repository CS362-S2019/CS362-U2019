//
// Created by Yuning 2019/07/21
//

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "assertCustom.h"
#include <stdlib.h>
#define NO_CARDS 0
#define TRUE 1
#define FALSE 0

void invariant_tests(int thisPlayer, struct gameState G, struct gameState testG) {

	assertCustom(testG.handCount[thisPlayer] == G.handCount[thisPlayer], "Receives No cards");
	assertCustom(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer], "Deck has the same number of cards");
	assertCustom(testG.coins == G.coins, "No extra coins received");
	assertCustom(testG.whoseTurn == G.whoseTurn, "Same Players Turn");
	assertCustom(testG.numActions == G.numActions, "Number of actions");
	assertCustom(testG.numBuys == G.numBuys, "Number of Buys");
	assertCustom(testG.playedCardCount == G.playedCardCount, "Number of Cards Discarded");
	assertGameState(thisPlayer + 1, &G, &testG);



}