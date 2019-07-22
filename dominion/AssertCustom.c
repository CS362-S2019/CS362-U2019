//
// Created by Yuning on 2019/07/21.
//
#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "assertCustom.h"
#include <stdlib.h>

#define TRUE   1
#define FALSE  0
#define NUM_CARDS 27


void assertCustom(int boolean, char * message) {

	if (boolean == TRUE) {

		printf("TEST PASSED: %s\n", message);
	}
	if (boolean == FALSE) {

		printf("TEST FAILED: %s\n", message);


	}

}

void assertGameState(int player, struct gameState * oldG, struct gameState * newG) {
	int failed = FALSE;
	if (oldG->handCount[player] != newG->handCount[player]) {
		printf("TEST FAILED: Hand Count for non-action player Changed\n");
		failed = TRUE;
	}
	if (oldG->numPlayers != newG->numPlayers) {
		printf("TEST FAILED: Number of Players in Game Changed\n");
		failed = TRUE;
	}
	if (oldG->deckCount[player] != newG->deckCount[player]) {
		printf("TEST FAILED: Deck Count for non-action player Changed\n");
		failed = TRUE;
	}
	int i;
	for (i = curse; i < NUM_CARDS; i++) {
		if (oldG->supplyCount[i] != newG->supplyCount[i]) {
			printf("TEST FAILED Card %d Supply Count Changed\n", i);
			failed = TRUE;
		}
		if (oldG->embargoTokens[i] != newG->embargoTokens[i]) {
			printf("TEST FAILED Embargo Token on Card %d Changed\n", i);
			failed = TRUE;

		}
	}
	if (!failed) {
		printf("TEST PASSED: Game State Invariants\n");
	}

}

