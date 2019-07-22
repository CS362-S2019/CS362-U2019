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


	assertCustom(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer], "Deck has the same number of cards");
	assertCustom(testG.coins == G.coins, "No extra coins received");
	assertCustom(testG.whoseTurn == G.whoseTurn, "Same Players Turn");
	assertCustom(testG.numActions == G.numActions, "Number of actions");
	assertCustom(testG.numBuys == G.numBuys, "Number of Buys");
	assertCustom(testG.playedCardCount == G.playedCardCount, "Number of Cards Discarded");
	assertGameState(thisPlayer + 1, &G, &testG);



}



int main() {

	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;
	int bonus = 0;
	struct gameState G, testG;
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
				 sea_hag, tribute, smithy, council_room };

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
	printf("----------------- Testing numHandCards----------------\n");

	//copy game state
	memcpy(&testG, &G, sizeof(struct gameState));
	assertCustom(numHandCards(&testG) == testG.handCount[thisPlayer], "Five Cards in Hand at Start of State");
	invariant_tests(thisPlayer, G, testG);
	testG.handCount[thisPlayer]++;
	assertCustom(numHandCards(&testG) == testG.handCount[thisPlayer], "Six Cards in Hand");
	invariant_tests(thisPlayer, G, testG);

	testG.handCount[thisPlayer]--;
	assertCustom(numHandCards(&testG) == testG.handCount[thisPlayer], "Five Cards in Hand");
	invariant_tests(thisPlayer, G, testG);

	return 0;
}