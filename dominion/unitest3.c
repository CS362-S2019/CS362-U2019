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


int main() {

	int discarded;

	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;
	int nextPlayer = 1;

	struct gameState G, testG;
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse,
				 sea_hag, tribute, smithy, council_room };

	// initialize a game state and player cards
	initializeGame(numPlayers, k, seed, &G);
	printf("----------------- Testing endTurn----------------\n");
	//current player discards all of his cards
	//state is reset for next player
	//player is then dealt 5 cards
	//copy game state
	memcpy(&testG, &G, sizeof(struct gameState));
	discarded = 5;
	int drawn = 5;
	endTurn(&testG);
	assertCustom(testG.whoseTurn == G.whoseTurn + 1, "Next Player Turn");
	assertCustom(testG.discardCount[thisPlayer] == G.discardCount[thisPlayer] + discarded, "Discard Pile has 5 more cards");
	assertCustom(testG.handCount[thisPlayer] == G.handCount[thisPlayer] - discarded, "Old Player has nom more cards");
	assertCustom(testG.handCount[nextPlayer] == G.handCount[nextPlayer] + drawn, "New player has five cards");
	assertCustom(testG.outpostPlayed == 0, "Number of outposts reset");
	assertCustom(testG.phase == 0, "Phase Reset");
	assertCustom(testG.numActions == 1, "1 Buys");
	assertCustom(testG.playedCardCount = 0, "No Played Cards");
	assertCustom(testG.coins <= 15, "No impossible number of coins");

	return 0;
}