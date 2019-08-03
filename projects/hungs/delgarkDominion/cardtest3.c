#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

void asserttrue(int boolean, char message[]) {
	if (boolean) {
		printf("PASSED: %s", message);
	}
	else {
		printf("FAILED: %s", message);
	}
}


int main() {


	int r1, r2;
	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;

	int k[10] = { adventurer, council_room, feast, gardens, mine,
			 remodel, smithy, village, baron, great_hall };

	struct gameState G, testG, testH;

	initializeGame(numPlayers, k, seed, &G);

	memcpy(&testG, &G, sizeof(struct gameState));

	printf("Testing endTurn.\n");

	int testOne = endTurn(&testG);

	int boolean = 1;

	if (testOne == 0) {
		boolean = 1;
		char message[] = "successfully runs the function\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "successfully runs the function\n";
		asserttrue(boolean, message);
	}

	int prevHandCount = testG.handCount[testG.whoseTurn-1];
	
	if (prevHandCount == 0) {
		boolean = 1;
		char message[] = "reset player's hand count\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "reset player's hand count\n";
		asserttrue(boolean, message);
	}

	if (testG.outpostPlayed == 0) {
		boolean = 1;
		char message[] = "reset outpost\n";
		asserttrue(boolean, message);
	}
	else{
		boolean = 0;
		char message[] = "reset outpost\n";
		asserttrue(boolean, message);
	}

	if (testG.phase == 0) {
		boolean = 1;
		char message[] = "reset phase\n";
		asserttrue(boolean, message);
	}
	else{
		boolean = 0;
		char message[] = "reset phase\n";
		asserttrue(boolean, message);
	}

	if (testG.numActions == 1) {
		boolean = 1;
		char message[] = "reset actions\n";
		asserttrue(boolean, message);
	}
	else{
		boolean = 0;
		char message[] = "reset actions\n";
		asserttrue(boolean, message);
	}

	if (testG.coins == 0) {
		boolean = 1;
		char message[] = "reset coins\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "reset coins\n";
		asserttrue(boolean, message);
	}

	if (testG.numBuys == 1) {
		boolean = 1;
		char message[] = "reset buys\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "reset buys\n";
		asserttrue(boolean, message);
	}

	if (testG.playedCardCount == 0) {
		boolean = 1;
		char message[] = "reset played cards\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "reset played cards\n";
		asserttrue(boolean, message);
	}

	if (testG.handCount[testG.whoseTurn] == 5) {
		boolean = 1;
		char message[] = "reset next player's hand\n";
		asserttrue(boolean, message);
	}
	else {
		boolean = 0;
		char message[] = "reset next player's hand\n";
		asserttrue(boolean, message);
	}

	return 0;
}