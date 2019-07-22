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

int main() {
	int newCards = 0;
	int discarded = 1;
	int xtraCoins = 0;
	int shuffledCards = 0;
	int numBuys = 0;
	int numActions = 0;
	int handCount;
	int handpos = 0;
	int i;
	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;
	int bonus = 0;
	struct gameState G, testG;
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
				 sea_hag, tribute, smithy, council_room };

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
	printf("----------------- Testing UpdateCoins----------------\n");
	//Smithy should receive exactly 3-cards
	//copy game state
	memcpy(&testG, &G, sizeof(struct gameState));

	int coins[] = { gold, silver, copper };
	newCards = 3;
	for (i = 0; i < newCards; i++) {
		testG.hand[thisPlayer][testG.handCount[thisPlayer]++] = coins[i];
	}

	xtraCoins = 6;

	updateCoins(thisPlayer, &testG, bonus);

	assertCustom(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards, "Receives 3 cards");
	assertCustom(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer], "Deck has the same number of cards");
	assertCustom(testG.coins == G.coins + xtraCoins, "Six extra coins received");
	assertCustom(testG.whoseTurn == G.whoseTurn, "Same Players Turn");
	assertCustom(testG.numActions == G.numActions, "Number of actions");
	assertCustom(testG.numBuys == G.numBuys, "Number of Buys");
	assertCustom(testG.playedCardCount == G.playedCardCount, "Number of Cards Discarded");
	assertGameState(thisPlayer + 1, &G, &testG);



	return 0;
}